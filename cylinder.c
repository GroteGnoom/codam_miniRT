#include "miniRT.h"
#include <math.h>

t_angle	get_angle(t_cylinder cylinder)
{
	t_angle	s_angle;
	t_vec3f	rot_plane;
	float	len_rot_plane;

	rot_plane = vec3f_cross(cylinder.dir, vec3f_init(0, 1, 0));
	len_rot_plane = vec3f_len(rot_plane);
	s_angle.angle = asin(len_rot_plane);
	s_angle.k = vec3f_unit(rot_plane);
	return (s_angle);
}

t_ray	rotate_ray(t_ray r, t_cylinder cylinder)
{
	t_vec3f	rotated_dir;
	t_vec3f	rotated_origin;
	t_vec3f	rotated_cylinder;
	t_ray	rot_ray;
	t_angle	angle;

	if (cylinder.dir.x == 0 && cylinder.dir.z == 0)
	{
		rotated_origin = r.origin;
		rotated_cylinder = cylinder.pos;
		rotated_dir = r.dir;
	}
	else
	{
		angle = get_angle(cylinder);
		rotated_dir = ft_rodrigues(r.dir, angle.k, angle.angle);
		rotated_origin = ft_rodrigues(r.origin, angle.k, angle.angle);
		rotated_cylinder = ft_rodrigues(cylinder.pos, angle.k, angle.angle);
	}
	rotated_origin = vec3f_sub(rotated_origin, rotated_cylinder);
	rot_ray.dir = rotated_dir;
	rot_ray.origin = rotated_origin;
	return (rot_ray);
}

float	hit_infinite_cylinder(t_ray r, t_cylinder cylinder)
{
	float	a;
	float	b;
	float	c;
	int		solved;
	float	t;

	if (r.dir.x == 0 && r.dir.z == 0)
		return (-1.0);
	r.dir.y = 0;
	r.origin.y = 0;
	a = vec3f_len_sq(r.dir);
	b = 2.0 * (vec3f_dot(r.origin, r.dir));
	c = vec3f_len_sq(r.origin) - (cylinder.radius * cylinder.radius);
	t = abc(a, b, c, &solved);
	if (!solved)
		return (-1.0);
	return (t);
}

float	hit_cylinder(t_cylinder cylinder, t_ray r, int *hit_side)
{
	float	t_plane;
	t_vec3f	p2;
	t_ray	rot_ray;
	float	t;

	*hit_side = 0;
	rot_ray = rotate_ray(r, cylinder);
	if (!ray_in_right_dir(rot_ray, cylinder))
		return (-1.0);
	t_plane = hit_top_or_bottom(rot_ray, cylinder);
	t = hit_infinite_cylinder(rot_ray, cylinder);
	if (t < 0 && t_plane < 0)
		return (-1.0);
	p2 = at(rot_ray, t);
	if (fabsf(p2.y) < cylinder.height / 2)
	{
		if (t_plane < 0 || t < t_plane)
		{
			*hit_side = 1;
			return (t);
		}
	}
	if (t_plane > 0)
		return (t_plane);
	return (-1.0);
}
