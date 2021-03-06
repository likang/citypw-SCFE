#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*
Write everything even something bad happened. End conditions are:
--------------------------------------------------------------------------------
write all len bytes( return len)
--------------------------------------------------------------------------------
And, execeptional handling:
--------------------------------------------------------------------------------
return a value less than len, re-calculate the buffer and number of
bytes haven't write yet, then reissue write() again.  return -1 and
errno=EINTR, it's signal break before read anything.  return -1 and
errno=?, loop terminates. Note: write() is less likely to return a
partial write than read()
--------------------------------------------------------------------------------
*/

void write_all(int fd, void *buf, size_t len)
{
	ssize_t ret;
	while (len != 0 && (ret = write(fd, buf, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR) /* EINTR = signal break, EAGAIN = no data currently available */
				continue;
			perror("write");
			break;
		}
		len -= ret;
		buf += ret;
	}
}

int main(int argc, char *argv[])
{
	int fd;
	char buf[256] = "How you doing? dude";

	fd = open("test.log", O_RDWR | O_EXCL, 0644);

	write_all(fd, buf, sizeof(buf));

	printf("buf: %s\n", buf);
	return 0;
}
