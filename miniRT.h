/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   miniRT.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dnoom <marvin@codam.nl>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 11:25:34 by dnoom         #+#    #+#                 */
/*   Updated: 2022/05/10 12:31:26 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft/libft.h"
# include "stdlib.h"
# include "vec/vec.h"
# include "fcntl.h"
# include "get_next_line/get_next_line.h"
# include <stdio.h>
# include "mlx/mlx.h"
# include <pthread.h>

# ifndef BONUS
#  define BONUS 0
# endif

# ifndef PROFILE
#  define PROFILE 0
# endif

# ifndef HEADLESS
#  define HEADLESS 0
# endif

# define MAX_TEXTURE_FILE_SIZE 50
# if HEADLESS
#  define WIN_WIDTH 10
# else
#  define WIN_WIDTH 2560
# endif
# define MAX_BOUNCES 15
# define AA 1

struct	__attribute__((packed)) s_BMPFileHeader
{
	uint16_t	file_type;
	uint32_t	file_size;
	uint16_t	reserved1;
	uint16_t	reserved2;
	uint32_t	offset_data;
};

struct	__attribute__((packed)) s_BMPInfoHeader {
	uint32_t	size;
	int32_t		width;
	int32_t		height;
	uint16_t	planes;
	uint16_t	bit_count;
	uint32_t	compression;
	uint32_t	size_image;
	int32_t		x_pixels_per_meter;
	int32_t		y_pixels_per_meter;
	uint32_t	colors_used;
	uint32_t	colors_important;
};

typedef struct s_bmp {
	unsigned char	*data;
	int				width;
	int				height;
	int				bytes_per_pixel;
	int				bytes_per_row;
}	t_bmp;

typedef enum e_scene_elem_type {
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	PARABOLOID,
	NR_ELEM_TYPES
}	t_scene_elem_type;

typedef enum e_material_type {
	NORMAL,
	CHECKER,
	MIRROR,
	NR_MATERIAL_TYPES
}	t_material_type;

typedef struct s_ambient {
	float	brightness;
	t_vec3f	color;
}	t_ambient;

typedef struct s_camera {
	t_vec3f	pos;
	t_vec3f	dir;
	float	fov;
}	t_camera;

typedef struct s_light {
	t_vec3f	pos;
	float	brightness;
	t_vec3f	color;
}	t_light;

typedef struct s_hits {
	float				hit_min;
	int					object_index;
	t_scene_elem_type	hit_type;
	int					hit_side_cylinder;
	t_material_type		material;
}	t_hits;

typedef struct s_scene {
	t_ambient	*ambient;
	t_camera	*camera;
	t_vec		lights;
	t_vec		objects;
}	t_scene;

typedef struct s_ray {
	t_vec3f	origin;
	t_vec3f	dir;
	int		bounces;
}	t_ray;

typedef t_vec3f	(*t_get_normal)(t_hits hit, t_ray r, t_scene *scene);
typedef t_vec3f	(*t_get_color)(t_hits hit, t_ray r, t_vec3f norm_dir, \
	t_scene *scene);

typedef struct s_base {
	t_scene_elem_type	type;
	t_vec3f				pos;
	t_vec3f				color;
	t_material_type		material;
	t_bmp				texture;
	t_bmp				bump_map;
	t_get_normal		get_normal;
	t_get_color			get_color;
}	t_base;

typedef struct s_directed_base {
	t_base	base;
	t_vec3f	dir;
}	t_directed_base;

typedef struct s_sphere {
	t_base	base;
	float	radius;
}	t_sphere;

typedef struct s_plane {
	t_directed_base	dir_base;
}	t_plane;

typedef struct s_cylinder {
	t_directed_base	dir_base;
	float			radius;
	float			height;
}	t_cylinder;

typedef struct s_paraboloid{
	t_directed_base	dir_base;
	float			curvature;
}	t_paraboloid;

typedef union u_object {
	t_base			base;
	t_sphere		sphere;
	t_paraboloid	paraboloid;
	t_plane			plane;
	t_cylinder		cylinder;
}	t_object;

typedef struct s_arr2di {
	int	*data;
	int	size_x;
	int	size_y;
}	t_arr2di;

typedef struct s_info {
	void			*mlx_ptr;
	void			*win_ptr;
	void			*mlximg_ptr;
	t_arr2di		img;
	t_vec3f			horizontal;
	t_vec3f			vertical;
	t_vec3f			lower_left_corner;
	float			viewport_height;
	float			viewport_width;
	float			focal_length;
	float			win_height;
}	t_info;

typedef struct s_old_new_ray {
	t_ray	n;
	t_ray	o;
	float	hit_dist;
}	t_old_new_ray;

typedef struct s_colors {
	t_vec3f	color_1;
	t_vec3f	color_2;
	t_vec3f	ratio_color;
}	t_colors;

typedef enum e_error {
	MRT_SUCCESS,
	MRT_ERROR
}	t_error;

typedef struct s_parse_line {
	char	*line;
	int		i;
	int		line_nr;
}	t_parse_line;

typedef struct s_angle {
	t_vec3f	k;
	float	angle;
}	t_angle;

struct s_thread_data {
	t_info			*info;
	t_scene			*scene;
	int				next_pixel;
	pthread_mutex_t	pixel_mutex;
};

typedef void	(*t_elem_parser)(t_parse_line *line, t_scene *scene);

void	skip_one_char(t_parse_line *line, char c);
void	skip_zero_or_more_char(t_parse_line *line, char c);
void	skip_one_or_more_char(t_parse_line *line, char c);
char	line_next(t_parse_line *line);

void	check_can_be_normalized(t_vec3f dir, t_parse_line *line, char *name);
void	parse_elem_type(t_parse_line *line, t_scene_elem_type *t);
void	parse_ambient(t_parse_line *line, t_scene *scene);
void	parse_camera(t_parse_line *line, t_scene *scene);
void	parse_light(t_parse_line *line, t_scene *scene);
void	parse_sphere(t_parse_line *line, t_scene *scene);
void	parse_plane(t_parse_line *line, t_scene *scene);
void	parse_cylinder(t_parse_line *line, t_scene *scene);
void	parse_line(t_parse_line line, t_scene *scene);
void	parse_paraboloid(t_parse_line *line, t_scene *scene);
void	parse_int(t_parse_line *line, int *i);
void	parse_float(t_parse_line *line, float *f);
void	parse_vec3i(t_parse_line *line, t_vec3i *v);
void	parse_vec3f(t_parse_line *line, t_vec3f *v);
void	parse_char(t_parse_line *line, char *c);
void	parse_line(t_parse_line line, t_scene *scene);
void	parse_string(t_parse_line *line, char *buffer, int size);
void	parse_texture(t_bmp *texture, t_parse_line *line);

void	check_in_set(t_parse_line *line, char *c, char *set);
void	check_range_f(t_parse_line *line, float f, float min, float max);
void	check_range_vec3f(t_parse_line *line, t_vec3f v, float min, float max);
void	check_range_i(t_parse_line *line, int i, int min, int max);
void	check_range_vec3i(t_parse_line *line, t_vec3i v, int min, int max);
void	check_range_f_exc(t_parse_line *line, float f, float min, float max);
void	parse_check_float(t_parse_line *line, float *f, float min, float max);
void	parse_check_float_exc(t_parse_line *line, float *f, \
		float min, float max);
void	parse_check_float_pos(t_parse_line *line, float *f);
void	check_positive_f(t_parse_line *line, float f);
void	parse_check_int(t_parse_line *line, int *i, int min, int max);
void	parse_check_vec3f(t_parse_line *line, t_vec3f *f, float min, float max);
void	parse_check_vec3i(t_parse_line *line, t_vec3i *i, int min, int max);
void	parse_check_color(t_parse_line *line, t_vec3f *color, int min, int max);
void	parse_check_char(t_parse_line *line, char *c, char *set);
void	check_long_over_int(long l, t_parse_line *line);
void	check_double_over_float(double d, t_parse_line *line);

int		trgb_to_int(int t, int r, int g, int b);
int		rgb_to_color(t_vec3i color);
int		float_to_color(float color);
t_vec3i	float_to_color_vec(t_vec3f color);
t_vec3f	ray_color(t_ray r, t_scene *scene);
t_ray	new_ray(t_ray r, t_vec3f norm_dir, float hit_min);
int		ray_to_pixel_color(t_vec3f ray_colour);

t_vec3f	spot_and_ambient(t_old_new_ray rays, t_vec3f object_color, \
	t_scene *scene, t_vec3f norm_dir);
t_vec3f	spot_light(t_old_new_ray rays, t_vec3f normal, \
	t_light light, t_scene *scene);
t_vec3f	spot_light_specular(t_vec3f normal, t_light light, \
	t_old_new_ray rays, t_scene *scene);
int		in_shadow(t_vec3f pos, t_light light,
			t_old_new_ray rays, t_scene *scene);
int		get_hit_shadow(t_light light, t_old_new_ray rays,
			t_vec3f pos, t_scene *scene);
void	get_scene(char *filename, t_scene *scene);

float	hit_sphere(t_sphere sphere, t_ray r);
float	hit_plane(t_vec3f plane_dir, t_vec3f plane_pos, t_ray r);
float	hit_cylinder(t_cylinder cylinder, t_ray r, int *hit_side);
float	hit_paraboloid(t_paraboloid paraboloid, t_ray r);
t_vec3f	cylinder_side_normal(t_vec3f hit_pos, t_cylinder cylinder);

t_vec3f	plane_normal_bump(t_vec3f pos_on_plane, t_plane plane, t_vec3f ray_dir);
void	get_sphere_angles(t_vec3f normal, float *x_angle, float *y_angle);
int		get_sphere_base_index(t_bmp texture, t_vec3f normal);

void	init_mlx(t_scene *scene);
void	init_image(t_info *info);
int		handle_key(int keycode, void *param);
int		handle_destroy(void *param);
int		draw_to_window(t_info *info);
void	pixel_put_image(t_arr2di *image, int x, int y, int color);
void	paint_img(t_info *info, t_scene *scene);

t_vec3f	f_reflection(t_vec3f incoming, t_vec3f normal);
t_vec3f	get_normal_sphere(t_vec3f hit_point, t_vec3f sphere_center);
t_vec3f	plane_normal(t_vec3f plane_dir, t_vec3f ray_dir);
t_vec3f	paraboloid_normal(t_paraboloid paraboloid, t_ray r, t_hits hit);
t_ray	rotate_ray(t_ray r, t_vec3f pos, t_vec3f dir);
int		ray_in_right_dir(t_ray r, t_cylinder cylinder);
float	hit_top_or_bottom(t_ray ray, t_cylinder cylinder);
float	hit_infinite_cylinder(t_ray r, t_cylinder cylinder);
void	objects_loop(t_ray r, t_scene *scene, t_hits *hits, \
	float distance_to_spot);
void	get_hit(t_hits *hit, t_scene *scene, t_ray r);
t_vec3f	get_plane_pos(t_hits hit, t_ray r, t_plane plane);

t_vec3f	get_sphere_normal_color(t_hits hit, t_ray r, \
	t_vec3f *norm_dir, t_scene *scene);
t_vec3f	get_plane_normal_color(t_hits hit, t_ray r, \
	t_vec3f *norm_dir, t_scene *scene);
t_vec3f	get_cylinder_normal_color(t_hits hit, t_ray r, \
	t_vec3f *norm_dir, t_scene *scene);
t_vec3f	get_paraboloid_normal_color(t_hits hit, t_ray r, \
	t_vec3f *norm_dir, t_scene *scene);
t_vec3f	get_paraboloid_color(t_hits hit, t_ray r, t_vec3f norm_dir, \
	t_scene *scene);
t_vec3f	get_paraboloid_normal(t_hits hit, t_ray r, t_scene *scene);
t_vec3f	get_cylinder_color(t_hits hit, t_ray r, t_vec3f norm_dir, \
	t_scene *scene);
t_vec3f	get_cylinder_normal(t_hits hit, t_ray r, t_scene *scene);
t_vec3f	get_plane_color(t_hits hit, t_ray r, t_vec3f norm_dir, t_scene *scene);
t_vec3f	get_plane_normal(t_hits hit, t_ray r, t_scene *scene);
t_vec3f	get_sphere_color(t_hits hit, t_ray r, t_vec3f norm_dir, t_scene *scene);
t_vec3f	get_sphere_normal(t_hits hit, t_ray r, t_scene *scene);

t_vec3f	get_plane_texture(t_plane plane, t_vec3f plane_pos);
t_vec3f	get_sphere_texture(t_sphere sphere, t_ray r, t_hits hit);

t_vec3f	checkerboard_make_squares(int x_plus_y, t_vec3f color);
float	get_x_angle(t_vec3f rotated_hit_point);
t_vec3f	checkerboard_x_and_y(t_ray r, float hit_min, \
	t_directed_base dir_base, float multiplier);
t_vec3f	get_color_checkerboard_plane(t_plane plane, t_ray r, float hit_min, \
	t_vec3f color);
t_vec3f	get_color_checkerboard_sphere(t_vec3f norm_dir, t_vec3f color);
t_vec3f	get_color_checkerboard_cylinder(t_cylinder cylinder, t_ray r, \
	float hit_min, int hit_side_cylinder);
t_vec3f	get_color_checkerboard_paraboloid(t_paraboloid paraboloid, t_ray r, \
	float hit_min);

t_vec3f	get_color_mirror(t_vec3f norm_dir, t_ray r, \
	float hit_min, t_scene *scene);

float	abc(float a, float b, float c, int *solved);
t_vec3f	ft_rodrigues(t_vec3f v, t_vec3f k, float angle);
t_vec3f	get_rotated_hit_point(t_plane plane, t_ray r, float hit_min);
float	signf(float a);
t_vec3f	at(t_ray r, float t);
t_angle	get_angle_to(t_vec3f dir, t_vec3f to);

void	error(char *str);
void	digit_error(t_parse_line *line);

void	check_magic(uint16_t file_type, char *file);
void	checked_read(int fd, void *buffer, long size, char *file);
int		open_file(char *file);
void	init_bmp_image(t_bmp *image, struct s_BMPInfoHeader infoheader);
t_bmp	read_bmp(char *file);
t_vec3f	read_bump(t_bmp bump_map, int index);
t_vec3f	rotate_bump_to_sphere_normal(t_vec3f bump_normal, t_vec3f normal);
#endif
