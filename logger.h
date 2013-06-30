#ifndef LOGGER_H
#define LOGGER_H 1

#define debug_log(fmt, ...) printf("%s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

#endif
