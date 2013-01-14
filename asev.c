#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "picoev.h"
#include "sandbox.h"

#define HOST 0 /* 0x7f000001 for localhost */
#define PORT 23456
#define MAX_FDS 1024
#define TIMEOUT_SECS 10

static void 
set_nonblock (int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  assert(0 <= r && "Setting socket non-block failed!");
}

static void
setup_sock(int fd)
{
  int on = 1, r;

  /* Try to use TCP Fast Open */
  /* The qlen value specifies this server's limit on the size of the 
   * queue of TFO requests that have not yet completed the three-way handshake 
   */
  int qlen = 5;
  r = setsockopt(fd, SOL_TCP, TCP_FASTOPEN, &qlen, sizeof(qlen));
  if(r != 0)
  {
    r = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
    assert(r == 0);
  }

  set_nonblock(fd);
}

static void 
close_conn(picoev_loop* loop, int fd)
{
  picoev_del(loop, fd);
  close(fd);
  printf("closed: %d\n", fd);
}

static void 
rw_callback(picoev_loop* loop, int fd, int events, void* cb_arg)
{
  if ((events & PICOEV_TIMEOUT) != 0)
  {  
    /* timeout */
    close_conn(loop, fd);
  } 
  else if ((events & PICOEV_READ) != 0) 
  {
    /* update timeout, and read */
    char buf[1024];
    ssize_t r;
    picoev_set_timeout(loop, fd, TIMEOUT_SECS);
    r = read(fd, buf, sizeof(buf));

    switch (r) 
    {
      case 0: /* connection closed by peer */
        close_conn(loop, fd);
        break;

      case -1: /* error */
        if (errno == EAGAIN || errno == EWOULDBLOCK) 
        { /* try again later */
          break;
        }
        else
        { /* fatal error */
          close_conn(loop, fd);
        }
        break;
        
      default: /* got some data, send back */
        if (write(fd, buf, r) != r) 
        {
  	       close_conn(loop, fd); /* failed to send all data at once, close */
        }
        break;
    }
  }
}

static void 
accept_callback(picoev_loop* loop, int fd, int events, void* cb_arg)
{
  int newfd = accept(fd, NULL, NULL);
  if (newfd != -1)
  {
    printf("connected: %d\n", newfd);
    setup_sock(newfd);
    picoev_add(loop, newfd, PICOEV_READ, TIMEOUT_SECS, rw_callback, NULL);
  }
}

int 
main(void)
{
  picoev_loop* loop;
  int listen_sock, flag;
  
  /* listen to port */
  assert((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) != -1);
  flag = 1;
  assert(setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == 0);
  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(PORT);
  listen_addr.sin_addr.s_addr = htonl(HOST);
  assert(bind(listen_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) == 0);
  assert(listen(listen_sock, 5) == 0);
  setup_sock(listen_sock);
  
  /* init picoev */
  picoev_init(MAX_FDS);
  /* create loop */
  loop = picoev_create_loop(60);
  /* add listen socket */
  picoev_add(loop, listen_sock, PICOEV_READ, 0, accept_callback, NULL);

  /* Seccomp sandbox setup */
  sandbox_init();
  sandbox_setup();
  sandbox_lockdown();

  /* loop */
  while (1)
  {
    fputc('.', stdout);
    fflush(stdout);
    picoev_loop_once(loop, 10);
  }
  /* cleanup */
  picoev_destroy_loop(loop);
  picoev_deinit();
  
  return 0;
}
