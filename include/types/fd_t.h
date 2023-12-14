/**
 * @file types/fd_t.h
 * @brief the file containe the type fd_t
 */
#pragma once

/**
 * @struct fd_t
 * @brief this is a wrapper to the linux fd which allow for type safety for file operations
 *
 * @var fd_t::fd
 * the raw fd
 */
typedef struct
{
	int fd;
} fd_t;

/**
 * @brief this is the value you should put on new fds that are yet to have value
 */
#define INVALID_FD                                                                                                     \
	(fd_t)                                                                                                             \
	{                                                                                                                  \
		-1                                                                                                             \
	}

/**
 * @brief check if an fd struct is a valid fd(.fd is not negative)
 *
 * @param  fd_t the fd to check if it is valid
 * @returns bool if fd is valid
 */
#define IS_VALID_FD(fdToCheck) ((fdToCheck).fd >= 0)

/**
 * @brief check if an fd struct is a not valid fd(.fd is negative)
 *
 * @param fd_t fdToCheck the fd to check if it is invalid, should of type
 * @returns bool if fd is invalid
 */
#define IS_INVALID_FD(fdToCheck) (!IS_VALID_FD(fdToCheck))

/**
 * @brief get the raw fd
 *
 * @param fd_t the fd to get the raw value of, should be of type \a fd_t \a
 * @returns int  if the raw fd
 */
#define GET_FD(fdVar) fdVar.fd
