#include "../pipex.h"

int main(int argc, char *argv[])
{
	int fdin;
	int fdout;
	int res;
	int pid1;
	int pid2;
	int pfd[2];
	open_check_files(argv[1], argv[2], &fdin, &fdout, argc);
	res = pipe(pfd);
	if (res == -1)
		return 1;
	
	pid1 = fork();
	if (pid1 < 0)
		return 2;
	if (pid1 == 0)
	{
		// grep child
		close(pfd[0]);
		// replace stdin by finput
		dup2(fdin, STDIN_FILENO);
		// replace stdout by pid[1]
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		execlp("grep", "grep", " tes ", NULL);

		return (0);
	}
	
	pid2 = fork();
	if (pid2 < 0)
		return 2;
	if (pid2 == 0)
	{
		// wc child
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		dup2(fdout, STDOUT_FILENO);
		execlp("wc", "wc", "-l", NULL);
		return (0);
	}
	close(pfd[0]);
	close(pfd[1]);
	wait(NULL);
	wait(NULL);
	close_check_files(fdin, fdout);
	return (0);
}