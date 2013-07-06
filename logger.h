#ifndef LOGGER_H
#define LOGGER_H 1

#ifdef _DEBUG
#define debug_log(fmt, ...) printf("%s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)
#else
#define debug_log(fmt, ...) {}
#endif

#endif
