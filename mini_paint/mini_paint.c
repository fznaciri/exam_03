/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnaciri- <fnaciri-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 12:12:47 by fnaciri-          #+#    #+#             */
/*   Updated: 2020/12/31 17:23:47 by fnaciri-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_paint
{
    int w;
    int h;
    char c;
    char *area;
} t_paint;

typedef struct s_cercle
{
    char    type;
    float   x;
    float   y;
    float   r;
    char    c;
}t_cercle;


int ft_strlen(char *s)
{
    int i = 0;
    while(s[i])
        i++;
    return (i);
}

void    ft_putstr(char *s)
{
    if (s)
        write(1, s, ft_strlen(s));  
}

int get_paint(FILE *file, t_paint *area)
{
    if (fscanf(file, "%d %d %c\n", &(area->w), &(area->h), &(area->c)) != 3)
        return 1;
    if (area->w <= 0 || area->w > 300 || area->h <= 0 || area->h > 300)
        return 1;
   return 0;
}

char     *fill_background(t_paint *area)
{
    int i; 
    
    i = 0;
    if (!(area->area = (char*)malloc(area->w * area->h)))
        return NULL;
    while (i < area->w * area->h)
        area->area[i++] = area->c;
    return area->area;
}

int in_cercle(float x, float y, t_cercle c)
{
    float dist;
    
    dist = sqrtf(powf(x - c.x, 2.) + powf(y - c.y, 2.));
    if (dist > c.r)
        return 0;
    if (c.r - dist < 1)
            return 2;
    return 1;
}

int fill_area(FILE *file, t_paint *area)
{
    int i;
    int j;
    int r;
    t_cercle c;
    
    i = 0;
    while((fscanf(file, "%c %f %f %f %c\n", &(c.type), &(c.x), &(c.y), &(c.r), &(c.c))) == 5)
    {
        if ((c.type != 'c' && c.type != 'C') || c.r <= 0)
            return 1;
        while (i < area->h) 
        {
            j = 0;
            while (j < area->w)
            {
                r = in_cercle((float)j, (float)i, c);
                if ((c.type == 'c' && r == 2) || (c.type == 'C' && r))
                    area->area[i * area->w + j] = c.c;
                j++;
            }
            i++;    
        }  
    } 
    return 0;  
}

void    draw(t_paint *p_area)
{
    int i;
    int j;

    i = 0;
    while (i < p_area->h)
    {
        j = 0;
        while (j < p_area->w)
        {
            write(1, p_area->area + (i * p_area->w + j), 1);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }
}

int main(int ac, char **av)
{
    FILE *file;
    t_paint p_area;
    char *area;
    
    if(ac !=2)
    {
        ft_putstr("Error: argument\n");
        return 1;
    }
    if (!(file = fopen(av[1], "r")))
    {
        ft_putstr("Error: Operation file corrupted\n");
        return 1;
    } 
    if(get_paint(file, &p_area))
    {
        ft_putstr("Error: Operation file corrupted\n");
        return 1;
    }
    area = fill_background(&p_area);
    if (!fill_area(file, &p_area))
        draw(&p_area);
    return 0;
}