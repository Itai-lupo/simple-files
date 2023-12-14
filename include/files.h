#pragma once
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // !_GNU_SOURCE

#include <sys/poll.h>
#include <sys/types.h>

#include "types/err_t.h"
#include "types/fd_t.h"

/**
 * @brief this is the callback function type for safePpoll
 *
 * @param events the fds and the events that poll got.
 * @param shouldContinue if the function should keep polling for more events or return
 * @param ptr a data that the user of ppoll can pass to it's functions
 * @returns err_t if there was any error in the callback will be quite rethrow to the caller of safe_ppoll
 * @see safePpoll
 */
typedef err_t (*pollCallback)(struct pollfd *events, bool *shouldContinue, void *ptr);

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief open a file descriptor to a file in path
	 *
	 * @param path the path to the file
	 * @param flags the flags to open the file with
	 * @param mode the mode the file should be opened in
	 * @param fd the result fd will be placed here
	 * @return err_t f there was an error with the parms or while opening the file
	 * @see https://man7.org/linux/man-pages/man2/open.2.html
	 */
	THROWS err_t safeOpen(const char *const path, int flags, mode_t mode, fd_t *fd);

	/**
	 * @brief closes fd
	 *
	 * @param fd the file descriptor to close
	 * @return err_t if there was an error with the parms or while closing the file
	 * @see https://man7.org/linux/man-pages/man2/close.2.html
	 */
	err_t safeClose(fd_t *fd);

	/**
	 * @brief reads from fd to buf up to size
	 *
	 * @param fd the file descriptor to read from.
	 * @param buf the buffer to read into.
	 * @param size the size of the buffer.
	 * @param outSize how many bytes were read from fd.
	 * @return err_t if there was any error while reading from the files or with the parms
	 * @see https://man7.org/linux/man-pages/man2/read.2.html
	 */
	THROWS err_t safeRead(fd_t fd, void *buf, size_t size, ssize_t *outSize);

	/**
	 * @brief write buf of size size to fd
	 *
	 * @param fd the file descriptor to write to.
	 * @param buf the buffer to write.
	 * @param size the size of the buffer.
	 * @param bytesWritten how many bytes were written to fd, on full write should be size
	 * @return err_t if there was any error while writing to the files or with the parms
	 * @see https://man7.org/linux/man-pages/man2/write.2.html
	 */
	THROWS err_t safeWrite(fd_t fd, void *buf, size_t size, ssize_t *bytesWritten);

	/**
	 * @brief poll on fds until the callback tells it to stop or until error
	 *
	 * @param fds the files descriptors to monitor
	 * @param ntfds the amount of file descriptors to monitor
	 * @param timeout the maxim amount of time until callback will called next, can be null
	 * @param sigmask a sigmask to apply while ppoll is running, can be null(will not change the sigmask)
	 * @param callback a function to call when there is events from ppoll or on timeout
	 * @param ptr data that will be passes to the callback
	 * @return err_t if there was any error from ppoll or in the callback or if one of the parems was invalid
	 * @see pollCallback
	 * @see man poll https://man7.org/linux/man-pages/man2/poll.2.html
	 */
	THROWS err_t safePpoll(struct pollfd *fds, int ntfds, struct timespec *timeout, const sigset_t *sigmask,
						   pollCallback callback, void *ptr);

#ifdef __cplusplus
}
#endif
