#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

/*
**	count_commands function is for counting the commands
*/
static int	count_commands(char	**cmds[])
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

static int	picoshell(char **cmds[])
{
	int	num_cmds;
	int	pfd[2];
	pid_t	pid;
	int	in_fd;
	int status;
	int ret;
	int i = 0;

	in_fd = 0;
	num_cmds = count_commands(cmds);
	ret = 0;
	while(cmds[i])
	{
		if (i < (num_cmds - 1))
			pipe(pfd);
		pid = fork();
		if (pid == 0)
		{
			if (i < (num_cmds - 1))
			{
				dup2(pfd[1], 1);
				close(pfd[0]);
				close(pfd[1]);
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
		if (i < (num_cmds - 1))
		{
			in_fd = pfd[0];
			close(pfd[1]);
		}
		i++;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			ret = 1;
		else if (!WIFEXITED(status))
			ret = 0;
	}
	return (ret);
}

int main(void)
{
	char	*cmd2[] = {"ls", "-l", NULL};
	char	*cmd3[] = {"wc", "-l", NULL};
	char	*cmd1[] = {"pwd", NULL};
	char	**cmds[] = {cmd1, cmd2, cmd3, NULL};
	int		ret;

	ret = picoshell(cmds);
	printf("ret = %i;\n", ret);
	return (0);
}
