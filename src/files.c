#include "files.h"

#include "defaultTrace.h"
#include "err.h"

#include <fcntl.h>
#include <sys/poll.h>
#include <time.h>

#define NAX_IO_RETRIES 3

THROWS err_t safe_open(const char *const path, int flags, mode_t mode, fd_t *fd)
{
	err_t err = NO_ERRORCODE;

	QUITE_CHECK(path != NULL);
	QUITE_CHECK(fd != NULL);
	QUITE_CHECK(IS_INVALID_FD(*fd));

	for (size_t i = 0; i < NAX_IO_RETRIES && IS_INVALID_FD(*fd); i++)
	{
		fd->fd = open(path, flags, mode);
		QUITE_CHECK(IS_VALID_FD(*fd) || EINTR);
	}
	QUITE_CHECK(IS_VALID_FD(*fd));
cleanup:
	return err;
}

THROWS err_t safe_read(fd_t fd, void *buf, size_t size, ssize_t *outSize)
{
	err_t err = NO_ERRORCODE;
	ssize_t tempOutSize = -1;

	QUITE_CHECK(outSize != NULL);
	QUITE_CHECK(buf != NULL);

	for (size_t i = 0; i < NAX_IO_RETRIES && tempOutSize == -1; i++)
	{
		tempOutSize = read(fd.fd, buf, size);
		CHECK(((tempOutSize != -1) || errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR));
	}

	QUITE_CHECK((tempOutSize != -1));

cleanup:
	*outSize = tempOutSize;
	return err;
}

THROWS err_t safe_write(fd_t fd, void *buf, size_t size, ssize_t *bytesWritten)
{
	err_t err = NO_ERRORCODE;
	ssize_t tempBytesWritten = -1;

	QUITE_CHECK(bytesWritten != NULL);
	QUITE_CHECK(buf != NULL);

	for (size_t i = 0; i < NAX_IO_RETRIES && tempBytesWritten == -1; i++)
	{
		tempBytesWritten = write(fd.fd, buf, size);
		QUITE_CHECK(((tempBytesWritten != -1) || errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR));
	}

	QUITE_CHECK((tempBytesWritten != -1));
cleanup:
	*bytesWritten = tempBytesWritten;
	return err;
}

err_t safe_close(fd_t *fd)
{
	err_t err = NO_ERRORCODE;
	QUITE_CHECK(close(fd->fd));

cleanup:
	*fd = INVALID_FD;
	return err;
}

THROWS err_t safe_ppoll(struct pollfd *fds, int ntfds, struct timespec *timeout, const sigset_t *sigmask,
					   pollCallback callback, void *ptr)
{
	err_t err = NO_ERRORCODE;
	bool shouldCountinue = true;
	int pollNum = 0;

	CHECK(callback != NULL);
	CHECK(fds != NULL);
	CHECK(ntfds != 0);
	while (shouldCountinue)
	{
		pollNum = ppoll(fds, ntfds, timeout, sigmask);
		QUITE_CHECK(pollNum != -1 || EINTR);

		callback(fds, &shouldCountinue, ptr);
	}

cleanup:
	return err;
}
