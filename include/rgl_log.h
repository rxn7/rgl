#ifndef __RGL_LOG_H
#define __RGL_LOG_H

#ifdef RGL_NO_LOG
#define RGL_LOG(...)
#define RGL_LOG_ERROR(...)
#else
#define RGL_LOG(fmt, args...) printf("[RGL] " fmt "\n", ##args)
#define RGL_LOG_ERROR(fmt, args...) fprintf(stderr, "[RGL ERROR] " fmt "\n", ##args)
#endif

#endif /* __RGL_LOG_H */
