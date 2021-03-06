/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sphere.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dnoom <marvin@codam.nl>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 11:25:34 by dnoom         #+#    #+#                 */
/*   Updated: 2022/04/06 11:25:34 by dnoom         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <math.h>

void	get_sphere_angles(t_vec3f normal, float *x_angle, float *y_angle)
{
	*x_angle = vec3f_dot(vec3f_unit(vec3f_init(normal.x, 0, normal.z)), \
		vec3f_init(1, 0, 0));
	*x_angle = acos(*x_angle);
	if (normal.z > 0)
		*x_angle = *x_angle / M_PI;
	else
		*x_angle = 2 - *x_angle / M_PI;
	*x_angle = 2 - *x_angle;
	*y_angle = vec3f_dot(normal, vec3f_init(0, 1, 0));
	*y_angle = acos(*y_angle);
	*y_angle = 1 - *y_angle / M_PI;
}

t_vec3f	get_normal_sphere(t_vec3f hit_point, t_vec3f sphere_center)
{
	return (vec3f_unit(vec3f_sub(hit_point, sphere_center)));
}

float	hit_sphere(t_sphere sphere, t_ray r)
{
	float	a;
	float	b;
	float	c;
	float	solution;
	int		solved;

	if (vec3f_dist(r.origin, sphere.base.pos) < sphere.radius)
		return (-1.0);
	a = vec3f_len_sq(r.dir);
	b = 2.0 * vec3f_dot(vec3f_sub(r.origin, sphere.base.pos), r.dir);
	c = vec3f_len_sq(vec3f_sub(r.origin, sphere.base.pos)) - \
		(sphere.radius * sphere.radius);
	solution = abc(a, b, c, &solved);
	if (!solved)
		return (-1.0);
	return (solution);
}

/*
t_vec3f	random_in_sphere(void)
{
	t_vec3f	random_point;
	float	x;
	float	y;
	float	z;

	while (1)
	{
		x = (float)rand() / (float)RAND_MAX * 2 - 1;
		y = (float)rand() / (float)RAND_MAX * 2 - 1;
		z = (float)rand() / (float)RAND_MAX * 2 - 1;
		random_point = vec3f_init(x, y, z);
		if (vec3f_len_sq(random_point) <= 1)
			break ;
	}
	return (vec3f_unit(random_point));
}
*/
