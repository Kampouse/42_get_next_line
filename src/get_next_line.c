/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaleman <jaleman@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 07:11:51 by jaleman           #+#    #+#             */
/*   Updated: 2016/11/26 07:11:52 by jaleman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Allocates sufficient memory for a copy of the string s1, does the copy,
** and returns a pointer to it.
*/

static char			*ft_strdup(const char *s1)
{
	char		*s2;
	size_t		i;

	i = 0;
	while (s1[i])
		i += 1;
	if (!(s2 = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	i = -1;
	while (s1[++i])
		s2[i] = s1[i];
	s2[i] = '\0';
	return (s2);
}

/*
** Allocates with malloc() and returns a “fresh” string ending with ’\0’,
** result of the concatenation of s1 and s2. If the allocation fails the
** function returns NULL.
*/

static char			*ft_strjoin(char const *s1, char const *s2)
{
	char		*s3;
	char		*tmp_s3;
	size_t		i;
	size_t		j;

	j = 0;
	i = 0;
	while (s1[i])
		i += 1;
	while (s2[j])
		j += 1;
	if (!s1 || !s2 || !(s3 = (char *)malloc(sizeof(char) * (i + j + 1))))
		return (NULL);
	tmp_s3 = s3;
	while (*s1 != '\0')
		*tmp_s3++ = *s1++;
	while (*s2 != '\0')
		*tmp_s3++ = *s2++;
	*tmp_s3 = '\0';
	return (s3);
}

/*
** Verify if whatever is in the stack, has a newline. If it doesn't, returns
** a zero (0) to indicate that it's not valid. If there is a newline, we make a
** copy of the stack into the line, and we copied whatever was in the stack
** before, back to the stack (with the temporary stack that we created).
*/

static int			gnl_verify_line(char **stack, char **line)
{
	char			*tmp_stack;
	char			*strchr_stack;
	int				i;

	i = 0;
	strchr_stack = *stack;
	while (strchr_stack[i] != '\n')
		if (!strchr_stack[i++])
			return (0);
	tmp_stack = &strchr_stack[i];
	*tmp_stack = '\0';
	*line = ft_strdup(*stack);
	*stack = ft_strdup(tmp_stack + 1);
	return (1);
}

/*
** Reads into the heap, from the file descriptors, a specific number of bytes
** defined by the BUFF_SIZE macro in the get_nex_line.h file. It's going to
** continue the reading when the return value of the read function is greater
** than zero (no errors, or if there is nothing else to read).
** If there is something in the stack, we will concatinate whatever is in
** there, with whatever is read in the heap. If no, we will just add
** whatever is in the heap into the stack. Then we will verify the stack to
** see if there is a newline. If there is, we will break from the while loop
** and force the positive ret value into a one (1), using the RET_VALUE() macro.
** This answer form SO helped me visualize the stack and heap in a better way:
** http://stackoverflow.com/a/1213360
*/

static	int			gnl_read_file(int fd, char *heap, char **stack, char **line)
{
	int				ret;
	char			*tmp_stack;

	while ((ret = read(fd, heap, BUFF_SIZE)) > 0)
	{
		heap[ret] = '\0';
		if (*stack)
		{
			tmp_stack = *stack;
			*stack = ft_strjoin(tmp_stack, heap);
			free(tmp_stack);
			tmp_stack = NULL;
		}
		else
			*stack = ft_strdup(heap);
		if (gnl_verify_line(stack, line))
			break ;
	}
	return (RET_VALUE(ret));
}

/*
** This is where the real shit happens.
** It first checks for errors (is the line is empty, if the number of the file
** descriptor is invalid, or if it fails to allocate the heap), so it can return
** a minus one (-1) if needed.
**
** If there is something in the stack (because we are using a static variable),
** we verify that there
