/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 01:31:59 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/10 02:39:33 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	handle_cmd_path_error(int *filefd, int *pipefd, char **cmd, char *cname)
{
	if (filefd[0] == -1 && filefd[1] == -1)
	{
		close_if_valid(STDIN_FILENO);
		close_if_valid(STDOUT_FILENO);
	}
	if (ft_strchr(cname, '/'))
	{
		if (access(cname, F_OK) != 0)
		{
			close_pipe_fd(pipefd, filefd);
			print_exit("no such file or directory\n", 127, cmd);
		}
		if (access(cname, X_OK) != 0)
		{
			close_pipe_fd(pipefd, filefd);
			print_exit("permission denied\n", 126, cmd);
		}
	}
	else
	{
		close_pipe_fd(pipefd, filefd);
		print_exit("command not found\n", 127, cmd);
	}
}

void	child_process1(int *pipefd, char **argv, char **envp, int *filefd)
{
	close_if_valid(pipefd[0]);
	pipefd[0] = -1;
	if (filefd[0] < 0)
	{
		close_if_valid(pipefd[1]);
		close_if_valid(filefd[1]);
		exit(1);
	}
	dup2(pipefd[1], STDOUT_FILENO);
	close_if_valid(pipefd[1]);
	pipefd[1] = -1;
	dup2(filefd[0], STDIN_FILENO);
	close_if_valid(filefd[0]);
	filefd[0] = -1;
	simple_execution(argv[2], envp, filefd, pipefd);
}

void	child_process2(int *pipefd, char **argv, char **envp, int *filefd)
{
	close_if_valid(pipefd[1]);
	pipefd[1] = -1;
	if (filefd[1] < 0)
	{
		close_if_valid(pipefd[0]);
		close_if_valid(filefd[0]);
		ft_putstr_fd("permission denied\n", 2);
		exit(1);
	}
	dup2(pipefd[0], STDIN_FILENO);
	close_if_valid(pipefd[0]);
	pipefd[0] = -1;
	dup2(filefd[1], STDOUT_FILENO);
	close_if_valid(filefd[1]);
	filefd[1] = -1;
	simple_execution(argv[3], envp, filefd, pipefd);
}

void	simple_execution(char *args, char **envp, int *filefd, int *pipefd)
{
	char	**cmd;
	char	*cmd_path;

	cmd = ft_split(args, ' ');
	if (!cmd || !cmd[0])
	{
		close_if_valid(STDIN_FILENO);
		close_if_valid(STDOUT_FILENO);
		close_pipe_fd(pipefd, filefd);
		print_exit("command not found\n", 127, cmd);
	}
	cmd_path = check_path(cmd[0], envp);
	if (!cmd_path)
		simple_handle(filefd, pipefd, cmd, cmd[0]);
	execve(cmd_path, cmd, envp);
	perror(cmd_path);
	free(cmd_path);
	print_exit("permission denied\n", 126, cmd);
}
