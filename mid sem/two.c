#include <stdio.h>    
#include <stdlib.h>    
#include <unistd.h>   
#include <ctype.h>    
                      

// translator(user_to_translator, translator_to_user)
void translator(int input_pipe[], int output_pipe[]) 
{ 
    // define variables to use throughout the function
    int c;     
    char ch;   
    int rc;    

    /* first, close unnecessary file descriptors */ 
	close(input_pipe[1]); // close user_to_translator input end
    close(output_pipe[0]);  // close translator_to_user output end

    /* enter a loop of reading from the user_handler's pipe, translating */ 
    /* the character, and writing back to the user handler.              */ 
    while (read(input_pipe[0], &ch, 1) > 0) { 
        c = ch; // store the read char into an int to perform operations below
        if (isascii(c) && isupper(c))  // check if the char is ascii or upper case
            c = tolower(c);  // if so, convert it to lower case
        ch = c; // store the int (c) back into a char (ch)

        /* write translated character back to user_handler. */ 
        // and store the return value of the write (to check for error (-1))
		rc = write(output_pipe[1], &ch, sizeof(char)); // write one char to the user
		
        // error checking if there was an error when writing to the ouput_pipe
		if (rc == -1) { 
            perror("translator: write"); // classify the error
            close(input_pipe[0]); // close user_to_translator output end 
            close(output_pipe[1]); // close translator_to_user input end
            exit(1); // exit with error code
        } 
    } 

    close(input_pipe[0]); // close user_to_translator output end 
    close(output_pipe[1]); // close translator_to_user input end
    exit(0); // exit normally
} 


// user_handler(translator_to_user, user_to_translator)
void user_handler(int input_pipe[], int output_pipe[]) 
{ 
    int c;    /* user input - must be 'int', to recognize EOF (= -1). */ 
    char ch;  /* the same - as a char. */ 
    int rc;   /* return values of functions. */ 

    /* first, close unnecessary file descriptors */ 
	close(input_pipe[1]); // close translator_to_user input end
    close(output_pipe[0]); // close user_to_translator output end

    // prompt the user to provide some text to translate
	printf("Enter text to translate:\n");
    /* loop: read input from user, send via one pipe to the translator, */ 
    /* read via other pipe what the translator returned, and write to   */ 
    /* stdout. exit on EOF from user.                                   */ 
    while ((c = getchar()) > 0) { 
        ch = (char)c; // convert the int representation of the read char to a char

        /* write to translator */ 
		rc = write(output_pipe[1], &ch, sizeof(char)); // write one char to the translator and store its return value

        if (rc == -1) { /* write failed - notify the user and exit. */ 
            perror("user_handler: write"); // classify the error
            close(input_pipe[0]); // close translator_to_user output end
            close(output_pipe[1]); // close user_to_translator input end
            exit(1); // exit with error code
        } 

        /* read back from translator */ 
		rc = read(input_pipe[0], &ch, sizeof(char)); // read one char from the translator and store its return value

        c = (int)ch; // convert char to int
        if (rc <= 0) {  // check if the read operation failed
            perror("user_handler: read"); // classify the error
            close(input_pipe[0]); // close translator_to_user output end
            close(output_pipe[1]); // close user_to_translator input end
            exit(1); // exit with error code
        } 

        // write the char to stdout/terminal
		putchar(c); 
		if (c=='\n' || c==EOF) break; // check if the char is an EOF, if so, break the loop
    } 

    close(input_pipe[0]); // close translator_to_user output end
    close(output_pipe[1]); // close user_to_translator input end
    exit(0); // exit normally
} 




int main(int argc, char* argv[]) 
{ 
    int user_to_translator[2]; // variable to store the user_to_translator pipe file descriptors
    int translator_to_user[2]; // variable to store the translator_to_user pipe file descriptors
    int pid; // variable to store the childs pid
    int rc; // variable to store the rc of function calls

    // create a pipe and store the fds in user_to_translator
    rc = pipe(user_to_translator); 
    if (rc == -1) { // check if there was an error
        perror("main: pipe user_to_translator"); // classify the error
        exit(1); // exit with error code
    } 

    // create a pipe and store the fds in translator_to_user
	rc = pipe(translator_to_user); 
    if (rc == -1) { // check if there was an error
        perror("main: pipe translator_to_user"); // classify the error
        exit(1); // exit with error code
    } 

    // fork the process, creating a child process and storing the pid
    pid = fork(); 

    switch (pid) { 
        // if there was an error when forking
        case -1:         
            perror("main: fork"); // classify the error
            exit(1); // exit with error code
        // if we are in the child process
        case 0:
            // call the translator function in the child process
            translator(user_to_translator, translator_to_user); /* line 'A' */ 
			exit(0); // exit
        // else, we are the parent thread
        default:         
            // call the user handler function in the parent process
            user_handler(translator_to_user, user_to_translator); /* line 'B' */ 
    } 

    // exit
    return 0;   
}



/* Show below the output for the following input: I wish YOU a HAPPY new YEAR

[sre90@cs18262kq ~/Desktop/360 test]$ ./two
Enter text to translate:
I wish YOU a HAPPY new YEAR
i wish you a happy new year

*/