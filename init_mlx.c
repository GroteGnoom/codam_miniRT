#include "mlx/mlx.h"
#include "vec/vec.h"
#include <stdlib.h>
#include <math.h>
#include "miniRT.h"

#define WIN_WIDTH 300
#define ASPECT_RATIO (16. / 9.)
#define WIN_HEIGHT (WIN_WIDTH / ASPECT_RATIO)
#define DESTROY_NOTIFY 17

void	init_image(t_info *info)
{
	int	bits_per_pixel;
	int	line_length;
	int	endian;

	info->img.data = (int *)mlx_get_data_addr(info->mlximg_ptr, &bits_per_pixel,
			&line_length, &endian);
	info->img.size_x = line_length / sizeof(int);
}

int	handle_key(int keycode, void *param)
{
	(void)param;
	if (keycode == 53)
		exit(0);
	return (0);
}

int	handle_destroy(void *param)
{
	(void)param;
	exit(0);
}

int	draw_to_window(t_info *info)
{
	mlx_put_image_to_window(info->mlx_ptr, info->win_ptr, info->mlximg_ptr,
		0, 0);
	return (0);
}

void	pixel_put_image(t_arr2di *image, int x, int y, int color)
{
	*(unsigned int *)(image->data + x + y * image->size_x)
		= color;
}

t_vec3f	get_ray_direction(t_info *info, int i, int j, t_vec3f camera)
{
	float	u;
	float	v;
	t_vec3f	hor_offset;
	t_vec3f	ver_offset;
	t_vec3f	direction;

	u = (float)i / (WIN_WIDTH - 1);
	v = (float)j / (WIN_HEIGHT - 1);
	hor_offset = vec3f_mul(info->horizontal, u);
	ver_offset = vec3f_mul(info->vertical, v);
	direction = vec3f_add(info->lower_left_corner, hor_offset);
	direction = vec3f_add(direction, ver_offset);
	direction = vec3f_sub(direction, camera);
	return (direction);
}

void	paint_img(t_info *info, t_vec3f camera, t_scene *scene)
{
	int		i;
	int		j;
	t_vec3f	direction;
	t_ray	r;
	int		color;

	j = WIN_HEIGHT - 1;
	while (j >= 0)
	{
		i = 0;
		while (i < WIN_WIDTH)
		{
			direction = get_ray_direction(info, i, j, camera);
			r.origin = camera;
			r.dir = direction;
			color = ray_color(r, *(t_sphere *)scene->spheres.data);
			pixel_put_image(&info->img, i, j, color);
			i++;
		}
		j--;
	}
}

void init_mlx(t_scene *scene)
{
	t_info		info;
	t_vec3f		camera;
	t_vec3f		left_edge;
	t_vec3f		lower_left_corner1;

	info.mlx_ptr = mlx_init();
	info.win_ptr = mlx_new_window(info.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, \
	"miniRT");
	info.mlximg_ptr = mlx_new_image(info.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	init_image(&info);
	info.viewport_height = 2.0;
	info.viewport_width = ASPECT_RATIO * info.viewport_height;
	info.focal_length = 1;
	camera = vec3f_init(0, 0, 0);
	info.horizontal = vec3f_init(info.viewport_width, 0, 0);
	info.vertical = vec3f_init(0, info.viewport_height, 0);
	left_edge = vec3f_sub(camera, vec3f_div(info.horizontal, 2));
	lower_left_corner1 = vec3f_sub(left_edge, vec3f_div(info.vertical, 2));
	info.lower_left_corner = vec3f_sub(lower_left_corner1, \
	vec3f_init(0, 0, info.focal_length));
	paint_img(&info, camera, scene);
	mlx_hook(info.win_ptr, DESTROY_NOTIFY, 0, handle_destroy, NULL);
	mlx_expose_hook(info.win_ptr, draw_to_window, &info);
	mlx_key_hook(info.win_ptr, handle_key, &info);
	mlx_loop(info.mlx_ptr);
}