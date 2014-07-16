#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SECTOR_SIZE 512
#define DEFAULT_BUF_SIZE (SECTOR_SIZE * 32)
#define DATA_SIZE (1048576UL * 5)

int main(int argc, char **argv)
{
	int BUF_SIZE = DEFAULT_BUF_SIZE;
	int i, fd, rv;
	char *addr = NULL;
	void *buf = NULL;
	off_t offset;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	BUF_SIZE = atoi(argv[2]);

	/* 
	 * O_DIRECT로 열린 파일은 기록하려는 메모리 버퍼, 파일의 오프셋, 버퍼의 크기가 
	 * 모두 디스크 섹터 크기의 배수로 정렬되어 있어야 한다. 따라서 메모리는 
	 * posix_memalign을 사용해서 섹터 크기로 정렬되도록 해야 한다.
	 */
	rv = posix_memalign(&buf, SECTOR_SIZE, BUF_SIZE);
	if (rv) {
		fprintf(stderr, "Failed to allocate memory: %s\n", strerror(rv));
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY | O_DIRECT);
	if (fd == -1) {
		fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}
	for (i = DATA_SIZE; i > 0; i -= BUF_SIZE) {
		if (pread(fd, buf, BUF_SIZE, 0) == -1) {
			fprintf(stderr, "Failed to write %s: %s\n", argv[1], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	printf("O_DIRECT..\n");

	free(buf);
	return 0;
}
