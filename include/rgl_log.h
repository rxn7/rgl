#ifndef __RGL_LOG_H
#define __RGL_LOG_H

#define RGL_LOG(fmt, args...) printf("[RGL] " fmt "\n", ##args)
#define RGL_LOG_ERROR(fmt, args...) fprintf(stderr, "[RGL ERROR] " fmt "\n", ##args)

#endif /* __RGL_LOG_H */
