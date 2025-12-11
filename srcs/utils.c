/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 01:30:41 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/09 22:06:08 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	wait_get_status(pid_t pid, int *filefd)
{
	int		status;
	int		exit_status;
	pid_t	ret;

	exit_status = 1;
	close_if_valid(filefd[0]);
	close_if_valid(filefd[1]);
	while (1)
	{
		ret = wait(&status);
		if (ret == -1)
			break ;
		if (ret == pid && WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

void	execute_help(int argc, char **argv)
{
	if (argc == 1)
	{
		ft_putstr_fd("Invalid number of parameters\n", 2);
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
	}
	else if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		ft_putstr_fd("Invalid number of parameters\n", 2);
		ft_putstr_fd("Usage: ./pipex here_doc LIMITER cmd cmd1 file", 2);
	}
	else
	{
		ft_putstr_fd("Invalid number of parameters\n", 2);
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
	}
	exit(1);
}

void	print_exit(const char *msg, int error_code, char **cmd)
{
	if (ft_strncmp(msg, "Error", 5) == 0)
	{
		perror("Error");
		exit(1);
	}
	if (cmd)
		free_split(cmd);
	ft_putstr_fd(msg, 2);
	exit(error_code);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
