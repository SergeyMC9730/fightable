#pragma once

#ifdef TARGET_UNIX
#define NPD_CLOSE(fd) close(fd)
#define NPD_DUP2(fd, n) dup2(fd, n)
#define NPD_READ(fd, buf, sz) read(fd, buf, sz)
#define NPD_FSTAT(fd, buf) fstat(fd, buf)
#define NPD_WRITE(fd, data, sz) write(fd, data, sz)
#elif defined(TARGET_WIN32)
#define NPD_CLOSE(fd) _close(fd)
#define NPD_DUP2(fd, n) _dup2(fd, n)
#define NPD_READ(fd, buf, sz) _read(fd, buf, sz)
#define NPD_FSTAT(fd, buf) _fstat(fd, buf)
#define NPD_WRITE(fd, data, sz) _write(fd, data, sz
#else
#define NPD_CLOSE(fd) -1
#define NPD_DUP2(fd, n) -1
#define NPD_READ(fd, buf, sz) -1
#define NPD_FSTAT(fd, buf) -1
#define NPD_WRITE(fd, data, sz) -1
#endif