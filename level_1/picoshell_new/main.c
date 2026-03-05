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

/*
** picoshell funksiyasi buyruqni oladi va uni bajarilishini taminlaydi
** num_cmds - qancha command borligini hisoblaydi
** pfd - file descriptor uchun
** pid - proccess id ni saqlash uchun
** in_fd - oqish fayl deskriptorini saqlash uchun
** status - 
** ret - return valueni saqlash uchun
*/
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
	// loop har bir command uchun
	while(cmds[i])
	{
		// agar oxirgi kommand bolmasa pipe yaratish kerak
		if (i < (num_cmds - 1))
			pipe(pfd);	// yangi pipe yaratish pfd[0] va pfd[1]
		// yangi process yaratish
		pid = fork();
		// CHILD PROCESS :
		if (pid == 0)
		{
			// stdoutni pipega yonaltirish
			if (i < (num_cmds - 1)) // agar oxirgi kommand bolmasa
			{
				dup2(pfd[1], 1); // stdout = pfd[1] (pipe write)
				close(pfd[0]); // read end kerak emas
				close(pfd[1]); // original write endni yop
			}
			// stdinni oldingi pipedan oqishga yonaltirish
			if (i > 0) // agar birinchi command emas
			{
				dup2(in_fd, 0); // stdin = in_fd (oldingi pipe read)
				close(in_fd); // original in fayl deskriptorni yop
			}
			// commandi bajarish
			execvp(cmds[i][0], cmds[i]);
		}
		// PARENT PROCESS:

		// Oldingi pipe read endini yopish
		if (i > 0)	// Birinchi kommanddan keyin
			close(in_fd); // Oldingi pipe read endni yop
		// Keyingi iteratsiya uchun read endni saqlash
		if (i < (num_cmds - 1)) // Oxirgi command emas
		{
			in_fd = pfd[0]; // Joriy pipe read endi = keyingi read end
			close(pfd[1]); // write endni yop parent yozmaydi
		}
		i++;
	}
	// Barcha child processlar tugashini kutish:
	while (wait(&status) > 0) // har bir child tugaguncha
	{
		// Child normal exited va exit code != 0 bo'lsa:
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			ret = 1;
		// Child signal bilan tugagan bo'lsa:
		else if (!WIFEXITED(status))
			ret = 1;
		continue;
	}
	return (ret);
}

/*
** Asosiy funksiya compile qilish uchun, u picoshell funksiyasini chaqiradi va 
** buyruq bajarilishini nazorat qiladi
*/
int main(void)
{
	// char	*cmd2[] = {"ls", "-l", NULL};
	// char	*cmd3[] = {"wc", "-l", NULL};
	char	*cmd1[] = {"pwd", NULL};
	char	**cmds[] = {cmd1, NULL};
	int		ret;

	ret = picoshell(cmds);
	printf("ret = %i;\n", ret);
	return (0);
}
