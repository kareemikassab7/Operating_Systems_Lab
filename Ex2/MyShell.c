#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_LEN 1024


size_t read_command(char *cmd) {
if(!fgets(cmd,BUFFER_LEN,stdin))/*get command and put it in line*/
return 0;/*if user hits CTRL+D break*/
size_t length=strlen(cmd);/* get command length*/
if(cmd[length-1] =='\n') cmd[length-1] ='\0';/* clear new line*/
return strlen(cmd);/* return length of the command read*/
}

int build_args(char * cmd, char ** argv) {
char *token;/*split command into separate strings*/
token = strtok(cmd," ");
int i=0;
while(token!=NULL){/* loop for all tokens*/
argv[i]=token;/* store token*/

if (argv[i]==">")
	inflag= true;

if(argv[i]=="<")
	outflag= true;

if(argv[i]=="=")
	assignflag= true;

if(argv[i]=="|")
	pipeflag= true;

token=strtok(NULL," ");/* get next token*/
i++;/* increment number of tokens*/
}
argv[i]=NULL;/*set last value to NULL for execvp*/
return i; /* return number of tokens*/
}

void set_program_path (char * path, char * bin, char * prog) 
{memset(path,0,1024);/* intialize buffer*/
strcpy(path,bin);/*copy /bin/ to file path*/
strcat(path,prog);/*add program to path*/
int i=0;
for( i;i<strlen(path);i++)/*delete newline*/
	if(path[i]=='\n')path[i]='\0';}


int getOperator(int argc, char** argv, ){
	int order

	/*create loop to find order of the find the order of the operator in command to redirect
	and pipe as required*/
return order;
}


int redirectIn (int input, char** argv, int argc ){

int opOrder;
opOrder= getOperator(argc, argv, command);
input= open(argv[4/*should be variable according to cmd length*/], O_CREAT | O_TRUNC | O_WRONLY, 0666);
dup2(input, 0);
close(input);
return 1;
}


int redirectOut(int output, char** argv, int argc ){
int opOrder;
opOrder= getOperator(argc, argv, command);
input= open(argv[4/*should be variable according to cmd length*/], O_CREAT | O_RDONLY);
dup2(output, 0);
close(output);

return 1;
}

int assignVar(char var[], char val[], char** argv){

char kbuffer[128];
strcpy(kbuffer, argv[0]);
char * token = strtok(kbuffer, "=");
strcpy(var, token);
token = strtok(kbuffer, NULL);
strcpy(val, token);
setenv(var,val,1);

//putenv(argv[0]);
return 1;
}





void PipeCommands(){ /*to handle piping*/
/*need to handle pipes*/
}



int main(){

/*printf("HOME : %s\n", getenv("HOME"));*/

char line[BUFFER_LEN];/*get command line*/
char*argv[100];/*user command*/
char*bin="/bin/";/*set path at bin*/
char path[1024];/*full file path*/
char var[128];
char val [128];
char* dirpath[1024];
int dirsize;
int argc;/*arg count*/
bool redirectionFlag,inflag, outflag, assignflag, pipeflag;
int callin, callout, callassign, callpipe;
while(1){
strcpy (dirpath,(getenv("HOME")));
printf("%s", dirpath);
printf (":  ")	;
/*printf(":  My shell>> "); /*print shell prompt*/
if(read_command(line) ==0) 
	{printf("\n");break;}/* CRTL+D pressed*/
if(strcmp(line,"exit") ==0) break; /*exit*/
argc=build_args (line,argv);/*build program argument*/
set_program_path (path,bin,argv[0]);/* set program full path*/



if (!(strcmp(argv[0],"cd"))){
printf("I'm in cd cond\n");
chdir(argv[1]);		
printf("path:%s\n",getcwd(dirpath,100));}



int pid=fork();/*fork child*/
if (pid==0){/*Child*/

if(inflag == 1){

callin= redirectIn();

}

if (outflag==1){
callout= redirectOut();
}

if (assignflag==1){

callassign= assignVar(); 
}



if (pipeflag==1){
callassign= PipeCommands();
}


execve(path,argv,0);/*if failed process is not replaced, then print error message*/
/*fprintf(stderr,"Child process could not do execve\n");*/
}else wait(NULL);/*Parent*/
}



return 0;
}


