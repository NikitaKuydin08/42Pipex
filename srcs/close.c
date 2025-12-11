/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:57:38 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/10 00:41:47 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_pipe_fd(int *pipefd, int *filefd)
{
	close_if_valid(filefd[0]);
	close_if_valid(filefd[1]);
	if (pipefd)
	{
		close_if_valid(pipefd[0]);
		close_if_valid(pipefd[1]);
	}
}

void	close_if_valid(int fd)
{
	if (fd >= 0)
		close(fd);
}

void	pipe_malloc_fail(int *filefd)
{
	close_if_valid(filefd[0]);
	close_if_valid(filefd[1]);
	perror("Error");
	exit(1);
}

void	close_all_pipes(int (*pipefd)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipefd[i][0] >= 0)
			close(pipefd[i][0]);
		if (pipefd[i][1] >= 0)
			close(pipefd[i][1]);
		i++;
	}
}
