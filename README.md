libasev
=======

Requires:
- linux with:
- epoll
- seccomp-bpf
- TCP Fast Open
- x86-64

The TCP Fast Open means that at least kernel 3.7 is needed.

Support for other platforms is not planned.

Resources
---
  - [epoll](http://linux.die.net/man/4/epoll)
  - [seccomp-bpf](http://lwn.net/Articles/475043/)
  - [TCP Fast Open](https://lwn.net/Articles/508865/)