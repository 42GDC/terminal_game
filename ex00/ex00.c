#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define MAP_WIDTH 15
#define MAP_HEIGHT 8

typedef struct s_map
{
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

void print_map(t_map *map)
{
	//add code
}

void read_map(char *filename, t_map *map)
{
	// if (file_check(filename))
	// 	perror("Fail: ");

	int fd = open(filename, O_RDONLY);
	
	if (fd == -1)
		perror("Fail: ");

	size_t size = (MAP_WIDTH + 1) * MAP_WIDTH;
	size_t bytes = read(fd, (char *)map->m, size);
	
	write(1, &map->m, bytes);
	close(fd);
}

int main(int ac, char *av[])
{
	if (ac == 2)
	{
		t_map map;
		read_map(av[1], &map);
		print_map(&map);
	}
	return 0;
}
