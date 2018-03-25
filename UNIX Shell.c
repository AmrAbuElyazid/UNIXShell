#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#define MAX_LINE 80

int main(void)
{
	char *args[MAX_LINE/2 + 1];
	char myString[MAX_LINE];
	int should_run = 1;

	char *history[10] = {"\0"};
	int historyCount = 0;

	while(should_run) {
	    
	    printf("osh>");
	    fflush(stdout);

	    scanf("%[^\n]%*c", myString);
	    printf("Input is %s\n", myString);

	    if(myString[0] == '!') {
		    if(myString[1] == '!') {
		    	strcpy(myString, history[historyCount - 1]);
		    	printf("Last History is %s\n", myString);
		    } else if (isdigit(myString[1])) {
		    	int index = myString[1] - '0';
		    	strcpy(myString, history[index - 1]);
		    	printf("Executing %s \n", myString);
		    } else {
		    	printf("Error\n");
		    	continue;
		    }

	    } else {
		    if (historyCount == 10) {
		    	printf("History full\n");
		    }else{
		    	history[historyCount] = (char *) malloc(100);
			    strcpy(history[historyCount], myString);
			    historyCount++;
		    }
	    }


	    int i = 0;

	    args[i] = strtok(myString," ");

	    do {
			i++;
			args[i] = strtok(NULL, " ");
	    } while (args[i] != NULL);

	    if(strcmp(args[0], "exit") == 0) {
	    	break;
	    }

	    if(strcmp(args[0], "history") == 0) {
			for(i = 0; history[i] != '\0'; i++)
			{
			   printf("%d. %s\n", i, history[i]);
			}
	    	continue;
	    }

	    if(strcmp(args[i-1], "&") != 0) {
			pid_t pid;
			pid = fork();
			if(pid < 0) {
			    fprintf(stderr,"Fork failed\n");
			    return 1;
			}
			else if (pid == 0) {
			    execvp(args[0],args);
			    for(int j = 0; j < i - 1; j++) {
			      args[j] = NULL;
			    }
			} else {
			    wait(NULL);
			}
	    } else {
			pid_t pid;
			pid = fork();
			if(pid < 0){
			    fprintf(stderr,"Fork failed\n");
			    return 1;
			} else if (pid == 0){
			    args[i-1] = NULL;
			    execvp(args[0],args);
			}
	    }
	}
	return 0;
}
