#ifndef _DEBUG_H_
#define _DEBUG_H_

void ledSwitchRGB(void *pdata);

#if defined(__CC_ARM) || defined(__ICCARM__)

int fputc(int ch, FILE *f);

#elif defined(__GNUC__)

int _write(int fd, char *ptr, int len);

#endif

#endif