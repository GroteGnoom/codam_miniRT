#include "vec.h"
#include <math.h>

float	vec3f_dot(t_vec3f v1, t_vec3f v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

t_vec3f	vec3f_cross(t_vec3f v1, t_vec3f v2)
{
	float	x;
	float	y;
	float	z;

	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
	return (vec3f_init(x, y, z));
}

float	vec3f_len_sq(t_vec3f v)
{
	return (vec3f_dot(v, v));
}

float	vec3f_len(t_vec3f v)
{
	return (sqrtf(vec3f_len_sq(v)));
}

int vec3f_equal(t_vec3f v1, t_vec3f v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

static int almost_equal(float f1, float f2)
{
	float e = 0.000001;
	return (f1 + e >= f2 && f1 - e <= f2);
}

int vec3f_almost_equal(t_vec3f v1, t_vec3f v2)
{
	return (almost_equal(v1.x, v2.x) && almost_equal(v1.y, v2.y) && almost_equal( v1.z, v2.z));
}
