#ifndef _GLOBALCONST_H_
#define _GLOBALCONST_H_

/**
 * open the config file and read config information
 */
void initializeConstant();

int const_tls, const_tlm, const_forbidden;

int timeLimitSecond();

int timeLimitMillis();

int allowForbidden();

#endif
