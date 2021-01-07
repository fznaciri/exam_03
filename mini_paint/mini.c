/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 01:14:38 by mac               #+#    #+#             */
/*   Updated: 2021/01/07 10:25:07 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_area
{
    int w;
    int h;
    char c;
    char *area;
} t_area;

typedef struct s_circle
{
    char type;
    float x;
    float y;
    float r;
    char c;
}t_circle;

int ft_strlen(char *s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return i;
}

int ft_str_error(char *s)
{
    if (s)
        write(1, s, ft_strlen(s));
    return 1;
}

char *get_zone(FILE *file, t_area *area)
{
    int i;

    i = 0;
    if (fscanf(file, "%d %d %c\n", &(area->w), &(area->h), &(area->c)) != 3)
        return NULL;
    if (area->w <= 0 || area->w > 300 || area->h <= 0 || area->h > 300)
        return NULL;
    if (!(area->area = (char*)malloc(area->w * area->h)))
        return NULL;
    while (i < area->w * area->h)
        area->area[i++] = area->c;
    return area->area;
}

int in_circle(float x, float y, t_circle c)
{
	float	distance;

	distance = sqrtf(powf(x - c.x, 2.) + powf(y - c.y, 2.));
	if (distance <= c.r)
	{
		if ((c.r - distance) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

int fill_area(FILE *file, t_area *area)
{
    int i;
    int j;
    int r;
    t_circle c;
    
    i = 0;
    
    while (fscanf(file, "%c %f %f %f %c\n", &(c.type), &(c.x), &(c.y), &(c.r), &(c.c)) == 5)
    {
        printf("%c %f %f %f %c\n", c.type, c.x, c.y, c.r, c.c);
        if (c.r <= 0.0 || (c.type != 'c' && c.type != 'C'))
            return 0;
        i = 0;
	    while (i < area->h)
	    {
		    j = 0;
		    while (j < area->w)
		    {
			    r = in_circle((float)j, (float)i, c);
			    if ((c.type == 'c' && r == 2) || (c.type == 'C' && r))
				    area->area[(i * area->w) + j] = c.c;
			    j++;
		    }
		    i++;
        }
    }
    return 1;
}

void draw(t_area *area)
{
    int i;
    int j;

    i = 0;
    while (i < area->h)
    {
        j = 0;
        while (j < area->w)
        {
            write(1, area->area + (i * area->w + j), 1);
            j++;
        }    
        write(1, "\n", 1);
        i++;
    }
}

void clear(FILE *file, t_area *area)
{
    if (file)
        fclose(file);
    if (area->area)
        free(area->area);   
}

int main(int ac, char **av)
{
    FILE *file;
    t_area area;
    char *tmp;

    if (ac != 2)
        ft_str_error("Error: argument\n");
    if (!(file = fopen(av[1], "r")))
        ft_str_error("Error: Operation file corrupted\n");
    if (!(tmp = get_zone(file, &area)))
    {
        clear(file, &area);
        ft_str_error("Error: Operation file corrupted\n");
    }    
    if (!(fill_area(file, &area)))
    {
        clear(file, &area);
        ft_str_error("Error: Operation file corrupted\n");
    }
    draw(&area);
    clear(file, &area);
    return 0;
}