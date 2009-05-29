#ifndef _GLOBALCONST_H_
#define _GLOBALCONST_H_

/**
 * open the config file and read config information
 */
void initializeConstant();

int const_tls, const_tlm, const_forbidden;

inline
int timeLimitSecond() {
  return const_tls;
}

inline
int timeLimitMillis() {
  return const_tlm;
}

inline
int allowForbidden() {
  return const_forbidden;
}

#endif
