#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
}				t_zone;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float raduis;
	char color;
}				t_shape;

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int clear_all(FILE *file, char *drawing)
{
	fclose(file);
	if (drawing)
		free(drawing);
	return (1);
}
int str_err(char *s, int ret)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
	return (ret);
}

char *fill_drawing(t_zone *zone)
{
	int i = 0;
	char *drawing = (char *)malloc(zone->height * zone->width);
	while (i < zone->height * zone->width)
	{
		drawing[i] = zone->background;
		i++;
	}
	return (drawing);
}

int check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);
}
int check_shape(t_shape *shape)
{
	return (shape->raduis > 0 && (shape->type == 'C' || shape->type == 'c'));
}

int get_zone(FILE *file, t_zone *zone)
{
	int ret = 0;
	ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
	if (ret != 3)
		return(0);
	if (!check_zone(zone))
		return (0);
	return (1);
}

int in_circle(int x, int y, t_shape *shape)
{
	float distance;

	distance = sqrtf(powf(x- shape->x, 2) + powf(y - shape->y, 2));
	if (distance <= shape->raduis)
	{
		if (shape->raduis - distance < 1.00000)
			return (2);
		return (1);
	}
	return (0);
}
void draw_shape(char *drawing, t_zone *zone, t_shape *shape)
{
	int ret = 0;
	int i = 0;
	int j = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			ret = in_circle(j, i, shape);
			if ((shape->type == 'c' &&  ret == 2) || (shape->type == 'C' && ret))
				drawing[i * zone->width + j] = shape->color;
			j++;
		}
		i++;
	}
}

int draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
	int ret = 0;
	t_shape tmp;
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.raduis, &tmp.color)) == 5)
	{
		if (!check_shape(&tmp))
			return (0);
		draw_shape(drawing, zone, &tmp);
	}
	if (ret != -1)
		return (0);
	return (1);
}

void draw_drawing(char *drawing, t_zone *zone)
{
	int i = 0;
	while (i < zone->height)
	{
		write(1, drawing + i * zone->width, zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_zone zone;
	FILE *file;
	char *drawing = NULL;

	if (argc != 2)
		return (str_err("Error: argument", 1));
	if (!(file = fopen(argv[1], "r")))
		return (str_err("Error: Operation file corrupted", 1));
	if (!get_zone(file, &zone))
		return (clear_all(file, drawing) && str_err("Error: Operation file corrupted", 1));
	drawing = fill_drawing(&zone);
	if (!draw_shapes(file, &zone, drawing))
		return (clear_all(file, drawing) && str_err("Error: Operation file corrupted", 1));
	draw_drawing(drawing, &zone);
	clear_all(file, drawing);
	return (0);
}