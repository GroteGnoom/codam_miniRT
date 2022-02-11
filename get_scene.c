#include "miniRT.h"

void	parse_line(t_parse_line line, t_scene *scene)
{
	t_scene_elem_type		et;
	static t_elem_parser	parse_elem[NR_ELEM_TYPES] = {
		parse_ambient,
		parse_camera,
		parse_light,
		parse_sphere,
		parse_plane,
		parse_cylinder,
	};

	parse_elem_type(&line, &et);
	parse_elem[et](&line, scene);
}

void	get_scene(char *filename, t_scene *scene)
{
	int				fd;
	t_parse_line	line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error("Could not open file");
	line.line = get_next_line(fd);
	line.line_nr = 0;
	while (line.line)
	{
		line.i = 0;
		// printf("line: %s", line.line);
		if (ft_strcmp(line.line, "\n"))
			parse_line(line, scene);
		free(line.line);
		line.line = get_next_line(fd);
		line.line_nr++;
	}
}