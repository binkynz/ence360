#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
       
#define PRE_FILE "PRE_PIPE"
#define POST_FILE "POST_PIPE"
#define BUFSIZE 80

#define ASCII_A 97
#define ASCII_E 101
#define ASCII_I 105
#define ASCII_O 111
#define ASCII_U 117


int main(void) {
	FILE* fp1;
	FILE* fp2;
	char buffer[BUFSIZE] = {0};
	char modified[BUFSIZE] = {0};
	pid_t childPid;
	int child_status;
	int fd1[2];
	int fd2[2];
	int i,p;
	
		
	/* set up pipes */
	if (pipe(fd1) == -1) {
		perror("pipe");
		exit(1);
	}
		
	if (pipe(fd2) == -1) {
		perror("pipe");
		exit(1);
	}
		
	
	/* fork a child */
	childPid = fork();
	if(childPid == -1) {
		perror("fork");
		exit(2);
	}
		
	else if (childPid == 0) {
		/* child code */

   		/* read text from a pipe from the parent */
		close(fd1[1]); close(fd2[0]);
		int num_bytes = read(fd1[0], modified, BUFSIZE - 1);
		modified[num_bytes] = '\0';
   			
   		/* convert all leters to lower case & remove vowels */
   		for(p=0, i=0; i < strlen(modified); i++) {
			char c = tolower(modified[i]);
			if (c != ASCII_A && c != ASCII_E && c != ASCII_I && c != ASCII_O && c != ASCII_U) // not a vowel
				modified[p++] = c;
		}
		modified[p] = '\0';
   			
   		/* write compressed text back to a pipe to the parent: */
		write(fd2[1], modified, num_bytes + 1);
		close(fd1[0]); close(fd2[1]);

		/* child is finished so exit */
    	exit(0);
	}

	  else {
		/* parent code */

		/*Create and open the client-to-parent named pipe */
		mknod(PRE_FILE, S_IFIFO | 0666, 0);
		fp1 = fopen(PRE_FILE, "r");
		/*Read the message from the client*/
		/* read text from client via the named pipe: */
		int num_bytes = fread(buffer, sizeof(char), BUFSIZE - 1, fp1);
		buffer[num_bytes] = '\0';

		/* pipe text to the child: */
		close(fd1[0]); close(fd2[1]);
		write(fd1[1], buffer, num_bytes);

		/* the child will convert the message...*/
		/* read compressed text back from the child: */
		num_bytes = read(fd2[0], modified, BUFSIZE - 1);
		modified[num_bytes] = '\0';
		close(fd1[1]); close(fd2[0]);

	
		fp2 = fopen(POST_FILE, "w");
		/* pipe compressed text back to the client: */
		fwrite(modified, sizeof(char), num_bytes + 1, fp2);
	}
	
	return 0;
}
