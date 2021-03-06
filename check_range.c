/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_range.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dnoom <marvin@codam.nl>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 11:25:30 by dnoom         #+#    #+#                 */
/*   Updated: 2022/04/06 11:25:30 by dnoom         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	check_range_i(t_parse_line *line, int i, int min, int max)
{
	if (i < min || i > max)
	{
		printf(
			"Expected int between %d and %d at line %d, column %d, found %d",
			min, max, line->line_nr, line->i + 1, i);
		error("int not in range");
	}
}

void	check_range_f_exc(t_parse_line *line, float f, float min, float max)
{
	if (f <= min || f >= max)
	{
		printf(
			"Expected float between %f and %f (excluding endpoints) at line %d "
			", column %d, found %f", min, max, line->line_nr, line->i + 1, f);
		error("Float not in range");
	}
}

void	check_range_f(t_parse_line *line, float f, float min, float max)
{
	if (f < min || f > max)
	{
		printf(
			"Expected float between %f and %f at line %d, column %d, found %f",
			min, max, line->line_nr, line->i + 1, f);
		error("Float not in range");
	}
}

void	check_range_vec3i(t_parse_line *line, t_vec3i v, int min, int max)
{
	int		j;
	int		i;

	i = 0;
	while (i < 3)
	{
		j = *(&v.x + i);
		if (j < min || j > max)
		{
			printf(
				"Expected three ints between %d and %d at line %d, "
				"before column %d, found %d for int number %d",
				min, max, line->line_nr, line->i + 1, j, i);
			error("Int not in range");
			exit(1);
		}
		i++;
	}
}

void	check_range_vec3f(t_parse_line *line, t_vec3f v, float min, float max)
{
	float	f;
	int		i;

	i = 0;
	while (i < 3)
	{
		f = *(&v.x + i);
		if (f < min || f > max)
		{
			printf(
				"Expected three floats between %f and %f at line %d, "
				"before column %d, found %f for float number %d",
				min, max, line->line_nr, line->i + 1, f, i);
			error("Float not in range");
			exit(1);
		}
		i++;
	}
}
