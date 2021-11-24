#include "libft.h"
#include "stdlib.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (!lst->content)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}
