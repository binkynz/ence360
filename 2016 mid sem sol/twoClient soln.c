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
	//fputs(buffer, fp1); //send the message
	fwrite(buffer, 1, strlen(buffer), fp1);//send the message
	fclose(fp1);
	
	umask(0);
	mknod(POST_FILE, S_IFIFO | 0666, 0);
	fp2 = fopen(POST_FILE, "r");
	//fgets(recieved, BUFSIZE, fp2); //read from server
	fread(recieved, 1, BUFSIZE, fp2);//read from server
	printf("Recieved from server: %s\n", recieved);
	fclose(fp2);
	
	return 0;
}
