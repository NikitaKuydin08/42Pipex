/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:47:43 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/09 23:51:18 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	calculate_cmd(int argc, char **argv)
{
	int	n;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		n = argc - 4;
	else
		n = argc - 3;
	return (n);
}

void	create_pipes(int (*pipefd)[2], int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		pipe(pipefd[i]);
		i++;
	}
}

void	get_files_fd(int argc, char **argv, int *filefd)
{
	mode_t	mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		filefd[0] = -1;
		filefd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, mode);
		if (filefd[1] == -1)
			perror(argv[argc - 1]);
		heredoc(argv[2], filefd[1]);
	}
	else
	{
		filefd[0] = open(argv[1], O_RDONLY);
		if (filefd[0] == -1)
			perror(argv[1]);
		filefd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, mode);
		if (filefd[1] == -1)
			perror(argv[argc - 1]);
	}
}

void	duplicate_fd(int (*pipefd)[2], int *filefd, int n, int i)
{
	if (i == 0)
		dup2(filefd[0], STDIN_FILENO);
	else
		dup2(pipefd[i - 1][0], STDIN_FILENO);
	if (i == n - 1)
		dup2(filefd[1], STDOUT_FILENO);
	else
		dup2(pipefd[i][1], STDOUT_FILENO);
	close_all_pipes(pipefd, n - 1);
	close_if_valid(filefd[0]);
	close_if_valid(filefd[1]);
	filefd[0] = -1;
	filefd[1] = -1;
}

int	wait_clean_get_status(int (*pipefd)[2], int *filefd, int n)
{
	int	status;
	int	exit_status;

	exit_status = 1;
	close_all_pipes(pipefd, n - 1);
	close_if_valid(filefd[0]);
	close_if_valid(filefd[1]);
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}
