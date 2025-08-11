#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>

#define MAP_WIDTH 15
#define MAP_HEIGHT 8

#define CH_PLAYER 'P'
#define CH_WALL '1'
#define CH_EMPTY '0'
#define CH_COIN 'C'
#define CH_EXIT 'E'

typedef struct s_map
{
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

static int get_character_count(t_map *map, char c)
{
	int	i;
	int c_count;

	c_count = 0;
	for (i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++)
		c_count += (map->m[i / MAP_WIDTH][i % MAP_WIDTH]) == c;
	return c_count;
}

static int is_character_valid(char c)
{
	return (c == CH_COIN ||
			c == CH_EMPTY ||
			c == CH_EXIT ||
			c == CH_PLAYER ||
			c == CH_WALL);
}

static int is_map_characters_valid(t_map *map)
{
	int	i;
	for (i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++)
	{
		if (!is_character_valid(map->m[i / MAP_WIDTH][i % MAP_WIDTH]))
			return 0;
	}
	return 1;
}

static int is_border_valid(t_map *map)
{
	int	i;
	for (i = 0; i < MAP_WIDTH || i < MAP_HEIGHT; i++)
	{
		if ((i < MAP_WIDTH &&
				(map->m[0][i] != CH_WALL || map->m[MAP_HEIGHT - 1][i] != CH_WALL) &&
				!(map->m[0][i] == CH_EMPTY && map->m[MAP_HEIGHT - 1][i] == CH_EMPTY)) ||
			(i < MAP_HEIGHT &&
				(map->m[i][0] != CH_WALL || map->m[i][MAP_WIDTH - 1] != CH_WALL) &&
				!(map->m[i][0] == CH_EMPTY || map->m[i][MAP_WIDTH - 1] == CH_EMPTY)))
			return 0;
	}
	return 1;
}

static int is_player_valid(t_map *map)
{
	return get_character_count(map, CH_PLAYER) == 1;
}

static int is_entrance_valid(t_map *map)
{
	return get_character_count(map, CH_EXIT) == 1;
}

static int validate_map(t_map *map)
{
	return is_map_characters_valid(map) &&
		is_border_valid(map) &&
		is_entrance_valid(map) &&
		is_player_valid(map);
}

void print_map(t_map *map)
{
	int		i;
	int		j;

	if (validate_map(map))
		for (i = 0; i < MAP_HEIGHT; i++)
		{
			write(0, &(map->m[i][0]), MAP_WIDTH);
			write(0, "\n", 1);
		}
	else 
		write(0, "Error: Map is not valid\n", 23);
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
