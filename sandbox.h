#ifndef ASEV_SANDBOX_H
#define ASEV_SANDBOX_H

struct asev_session;

void sandbox_init();

void sandbox_setup(const struct asev_session* p_sess);

void sandbox_lockdown();

#endif /* ASEV_SANDBOX_H */
