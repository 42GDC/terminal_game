#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct s_arena
{
	void	*mem;
	size_t	size;
	size_t	offset;
} t_arena;

typedef struct s_map
{
	char		**m;
	size_t		w;
	size_t		h;
	t_arena		arena;
} t_map;


void	free_map(t_map *map)
{
	if (map && map->arena.mem)
	{
		free(map->arena.mem);
		map->arena = (t_arena){0};
		map->m = NULL;
	}
}

/* Prints error message, frees map and closes fd if needed */
void	err_exit(const char *s, t_map *map, int fd)
{
	if (fd > 0) close(fd);
	if (map) free_map(map);
	write(2, s, strlen(s));
	write(2, "\n", 1);
	exit(1);
}

/* One malloc needed */
t_arena	arena_init(size_t size)
{
	t_arena a = {malloc(size), size, 0};
	if (!a.mem)
		err_exit("alloc fail", NULL, 0);
	return (a);
}

void	*alloc_arena(t_arena *a, size_t size)
{
	if (a->offset + size > a->size)
		err_exit("Arena out of mem", NULL, 0);
	void *ptr = a->mem + a->offset;
	a->offset += size;
	return (ptr);
}

int		is_wall(char c)
{
    return (c == '1');
}

void	print_map(t_map *map)
{
	for(size_t i = 0; i < map->h; i++)
	{
		write(1, map->m[i], map->w);
		write(1, "\n", 1);
	}	
}

/* Check top/bottom/left/right are enclosed */
int		validate_walls(t_map *map)
{
	for (size_t i = 0; i < map->w; i++)
	{
		if (!is_wall(map->m[0][i]) || !is_wall(map->m[map->h - 1][i]))
			return (err_exit("Bad horizontal walls", map, 0), 0);
	}
    
    for (size_t j = 0; j < map->h; j++)
		if (!is_wall(map->m[j][0]) || !is_wall(map->m[j][map->w - 1]))
			return (err_exit("Bad vertical walls", map, 0), 0);
    
    return (1);
}

/* Alloc and copy into map */
void	fill_map(const char *buf, t_map *map)
{
	map->m = alloc_arena(&map->arena, map->h * sizeof(char *));
	for (size_t i = 0; i < map->h; i++)
	{
		map->m[i] = alloc_arena(&map->arena, map->w + 1);
		memcpy(map->m[i], &buf[i * (map->w + 1)], map->w);
		map->m[i][map->w] = '\0';
	}
	return ;
}

/* Get size of map, making sure it is rectangular */
void	map_check(const char *buf, t_map *map)
{
	const char *line = strchr(buf, '\n');
	if (!line)
		return (err_exit("No newlines", map, 0));
	
	map->w = line - buf;
	map->h = 0;
	
	line = buf;
	while ((line = strchr(line, '\n')) != NULL)
	{
		map->h++;
		line++;
	}
	printf("w: %lu h: %lu\n", map->w, map->h);

	line = buf;
	for (size_t i = 0; i < map->h; i++)
	{
		const char *next_line = strchr(line, '\n');

		size_t line_len;
		if (next_line)
			line_len = next_line - line;
		else
			line_len = strlen(line);

		if (line_len != map->w)
			return (err_exit("Inconsistent line lengths", map, 0));
		if (next_line)
			line = next_line + 1;
	}
	fill_map(buf, map);
}

int	ends_with_txt(const char *str, size_t n)
{
	size_t	len = strlen(str);

	if (len < n)
		return (0);	
	return (strcmp(str + (len - n), ".txt") == 0);
}

/* Check filename and ability to open */
int	file_check(char *filename)
{
	int fd;

	if (!ends_with_txt(filename, 4))
		return (err_exit("Wrong filetype", NULL, 0), 0);

	fd = open(filename, O_RDONLY);
	
	printf("opened %d\n", fd);
	if (fd < 0)
		return (err_exit("Can't open", NULL, fd), 0);
	
	return fd;
}

/* Half arena is for file buf, other half is for program mem */
void	read_map(char *filename, t_map *map)
{
	int		fd = file_check(filename);
	
	size_t	buf_size = map->arena.size / 2;
	char	*buf = alloc_arena(&map->arena, map->arena.size / 2);
	size_t	bytes = read(fd, buf, buf_size);
	if (bytes <= 0)
		return (err_exit("Can't read file", map, fd));
	
	buf[bytes] = '\0';
	map_check(buf, map);
	validate_walls(map);
	
	close(fd);
}

int	main(int ac, char *av[])
{
	if (ac == 2)
	{
		t_map map = {0};
		map.arena = arena_init(1 << 16); //64kb

		read_map(av[1], &map);
		print_map(&map);
		free_map(&map);
	}
	return 0;
}
