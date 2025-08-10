//add includes

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
	//add code
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
