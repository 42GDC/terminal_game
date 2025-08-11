#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAP_WIDTH 15
#define MAP_HEIGHT 8

typedef struct s_map
{
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

void print_map(t_map *map)
{
	for(int i = 0; i < MAP_HEIGHT; i++)
	{
		write(1, &map->m[i], MAP_WIDTH );
		write(1, "\n", 1);
	}	
}

void read_map(char *filename, t_map *map)
{
	// if (file_check(filename))
	// 	perror("Fail: ");

	int fd = open(filename, O_RDONLY);
	
	if (fd == -1)
		perror("Fail: ");

	char buf[MAP_HEIGHT * MAP_WIDTH + MAP_HEIGHT];
	size_t bytes = read(fd, buf, sizeof(buf));

	for (int i = 0; i < MAP_HEIGHT; i++)
		memcpy(map->m[i], &buf[i * (MAP_WIDTH + 1)], MAP_WIDTH);
	
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
