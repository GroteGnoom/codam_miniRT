#include "miniRT.h"
#include <math.h>

#define MAX_BOUNCES 1

t_ray	new_ray(t_ray r, t_vec3f norm_dir, float hit_min)
{
	r.origin = at(r, hit_min);
	r.dir = f_reflection(r.dir, norm_dir);
	return (r);
}

t_vec3f	loop_lights(t_scene *scene, t_vec3f norm_dir, t_ray new_r, \
	t_ray old_r, t_vec3f object_color)
{
	t_vec3f	spot_color;
	t_light	*lights;
	int		i;

	i = 0;
	lights = scene->lights.data;
	spot_color = vec3f_init(0, 0, 0);
	while (i < scene->lights.len)
	{
		spot_color = vec3f_add(spot_color, spot_light(new_r, norm_dir, lights[i], old_r, scene));
		if (BONUS)
			spot_color = vec3f_add(spot_color, \
				spot_light_specular(norm_dir, lights[i], new_r, old_r, scene));
		i++;
	}
	spot_color = vec3f_mul_v(spot_color, object_color);
	return (spot_color);
}

t_vec3f	spot_and_ambient(t_ray new_r, t_vec3f object_color, \
	t_scene *scene, t_vec3f norm_dir, t_ray old_r)
{
	t_vec3f	spot_color;
	t_vec3f	ambient_color;

	spot_color = loop_lights(scene, norm_dir, new_r, old_r, object_color);
	if (scene->ambient && scene->ambient->brightness)
	{
		ambient_color = vec3f_mul(scene->ambient->color, \
				scene->ambient->brightness);
		ambient_color = vec3f_mul_v(ambient_color, object_color);
	}
	else
		ambient_color = vec3f_init(0, 0, 0);
	return (vec3f_add(spot_color, ambient_color));
}

void	get_hit(t_hits *hit, t_scene *scene, t_ray r)
{
	hit->hit_min = 0.;
	hit->hit_side_cylinder = 0;
	sphere_loop(r, scene, hit);
	plane_loop(r, scene, hit);
	cylinder_loop(r, scene, hit);
	if (BONUS)
		paraboloid_loop(r, scene, hit);
}

t_vec3f	ray_color(t_ray r, t_scene *scene)
{
	t_vec3f	norm_dir;
	t_vec3f	object_color;
	t_hits	hit;
	t_ray	new_r;

	get_hit(&hit, scene, r);
	if (hit.hit_min > 0)
	{
		if (hit.hit_type == SPHERE)
			object_color = get_sphere_norm_color(hit, r, scene->spheres.data,
					&norm_dir);
		else if (hit.hit_type == PLANE)
			object_color = get_plane_norm_color(hit, r, scene->planes.data,
					&norm_dir);
		else if (BONUS && hit.hit_type == PARABOLOID)
			object_color = get_paraboloid_norm_color(hit, r, \
				scene->paraboloids.data, &norm_dir);
		else
			object_color = get_cylinder_norm_color(hit, r,
					scene->cylinders.data, &norm_dir);
		new_r = new_ray(r, norm_dir, hit.hit_min);
		return (spot_and_ambient(new_r, object_color, scene, norm_dir, r));
	}
	return (vec3f_init(0, 0, 0));
}
