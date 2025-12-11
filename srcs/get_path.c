/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:49:40 by nkuydin           #+#    #+#             */
/*   Updated: 2025/12/07 20:49:45 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**get_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split((envp[i] + 5), ':'));
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*cat;
	char	*cmd_path;

	i = 0;
	while (paths[i])
	{
		cat = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(cat, cmd);
		free(cat);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*check_path(char *cmd, char **envp)
{
	char	*cmd_path;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			return (NULL);
		}
		return (NULL);
	}
	paths = get_path(envp);
	if (!paths)
		return (NULL);
	cmd_path = find_cmd_path(paths, cmd);
	free_split(paths);
	return (cmd_path);
}
