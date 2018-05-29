
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include "md5/md5.h"
#include "auth.h"

#define AUTH_HEX_SIZE    32
#define AUTH_HASH_SIZE   16

// srclen must be an even number
// dst must be able to hold 2*srclen + 1
static void hexify(unsigned char *src, unsigned int srclen, unsigned char *dst)
{
	static const char * const hexchars = "0123456789abcdef";
	unsigned int i = 0;

	while (i < srclen)
	{
		*dst++ = hexchars[(*src >> 4) & 0xf];
		*dst++ = hexchars[*src & 0xf];
		src++;
		i++;
	}

	*dst = 0;
}

int auth_userpass(char *user, char *pass)
{
	int fd;
	char pathname[1024];
	char *cp;
	ssize_t size;
	MD5_CTX ctx;
	unsigned char hash[16];
	unsigned char stored[AUTH_HEX_SIZE + 1];
	unsigned char phash[AUTH_HEX_SIZE + 1];

	for (cp = user; *cp; cp++)
		if (!isalnum(*cp))
			return 0;

	snprintf(pathname, sizeof(pathname), "auth/%s", user);

	fd = open(pathname, O_RDONLY);

	if (fd < 0)
		return 0;

	size = read(fd, stored, AUTH_HEX_SIZE);
	close(fd);

	if (size < AUTH_HEX_SIZE)
		return 0;

	stored[AUTH_HEX_SIZE] = 0;

	MD5_Init(&ctx);
	MD5_Update(&ctx, pass, strlen(pass));	
	MD5_Final(hash, &ctx);

	hexify(hash, AUTH_HASH_SIZE, phash);

	if (memcmp(phash, stored, AUTH_HEX_SIZE))
		return 0;

	return 1;
}

