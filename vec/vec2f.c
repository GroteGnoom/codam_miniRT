/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vec2f.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dnoom <marvin@codam.nl>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/05 16:42:33 by dnoom         #+#    #+#                 */
/*   Updated: 2022/04/05 16:42:33 by dnoom         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

t_vec2f	vec2f_init(float x, float y)
{
	t_vec2f	vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

t_vec2f	vec2f_add(t_vec2f v1, t_vec2f v2)
{
	t_vec2f	v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return (v);
}
