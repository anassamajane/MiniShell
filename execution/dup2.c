#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>



int	main()
{
	int	fd;

	fd = open("lala.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("working");
}
