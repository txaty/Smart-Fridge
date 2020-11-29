#ifndef _SNTP_H
#define _SNTP_H

#include "rtc.h"

#define NTP_PORT "123"

// #define NTP_SERVER_LIST "0.hk.pool.ntp.org", \
//                         "1.hk.pool.ntp.org", \
//                         "2.hk.pool.ntp.org", \
//                         "0.hk.pool.ntp.org"

#define NTP_SERVER_IP "45.76.101.197"

#define NTP_LOCAL_DIFFERENCE +(9 * 60 * 60)

#define NTP_DELAY_SYNC 10u

#define NTP_DELAY_FAILURE 2u

#define NTP_SERVER_YEARS 2208988800UL

#define NTP_PACKET_SIZE 48u

#define SYSTEM_RTC &hrtc

int ntp_client(void);

#endif