#include "sntp.h"
#include "time.h"
#include "rtc.h"
#include "sal_module_wrapper.h"

int set_rtc_time(struct tm* local_time)
{
    RTC_TimeTypeDef rtc_timer;
    rtc_timer.Hours = local_time->tm_hour;
    rtc_timer.Minutes = local_time->tm_min;
    rtc_timer.Seconds = local_time->tm_sec;
    HAL_RTC_SetTime(SYSTEM_RTC, &rtc_timer, RTC_FORMAT_BIN);
    return 0;
}

int ntp_client(void)
{
    uint8_t buffer[NTP_PACKET_SIZE];
    uint8_t ntp_data[NTP_PACKET_SIZE];
    ntp_data[0] = 0xa3;
    int socket_id = -1;
    int recv_len = 0;
    uint32_t tx_tm_s;
    uint32_t local_time_stamp;
    struct tm *net_time;
    int i;
    for (i = 1; i < 48; ++i)
    {
        ntp_data[i] = 0;
    }
    socket_id = tos_sal_module_connect(NTP_SERVER_IP, NTP_PORT, TOS_SAL_PROTO_UDP);
    if (socket_id == -1)
    {
        printf("Connection failed!\r\n");
        return;
    }
    else
    {
        tos_sal_module_sendto(socket_id, NTP_SERVER_IP, NTP_PORT, ntp_data, NTP_PACKET_SIZE);
        recv_len = tos_sal_module_recvfrom_timeout(socket_id, buffer, sizeof(buffer), 8000);
        if (recv_len != 0)
        {
            tx_tm_s = buffer[40] << 24 | buffer[40 + 1] << 16 | buffer[40 + 2] << 8 | buffer[40 + 3];
            local_time_stamp = tx_tm_s + NTP_SERVER_YEARS;
            local_time_stamp += NTP_LOCAL_DIFFERENCE;
            net_time = localtime(&local_time_stamp);
            if (set_rtc_time(net_time) == 0) {
                tos_sal_module_close(socket_id);
                return 0;
            }
            // printf("%d-%d-%d %d:%d:%d\r\n", (net_time->tm_year) + 1900, (net_time->tm_mon) + 1, net_time->tm_mday, 
            //         net_time->tm_hour, net_time->tm_min, net_time->tm_sec);
            tos_sal_module_close(socket_id);
        }
    }
    return -1;
}