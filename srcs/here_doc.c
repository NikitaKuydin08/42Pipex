/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:57:16 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/09 21:46:17 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	handle_error(int res, int i, char *line)
{
	if (res == -1 || (res == 0 && i == 0))
	{
		free(line);
		return (0);
	}
	return (1);
}

char	*take_next_line(void)
{
	int		i;
	char	*line;
	int		res;
	char	buffer;

	i = 0;
	line = (char *)malloc(1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	res = read(0, &buffer, 1);
	while (res > 0)
	{
		if (buffer == '\n')
			break ;
		line = ft_realloc(line, i + 2);
		line[i++] = buffer;
		line[i] = '\0';
		res = read(0, &buffer, 1);
	}
	if (handle_error(res, i, line) == 0)
		return (NULL);
	return (line);
}

void	child_process(int *pipefd, char *limiter, int output_fd)
{
	char	*line;

	close_if_valid(output_fd);
	close_if_valid(pipefd[0]);
	line = take_next_line();
	while (line != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			close_if_valid(pipefd[1]);
			free(line);
			exit(0);
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putstr_fd("\n", pipefd[1]);
		free(line);
		line = take_next_line();
	}
	close_if_valid(pipefd[1]);
}

void	parent_process(int *pipefd, pid_t pid)
{
	close_if_valid(pipefd[1]);
	waitpid(pid, NULL, 0);
	dup2(pipefd[0], STDIN_FILENO);
	close_if_valid(pipefd[0]);
}

void	heredoc(char *limiter, int output_fd)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		close_pipe_fd(pipefd, NULL);
		print_exit("Error", 1, NULL);
	}
	pid = fork();
	if (pid == -1)
	{
		close_pipe_fd(pipefd, NULL);
		print_exit("Error", 1, NULL);
	}
	if (pid == 0)
		child_process(pipefd, limiter, output_fd);
	else
		parent_process(pipefd, pid);
}
