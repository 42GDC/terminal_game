#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILESIZE 2048
#define WALL "1"
#define SPACE "0"
//testing, want to remove
#define MAP_WIDTH 15
#define MAP_HEIGHT 8

typedef struct s_map
{
	char	**m;
	size_t		w;
	size_t		h;
} t_map;

int is_wall(char c)
{
    return (c == '1');
}

void print_map(t_map *map)
{
	if (map)
	{
		for(size_t i = 0; i < map->h; i++)
		{
			write(1, map->m[i], map->w);
			write(1, "\n", 1);
		}	
	}
}

int ends_with_txt(const char *str, size_t n)
{
	size_t	len = strlen(str);

	printf("%s %d\n", str + (len - n), strcmp(str + (len - n), ".txt"));
	if (len < n)
		return (0);	
	return (strcmp(str + (len - n), ".txt") == 0);
}

int file_check(char *filename)
{
	int fd;

	if (!ends_with_txt(filename, 4))
		return (perror("Wrong filetype"), 0);

	fd = open(filename, O_RDONLY);
	
	printf("opened %d\n", fd);
	if (fd < 0)
	{
		close(fd);
		return (perror("Can't open"), 0);
	}
	
	return fd;
}

int validate_walls(t_map *map)
{   
	if (strspn(map->m[0], "1") != map->w || strspn(map->m[map->h - 1], "1") != map->w)
		return (perror("Bad horizontal walls"), 0);
    
    for (size_t j = 0; j < map->h; j++)
    {
        if (!is_wall(map->m[j][0]) || !is_wall(map->m[j][map->w - 1]))
            return (perror("Bad vertical walls"), 0);
    }
    
    return (1);
}

void map_check(const char *buf, t_map *map)
{
	const char *line = strchr(buf, '\n');
	if (!line)
		return (perror("No newlines"));

	size_t width = line - buf;
	size_t height = 0;
	
	line = buf;
	while ((line = strchr(line, '\n')) != NULL)
	{
		height++;
		line++;
	}
	map->w = width;
	map->h = height;
	printf("w: %lu h: %lu\n", map->w, map->h);
	map->m = malloc(height * sizeof(char *));
	if (!map->m)
		return ;
	for (size_t i = 0; i < height; i++)
	{
		map->m[i] = malloc(width * sizeof(char) + 1);
		if (!map->m[i])
		{
			while (i-- >= 0)
				free(map->m[i]);
			free(map->m);
			return;
		}
		memcpy(map->m[i], &buf[i * (width + 1)], width);
		map->m[i][width] = '\0';
	}
	return ;
}

void read_map(char *filename, t_map *map)
{
	int		fd = file_check(filename);
	char	buf[FILESIZE];
	
	if (fd <= 0)
		return (perror("Fail"));

	size_t bytes = read(fd, buf, sizeof(buf));
	if (bytes <= 0)
	{
		close(fd);
		return (perror("Can't read file"));
	}
	buf[bytes] = '\0';
	map_check(buf, map);
	if (map->m && !validate_walls(map))
    	return (perror("Wall validation failed"));

	close(fd);
}

int main(int ac, char *av[])
{
	if (ac == 2)
	{
		t_map map = {0};
		read_map(av[1], &map);
		print_map(&map);
	}
	return 0;
}
