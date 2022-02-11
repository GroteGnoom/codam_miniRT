#include "miniRT.h"

void    test_reflection(void)
{
    t_vec3f incoming = {1, -1, 1};
    t_vec3f normal = {0, 1, 0};
    t_vec3f result;

    result = f_reflection(incoming, normal);
    printf("------ Reflection -----\n");
    vec3f_print(result);
    printf("\n");
}

void    test_normal(void)
{
    t_vec3f hit_point = {0, 0, 1};
    t_vec3f sphere_center = {0, -1, 1};
    t_vec3f normal;

    normal = get_normal_sphere(hit_point, sphere_center);
    printf("------ Normal -----\n");
    vec3f_print(normal);
    printf("\n");
}

void    test_at(void)
{
    t_ray   r;
    float   distance;
    t_vec3f new_at;

    r.origin = vec3f_init(0, 0, 1);
    r.dir = vec3f_init(1, 1, 0);
    distance = 5;
    new_at = at(r, distance);
    printf("------ AT -----\n");
    vec3f_print(new_at);
    printf("\n");
}

void    test_ray_color(void)
{
    t_ray   r;
    t_scene scene = {};
    t_vec3f color;

    get_scene("test_light.rt", &scene);
    r.origin = vec3f_init(0, 0, 0);
    r.dir = vec3f_init(0, 0, -1);
    color = ray_color(r, &scene);
    printf("------ Ray Color -----\n");
    vec3f_print(color);
    printf("\n");
}

void    test_ray_to_color(void)
{
    t_vec3f ray_color = {0, 0, 0};
    int     color;

    color = sphere_to_pixel_color(ray_color);
    printf("------ Ray color to color int -----\n");
    printf("color = %i\n", color);
    printf("\n");
}

void    test_ray_bounce(void)
{
    t_ray   r;
    t_scene scene = {};

    get_scene("test_light.rt", &scene);
    r.origin = vec3f_init(0, 0, 0);
    r.dir = vec3f_init(0, 0, -1);
    printf("------ Ray Bounce -----\n");
    ray_color(r, &scene);
    printf("\n");
}

int main(void)
{
    test_reflection();
    test_normal();
    test_at();
    test_ray_color();
    test_ray_to_color();
    test_ray_bounce();
}