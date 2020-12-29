/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnaciri- <fnaciri-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:06:06 by fnaciri-          #+#    #+#             */
/*   Updated: 2020/12/29 10:51:34 by fnaciri-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_paint_area
{
    int     w;
    int     h;
    char    c;
    char    *area;
} t_paint_area;

typedef struct s_rec
{
    char    type;
    float     x;
    float     y;
    float     w;
    float     h;
    char    c;
}            t_rec;

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

int    get_paint_area(FILE *file, t_paint_area *p_area)
{
    int r;
    
    if((r = fscanf(file, "%d %d %c\n", &(p_area->w), &(p_area->h), &(p_area->c))) != 3)
        return 1;
    if (p_area->w < 0 || p_area->w > 300 || p_area->h < 0 || p_area->h > 300)
        return 1;
    return 0;
}

char    *fill_area(t_paint_area *p_area)
{
    int i;

    i = 0;
    if(!(p_area->area = (char *)malloc(sizeof(char) * (p_area->w * p_area->h))))
        return NULL;
    while(i < p_area->w * p_area->h)
    {
        p_area->area[i] = p_area->c;
        i++;
    }
    return p_area->area;
}

// int  in_rec(float x, float y, t_rec rec)
// {
//     if (x < rec.x || y < rec.y || x > rec.x + rec.w || y > rec.y + rec.h)
//     {
//         return 0;
//     }
//     //if ((x - 1 > rec.x  && x - 1 < rec.x + rec.w ) && (y  - 1> rec.y && y - 1< rec.y + rec.h ))
//     if (((x - rec.x < 1) || ((rec.x + rec.w) - x < 1)) || ((y - rec.y < 1 || ((rec.y + rec.h) - y < 1))))
//         return 2;
//     return 1;
// }

int     in_rec(float x, float y, t_rec rec)
{
    if (((x < rec.x || (rec.x + rec.w < x)) || (y < rec.y)) || (rec.y + rec.h < y))
        return (0);
    if (((x - rec.x < 1.00000000) || ((rec.x+ rec.w) - x < 1.00000000)) ||
        ((y - rec.y < 1.00000000 || ((rec.y + rec.h) - y < 1.00000000))))
        return (2);
    return 1;
}

int fill_rec(FILE *file, t_paint_area *p_area)
{
    int i;
    int j;
    int r;
    int n;
    t_rec rec;
    
    while ((n = fscanf(file, "%c %f %f %f %f %c\n", &(rec.type), &(rec.x), &(rec.y), &(rec.w), &(rec.h), &(rec.c))) == 6)
    {
        printf("rec: %c x: %f y:%f w: %f h: %f\n", rec.type, rec.x, rec.y, rec.w, rec.h);
        if (rec.h <= 0 || rec.w <= 0 || (rec.type != 'r' && rec.type != 'R'))
            return 1;
        j = 0;
        while (j < p_area->h)
        {
            i = 0;
            while (i < p_area->w)
            {
                r = in_rec(i, j, rec);
                if (rec.type == 'R' && r == 1)
                    p_area->area[i * p_area->w + j] = rec.c;
                else if (rec.type == 'r' && r == 2)
                    p_area->area[i * p_area->w + j] = rec.c;
                i++;
            }
            j++;    
        }  
    } 
    return 0;  
}

void    draw(t_paint_area *p_area)
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
    t_paint_area p_area;
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
    get_paint_area(file, &p_area);
    area = fill_area(&p_area);
    if (!fill_rec(file, &p_area))
        draw(&p_area);
    return 0;
}