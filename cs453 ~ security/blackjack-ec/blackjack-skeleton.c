
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

ssize_t readn(int fd, char *buffer, size_t count)
{
	int offset, block;

	offset = 0;
	while (count > 0)
	{
		block = read(fd, &buffer[offset], count);

		if (block < 0) return block;
		if (!block) return offset;

		offset += block;
		count -= block;
	}

	return offset;
}

ssize_t writen(int fd, char *buffer, size_t count)
{
	int offset, block;

	offset = 0;
	while (count > 0)
	{
		block = write(fd, &buffer[offset], count);

		if (block < 0) return block;
		if (!block) return offset;

		offset += block;
		count -= block;
	}

	return offset;
}

int nprintf(int fd, char *format, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	return writen(fd, buf, strlen(buf));
}

int readline(int fd, char *buf, size_t maxlen)
{
	int offset;
	ssize_t n;
	char c;

	offset = 0;
	while (1)
	{
		n = read(fd, &c, 1);

		if (n < 0 && n == EINTR)
			continue;

		if (c == '\n')
		{
			buf[offset++] = 0;
			break;
		}

		buf[offset++] = c;

		if (offset >= maxlen)
			return 0;
	}

	return 1;
}

int main(void)
{
	char host[] = "178.79.156.94";
	int port = 5555;
	int fd = -1;
	struct sockaddr_in sa;
	char buf[4096];
	ssize_t n;
	int epoch;
	while(1){
		if (!inet_aton(host, &sa.sin_addr))
		{
			perror("inet_aton");
			return 1;
		}

		if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			perror("socket");
			return 1;
		}

		sa.sin_port = htons(port);
		sa.sin_family = AF_INET;

		if (connect(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) < 0)
		{
			perror("inet_aton");
			return 1;
		}

		if (!readline(fd, buf, sizeof(buf)))
			return 1;

		printf("buf = %s\n", buf);

		nprintf(fd, "LOGIN josemedina Xv5Nbg\n");

		if (!readline(fd, buf, sizeof(buf)))
			return 1;

		//***********BET*******************
		nprintf(fd, "BET 50000\n");
		if (!readline(fd, buf, sizeof(buf)))
			return 1;
		printf("buf = %s\n", buf);
		//***********BET********************
		
		if (buf[22]=='2' && buf[23]=='1'){
			nprintf(fd, "STAND\n");
			if (!readline(fd, buf, sizeof(buf)))
				return 1;
			printf("buf = %s\n", buf);
		}
	

		nprintf(fd, "QUIT\n");
		if (!readline(fd, buf, sizeof(buf)))
			return 1;
		printf("buf = %s\n", buf);

	}

	return 0;
}

