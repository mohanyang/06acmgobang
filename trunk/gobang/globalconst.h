#ifndef _GLOBALCONST_H_
#define _GLOBALCONST_H_

/**
 * open the config file and read config information
 */
void initializeConstant();

int const_tls, const_tlm, const_forbidden;

#define allowForbidden() (const_forbidden)

#define timeLimitSecond() (const_tls)

#define timeLimitMillis() (const_tlm)

#endif
