/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 21:24:27 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/10 03:59:24 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// echo $? to check what error message was after processing last cmd

// declare int fd[2];
// (1). I need to open two files (argv[1] and argv[4] with different flags)
// the inputfile(argv[1]) should be opened with read only flag
// the outputfile(argv[4]) should be opened with:
// O_WRONLY - writing into the file
// O_CREATE  - creating the regular file 
// O_TRUNC - truncating/setting the file size to length 0
// These things above are flags, when we are creating the file, we need to
// specify its permissions (644). 
// mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
// open(argv[4], O_CREATE | O_WRONLY | O_TRUNC, MODE);

// declare pid_t pid[2];
// (2). I need to do two forks, for two child processes
// check if the pid == -1, then do perror("Error").

// write function int	simple_process(char	**argc, char **envp)

// find command path function needs:
//(1). For sure, 'envp', to pass it to 

// dup2(pipefd[1], STDOUT_FILENO); // fileds[1] - write end, writes to pipe
// close(pipefd[1]);
// dup2(inputfile, STDIN_FILENO); // reads from input file
// close(inputfile);

// dup2(pipefd[0], STDIN_FILENO); // fileds[0] - read end, reads from pipe
// close(pipefd[0]);
// dup2(outputfile, STDOUT_FILENO); // writes to the file
// close(outputfile);

// when the file isn't readable the error number is 0
// when the file isn't writable the error number is 1

// < Makefile "" "cat" > out-the cmd not exist,result:permission denied,code = 0

// int	wait_process(pid_t pid)
// {
// 	int	status;
// 	int	ret;

// 	ret = waitpid(pid, &status, 0);
// 	if (!ret)
// 		return (0);

// }

// printf("%d, %d\n", pid[0], pid[1]);
// close(pipefd[0]);
// close(pipefd[1]);
// printf("%d\n", wait(&status));
// printf("%d\n", wait(&status));
// return (status);
// }

int	main(int argc, char **argv, char **envp)
{
	int	start;

	start = 3;
	if (argc < 5)
		execute_help(argc, argv);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			execute_help(argc, argv);
		return (complex_process(argc, argv, envp, start));
	}
	else
	{
		if (argc == 5)
			return (simple_process(argv, envp));
		start = 2;
		return (complex_process(argc, argv, envp, start));
	}
	return (0);
}

int	complex_process(int argc, char **argv, char **envp, int start)
{
	int	filefd[2];
	int	(*pipefd)[2];
	int	n;
	int	i;

	i = 0;
	n = calculate_cmd(argc, argv);
	get_files_fd(argc, argv, filefd);
	pipefd = malloc(sizeof(int [2]) * (n - 1));
	if (!pipefd)
		pipe_malloc_fail(filefd);
	create_pipes(pipefd, n);
	while (i < n)
	{
		if (fork() == 0)
		{
			duplicate_fd(pipefd, filefd, n, i);
			free(pipefd);
			execute_cmd(argv[start + i], envp, filefd, NULL);
		}
		i++;
	}
	i = wait_clean_get_status(pipefd, filefd, n);
	free(pipefd);
	return (i);
}

void	simple_handle(int *filefd, int *pipefd, char **cmd, char *name_cmd)
{
	close_if_valid(STDIN_FILENO);
	close_if_valid(STDOUT_FILENO);
	if (ft_strchr(name_cmd, '/'))
	{
		if (access(name_cmd, F_OK) != 0)
		{
			close_pipe_fd(pipefd, filefd);
			print_exit("no such file or directory\n", 127, cmd);
		}
		if (access(name_cmd, X_OK) != 0)
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

void	execute_cmd(char *args, char **envp, int *filefd, int *pipefd)
{
	char	**cmd;
	char	*cmd_path;

	cmd = ft_split(args, ' ');
	if (!cmd || !cmd[0])
	{
		if (filefd[0] == -1 && filefd[1] == -1)
		{
			close_if_valid(STDIN_FILENO);
			close_if_valid(STDOUT_FILENO);
		}
		close_pipe_fd(pipefd, filefd);
		print_exit("command not found\n", 127, cmd);
	}
	cmd_path = check_path(cmd[0], envp);
	if (!cmd_path)
		handle_cmd_path_error(filefd, pipefd, cmd, cmd[0]);
	execve(cmd_path, cmd, envp);
	perror(cmd_path);
	free(cmd_path);
	print_exit("permission denied\n", 126, cmd);
}

int	simple_process(char **argv, char **envp)
{
	int		pipefd[2];
	int		filefd[2];
	pid_t	pid[2];

	filefd[0] = open(argv[1], O_RDONLY);
	if (filefd[0] == -1)
		perror(argv[1]);
	filefd[1] = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (filefd[1] == -1)
		perror(argv[4]);
	if (pipe(pipefd) == -1)
		print_exit("Error", 1, NULL);
	pid[0] = fork();
	if (pid[0] == -1)
		print_exit("Error", 1, NULL);
	else if (pid[0] == 0)
		child_process1(pipefd, argv, envp, filefd);
	pid[1] = fork();
	if (pid[1] == -1)
		print_exit("Error", 1, NULL);
	else if (pid[1] == 0)
		child_process2(pipefd, argv, envp, filefd);
	close_if_valid(pipefd[0]);
	close_if_valid(pipefd[1]);
	return (wait_get_status(pid[1], filefd));
}
