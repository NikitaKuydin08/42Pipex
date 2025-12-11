/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:53:22 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/10 03:58:53 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>

# include "../42Libft/includes/libft.h"

// the functions getting command path - get_path.c
char	*check_path(char *cmd, char **envp);
char	*find_cmd_path(char **paths, char *cmd);
char	**get_path(char **envp);

// utilities like freeing and error handles - utils.c
void	free_split(char **split);
void	print_exit(const char *msg, int error_code, char **cmd);
void	execute_help(int argc, char **argv);
int		wait_get_status(pid_t pid, int *filefd);
void	close_all_pipes(int (*pipefd)[2], int n);

// execution of the mandatory part - mandatory.c
void	child_process2(int *pipefd, char **argv, char **envp, int *filefd);
void	child_process1(int *pipefd, char **argv, char **envp, int *filefd);
void	execute_cmd(char *args, char **envp, int *filefd, int *pipefd);
void	close_if_valid(int fd);
void	close_pipe_fd(int *pipefd, int *filefd);
void	simple_execution(char *args, char **envp, int *filefd, int *pipefd);
void	handle_cmd_path_error(int *filefd,
			int *pipefd, char **cmd, char *cname);
void	simple_handle(int *filefd, int *pipefd, char **cmd, char *name_cmd);

// the logic of the bonus part - multiple_cmd.c
int		wait_clean_get_status(int (*pipefd)[2], int *filefd, int n);
void	duplicate_fd(int (*pipefd)[2], int *filefd, int n, int i);
void	get_files_fd(int argc, char **argv, int *filefd);
void	create_pipes(int (*pipefd)[2], int n);
int		calculate_cmd(int argc, char **argv);

// the logic of the input redirecting - here_doc.c
void	heredoc(char *limiter, int output_fd);

// main.c (simple & complex processes)
int		complex_process(int argc, char **argv, char **envp, int start);
int		simple_process(char **argv, char **envp);
void	pipe_malloc_fail(int *filefd);

#endif