#pragma once

#define RGL_LOG(fmt, args...) printf("[RGL] " fmt "\n", ##args)
#define RGL_LOG_ERROR(fmt, args...) fprintf(stderr, "[RGL ERROR] " fmt "\n", ##args)
