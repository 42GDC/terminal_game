#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>

#define MAP_WIDTH 15
#define MAP_HEIGHT 8

typedef struct s_map
{
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

void print_map(t_map *map)
{
	int		i;
	int		j;

	for (i = 0; i < MAP_HEIGHT; i++)
	{
		write(0, &(map->m[i][0]), MAP_WIDTH);
		write(0, "\n", 1);
	}
}

void read_map(char *filename, t_map *map)
{
	size_t	fd;
	int		i;
	int		j;

	fd = open(filename, O_RDONLY);
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		if (read(fd, &(map->m[i][0]), MAP_WIDTH) < MAP_WIDTH)
		{
			map->m[0][0] = '\0';
			return ;
		}
		lseek(fd, 1, SEEK_CUR);
	}
	close(fd);
}

int main(int ac, char *av[])
{
	t_map map;

	if (ac == 2)
	{
		read_map(av[1], &map);
		if (map.m[0][0] == '\0')
			return -1;
		print_map(&map);
	}
	return 0;
}
