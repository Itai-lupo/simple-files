#pragma once
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // !_GNU_SOURCE
 
#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>

#include "defines/errType.h"

typedef struct
{
	int fd;
} fd_t;

#define INVALID_FD                                                                                                     \
	(fd_t)                                                                                                             \
	{                                                                                                                  \
		-1                                                                                                             \
	}

#define IS_VALID_FD(fdToCheck) ((fdToCheck).fd != -1)
#define IS_INVALID_FD(fd) (!IS_VALID_FD(fd))

#ifdef __cplusplus
extern "C"
{
#endif


THROWS err_t safe_open(const char *const path, int flags, mode_t mode, fd_t *fd);
err_t safe_close(fd_t *fd);
THROWS err_t safe_read(fd_t fd, void *buf, size_t size, ssize_t *outSize);

THROWS err_t safe_write(fd_t fd, void *buf, size_t size, ssize_t *bytesWritten);

THROWS err_t safe_poll(struct pollfd *fds, int ntfds, int timeout,
					   err_t callback(struct pollfd *events, bool *shouldCountinue, void *ptr), void *ptr);


#ifdef __cplusplus
}
#endif


