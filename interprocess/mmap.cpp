#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	int fd = 0;

	fd = open("aaa", O_CREAT | O_TRUNC | O_RDWR, 0600);
	write(fd, "version", 8);
	close(fd);

	fd = open("aaa", O_RDWR);
	char buf[100] = {0};
	char *ptr = (char*)mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	int h = lseek(fd, 0, SEEK_CUR);
	cout << h << endl;
	
	h = lseek(fd, 0, SEEK_SET);
	cout << h << endl;

	int k = read(fd, buf, 8);
	cout << k << endl;

	if(strcmp(buf, ptr)) {
		cout << "buf: " << buf << endl;
		cout << "ptr: " << ptr << endl;
		abort();
	}

	cout << ptr << endl;
	munmap(ptr, 8);
	close(fd);

	return 0;
}
