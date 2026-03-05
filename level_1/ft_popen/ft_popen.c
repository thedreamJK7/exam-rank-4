#include <unistd.h>
#include <stdio.h>

/*
** ft_popen funksiyasi commandni ishga tushirib unga pipe orqali bog'lanadi
** Parametrlar:
** 	file - bajariladigan dastur nome(misol uchun: "ls", "cat")
**	argv - command argumentlari (misol uchun: {"ls", "-l", NULL})
**	type - 'r' = o'qish uchun (commandan malumot olish)
**			'w' = yozish (commandga malumot yuborish)
*/
int	ft_popen(const char *file, char *argv[], char type)
{
	int fd[2];
	int	pid;

	if (!argv || !argv[0])
		return (-1);
	if (!(type == 'r' || type == 'w'))
		return (-1);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		// READ MODE (type == 'r'):
		// Parent o'qimoqchi, shuning uchun child yozishi kerak
		if (type == 'r')
			dup2(fd[1], 1); // Child YOZADI (parent o'qishi uchun)
		// WRITE MODE (type == 'w'):
		// Parent yozmoqchi, shuning uchun child o'qishi kerak
		if (type == 'w')
			dup2(fd[0], 0);  // Child O'QIYDI (parent yozishi uchun)
		close(fd[0]);
		close(fd[1]);
		execvp(file, (char * const *)argv);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	close(fd[0]);
	return (fd[1]);
}

int main(void)
{
	char buffer[1024];
	char *args1[] = {"ls", "-l", NULL};
    int fd = ft_popen("ls", args1, 'r');
	int bytes_read;


    char line;
    while ((bytes_read = read(fd, buffer, 1023)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(fd);
}