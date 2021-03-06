/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dnoom <marvin@codam.nl>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 11:25:34 by dnoom         #+#    #+#                 */
/*   Updated: 2022/04/06 11:25:34 by dnoom         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <math.h>

t_ray	new_ray(t_ray r, t_vec3f norm_dir, float hit_min)
{
	r.origin = at(r, hit_min - 0.0001);
	r.dir = f_reflection(r.dir, norm_dir);
	r.bounces++;
	return (r);
}

t_vec3f	loop_lights(t_scene *scene, t_vec3f norm_dir, t_old_new_ray rays,
		t_vec3f object_color)
{
	t_vec3f	spot_color;
	t_light	*lights;
	int		i;

	i = 0;
	lights = scene->lights.data;
	spot_color = vec3f_init(0, 0, 0);
	while (i < scene->lights.len)
	{
		spot_color = vec3f_add(spot_color, spot_light(rays, norm_dir, lights[i],
					scene));
		if (BONUS)
			spot_color = vec3f_add(spot_color, \
				spot_light_specular(norm_dir, lights[i], rays, scene));
		i++;
	}
	spot_color = vec3f_mul_v(spot_color, object_color);
	return (spot_color);
}

t_vec3f	spot_and_ambient(t_old_new_ray rays, t_vec3f object_color, \
	t_scene *scene, t_vec3f norm_dir)
{
	t_vec3f	spot_color;
	t_vec3f	ambient_color;

	spot_color = loop_lights(scene, norm_dir, rays, object_color);
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
	objects_loop(r, scene, hit, 0);
}

t_vec3f	ray_color(t_ray r, t_scene *scene)
{
	t_vec3f			norm_dir;
	t_vec3f			object_color;
	t_hits			hit;
	t_old_new_ray	rays;

	get_hit(&hit, scene, r);
	if (hit.hit_min > 0)
	{
		norm_dir = (((t_object *)(scene->objects.data)) \
			[hit.object_index]).base.get_normal(hit, r, scene);
		object_color = (((t_object *)(scene->objects.data)) \
			[hit.object_index]).base.get_color(hit, r, norm_dir, scene);
		rays.n = new_ray(r, norm_dir, hit.hit_min);
		rays.o = r;
		rays.hit_dist = hit.hit_min;
		if (hit.material == MIRROR)
			return (object_color);
		return (spot_and_ambient(rays, object_color, scene, norm_dir));
	}
	return (vec3f_init(0, 0, 0));
}
