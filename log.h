#ifndef LOG_H_
#define LOG_H_

#if defined(HOMER_LOG) && HOMER_LOG > 0
#define LOG(x) homer_log x
#else
#define LOG(x) do {} while (0)
#endif

void homer_log(const char* fmt, ...);

#endif
