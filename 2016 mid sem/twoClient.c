#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define PRE_FILE "PRE_PIPE"
#define POST_FILE "POST_PIPE"
#define BUFSIZE 80

int main(void) {
	FILE* fp1;
	FILE* fp2;
	char buffer[BUFSIZE] = {0};
	char recieved[BUFSIZE] = {0};
	
	fp1 = fopen(PRE_FILE, "w");
	printf("Enter a string to compress (no spaces): ");
	scanf("%s", buffer);

	// send this entered text to the server via "PRE_PIPE"
	fwrite(buffer, sizeof(char), strlen(buffer), fp1);

	fclose(fp1);
	
	umask(0);
	mknod(POST_FILE, S_IFIFO | 0666, 0);
	fp2 = fopen(POST_FILE, "r");

	// read compressed text back from server via �POST_PIPE�
	int num_bytes = fread(recieved, sizeof(char), BUFSIZE - 1, fp2);
	recieved[num_bytes] = '\0';

	printf("Recieved from server: %s\n", recieved);
	fclose(fp2);
	
	return 0;
}
