#ifndef __RGL_LOG_H
#define __RGL_LOG_H

#define ANSII_WHITE "\e[0;37m"
#define ANSII_RED "\e[0;31m"
#define ANSII_YELLOW "\e[0;33m"

#define ANSII_BOLD_WHITE "\e[1;37m"
#define ANSII_BOLD_RED "\e[1;31m"
#define ANSII_BOLD_YELLOW "\e[1;33m"

#define RGL_LOG(fmt, args...) printf(ANSII_BOLD_WHITE "[RGL] " fmt "\n" ANSII_WHITE, ##args)
#define RGL_LOG_ERROR(fmt, args...) printf(ANSII_BOLD_RED "[RGL ERROR] " fmt "\n" ANSII_WHITE, ##args)
#define RGL_LOG_WARN(fmt, args...) print(ANSII_BOLD_YELLOW "[RGL WARN] " fmt "\n" ANSII_WHITE, ##args)

#endif /* __RGL_LOG_H */
