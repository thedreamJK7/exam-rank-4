#include "microshell.h"

int	count_commands(char **cmd[])
{
	int i = 0;
	while (cmd[i])
		i++;
	return (i);
}

int    picoshell(char **cmds[])
{
	int status;
	int ret = 0;
	int fd[2];
	int i = 0;
	int num = count_commands(cmds);
	pid_t pid;
	int in_fd = 0;

	while (cmds[i])
	{
		if (i < num - 1)
			pipe(fd);
		pid = fork();
		if (!pid)
		{
			if (i < num - 1)
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
			}
			if (i > 0)
			{
				dup2(in_fd, 0);
				close(in_fd);
			}
			execvp(cmds[i][0], cmds[i]);
		}
		if (i > 0)
			close(in_fd);
		if (i < num - 1)
		{
			in_fd = fd[0];
			close(fd[1]);
		}
		i++;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status))
			ret = 1;
		else if (!WIFEXITED(status))
			ret = 0;
	}
	return(ret);
}

int main(void)
{
	char	*cmd1[] = {"ls", "-l", NULL};
	char	*cmd2[] = {"wc", "-l", NULL};
	char	**cmds[] = {cmd1, cmd2, NULL};
	int ret;
	
	ret = picoshell(cmds);
	printf("ret = %i;\n", ret);
	return (0);
}
