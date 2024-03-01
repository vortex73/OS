// Program to demo pipes

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main()
{
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE] = "";
	int fd[2]; // as 2 file descriptors reqd
	pid_t pid; 

	// Create a pipe 
	if (pipe(fd) == -1) {
		printf("Pipe failed\n"); // -1 on error
		return 1;
	}

	// Fork a child process 
	pid = fork();

	if (pid > 0)  // parent process
	{	
		// Close the unused end of the pipe
		close(fd[READ_END]); // it writes to child, so close read port of parent, important

		// Write to the pipe
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);

		// Close the write end of the pipe
		close(fd[WRITE_END]);

		wait(NULL); // to wait for child, to avoid orphan process
	}
	else if (pid == 0)  // Child process 
	{
		// close the unused end of the pipe 
		close(fd[WRITE_END]); // as it only has to read

		// read from the pipe
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("child process read -%s\n", read_msg);

		// close the write end of the pipe
		close(fd[READ_END]);
	}
	else // error while forking
	{ 
		printf("Fork failed\n");
		return 1;
	}

}
