#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80
#define DELIMITERS " \t\n\v\f\r"

//Put current arguments to NULL, avoid the influence of last iteration.
void refresh_args(char *args[]);

int get_input(char *command);

//Split and store user input "command" to args.
size_t parse_input(char *args[], char *original_command);

//Check whether an ampersand(&) is in the end of args.
//If so, remove it from args and possibly reduce the size of args.
int check_ampersand(char **args, size_t *size);

//Check wheter the redirection tokens is in arguments and remove such tokens.
unsigned check_redirection(char **args, size_t *size, char **input_file, char **output_file);

//Open files and redirect I/O.
int redirect_io(unsigned io_flag, char *input_file, char *output_file, int *input_desc, int *output_desc);

//Close files for input and output.
void close_file(unsigned io_flag, int input_desc, int output_desc);

//Detect the pipe '|' and split aruguments into two parts accordingly.
void detect_pipe(char **args, size_t *args_num, char ***args2, size_t *args_num2);

int run_command(char **args, size_t args_num);

int main(void) {
	char **args = (char**)malloc((MAX_LINE/2 + 1)*sizeof(char*));
	char *command = (char*)malloc((MAX_LINE + 1)*sizeof(char));

	while (1) {
		printf("osh>");
		fflush(stdout);
		fflush(stdin);

		refresh_args(args);  //Avoid the influence of last iteration.

		if(!get_input(command)) {
			printf("get_input falied!\n");
			continue;
		}
		if(strcmp(command, "exit\n") == 0)
			break;
		if(strcmp(command, "\n") == 0)
			continue;

		size_t args_num = parse_input(args, command);
		run_command(args, args_num);
	}

	free(args);
	free(command);
	return 0;
}

//Put current arguments to NULL, avoid the influence of last iteration.
void refresh_args(char *args[]) {
	while(*args) {
		free(*args);
		*args++ = NULL;
	}
}

int get_input(char *command) {
	char *input_buffer = (char*)malloc((MAX_LINE + 1)*sizeof(char));
	if(fgets(input_buffer, MAX_LINE + 1, stdin) == NULL) {
		fprintf(stderr, "Failed to read input!\n");
		return 0;
	}
	if(strncmp(input_buffer, "!!", 2) == 0) {
		if(strlen(command) == 0) {  // no history yet
			fprintf(stderr, "No history available yet!\n");
			return 0;
		}
		printf("Last command is %s", command);    // keep the command unchanged and print it
		return 1;
	}
	strcpy(command, input_buffer);  // update the command

	free(input_buffer);
	return 1;
}

//Splite and store user input "command" in args.
size_t parse_input(char *args[], char *original_command) {
	size_t num = 0;
	char *command = (char*)malloc((MAX_LINE + 1)*sizeof(char));
	strcpy(command, original_command);  // make a copy since 'strtok' will modify it
	char *token = strtok(command, DELIMITERS);
	while(token != NULL) {
		args[num] = (char*)malloc((strlen(token) + 1)*sizeof(char));
		strcpy(args[num], token);
		++num;
		token = strtok(NULL, DELIMITERS);
	}

	free(command);
	return num;
}

//Check whether an ampersand(&) is in the end of args.
//If so, remove it from args and possibly reduce the size of args.
int check_ampersand(char **args, size_t *size) {
	size_t len = strlen(args[*size - 1]);
	if(args[*size - 1][len - 1] != '&') {
		return 0;
	}
	if(len == 1) {  //remove this argument if it only contains '&'
		free(args[*size - 1]);
		args[*size - 1] = NULL;
		--(*size);  //reduce its size
	}
	else {
		args[*size - 1][len - 1] = '\0';
	}
	return 1;
}

//Check wheter the redirection tokens is in arguments and remove such tokens.
unsigned check_redirection(char **args, size_t *size, char **input_file, char **output_file) {
	unsigned flag = 0;
	size_t to_remove[4], remove_cnt = 0;
	for(size_t i = 0; i != *size; ++i) {
		if(remove_cnt >= 4) {
			break;
		}
		if(strcmp("<", args[i]) == 0) {  //input
			to_remove[remove_cnt++] = i;
			if(i == (*size) - 1) {
				fprintf(stderr, "No input file provided!\n");
				break;
			}
			flag |= 1;
			*input_file = args[i + 1];
			to_remove[remove_cnt++] = ++i;
		}
		else if(strcmp(">", args[i]) == 0) {  //output
			to_remove[remove_cnt++] = i;
			if(i == (*size) - 1) {
				fprintf(stderr, "No output file provided!\n");
				break;
			}
			flag |= 2;
			*output_file = args[i + 1];
			to_remove[remove_cnt++] = ++i;
		}
	}

	//Remove I/O indicators and filenames from arguments.
	for(int i = remove_cnt - 1; i >= 0; --i) {
		size_t pos = to_remove[i];  //the index of arg to remove
        // printf("%lu %s\n", pos, args[pos]);
		while(pos != *size) {
			args[pos] = args[pos + 1];
			++pos;
		}
		--(*size);
	}
	return flag;
}

//Open files and redirect I/O.
int redirect_io(unsigned io_flag, char *input_file, char *output_file, int *input_desc, int *output_desc) {
	//printf("IO flag: %u\n", io_flag);
	if(io_flag & 2) {  //redirecting output
		*output_desc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 644);
		if(*output_desc < 0) {
			fprintf(stderr, "Failed to open the output file: %s\n", output_file);
			return 0;
		}
		// printf("Output To: %s %d\n", output_file, *output_desc);
		dup2(*output_desc, STDOUT_FILENO);
	}
	if(io_flag & 1) { //redirecting input
		*input_desc = open(input_file, O_RDONLY, 0644);
		if(*input_desc < 0) {
			fprintf(stderr, "Failed to open the input file: %s\n", input_file);
			return 0;
		}
		//printf("Input from: %s %d\n", input_file, *input_desc);
		dup2(*input_desc, STDIN_FILENO);
	}
	return 1;
}

//Close files for input and output.
void close_file(unsigned io_flag, int input_desc, int output_desc) {
	if(io_flag & 2) {
		close(output_desc);
	}
	if(io_flag & 1) {
		close(input_desc);
	}
}

//Detect the pipe '|' and split aruguments into two parts accordingly.
void detect_pipe(char **args, size_t *args_num, char ***args2, size_t *args_num2) {
	for(size_t i = 0; i != *args_num; ++i) {
		if (strcmp(args[i], "|") == 0) {
			free(args[i]);
			args[i] = NULL;
			*args_num2 = *args_num -  i - 1;
			*args_num = i;
			*args2 = args + i + 1;
		break;
		}
	}
}

int run_command(char **args, size_t args_num) {
	//Detect '&' to determine whether to run concurrently.
	int run_concurrently = check_ampersand(args, &args_num);

    	//Detect pipe
	char **args2;
	size_t args_num2 = 0;
	detect_pipe(args, &args_num, &args2, &args_num2);

	//Create a child process and execute the command
	pid_t pid = fork();
	if(pid < 0) {  //fork failed
		fprintf(stderr, "Failed to fork!\n");
		return 0;
	}
	else if (pid == 0) { //child process
		if(args_num2 != 0) {  //pipe
			//Create pipe
			int fd[2];
			pipe(fd);
			//Fork into another two processes
			pid_t pid2 = fork();
			if(pid2 > 0) {  //child process for the second command
                		// Redirect I/O
				char *input_file, *output_file;
				int input_desc, output_desc;
				unsigned io_flag = check_redirection(args2, &args_num2, &input_file, &output_file);    // bit 1 for output, bit 0 for input
				io_flag &= 2;   //disable input redirection
				if(redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
					return 0;
				}
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
				wait(NULL);  //wait for the first command to finish
				execvp(args2[0], args2);
				close_file(io_flag, input_desc, output_desc);
				close(fd[0]);
				fflush(stdin);
			}
			else if(pid2 == 0) {  //grandchild process for the first command
				//Redirect I/O
				char *input_file, *output_file;
				int input_desc, output_desc;
				unsigned io_flag = check_redirection(args, &args_num, &input_file, &output_file);    // bit 1 for output, bit 0 for input
				io_flag &= 1;   //disable output redirection
				if(redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
					return 0;
				}
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				execvp(args[0], args);
				close_file(io_flag, input_desc, output_desc);
				close(fd[1]);
				fflush(stdin);
			}
		}
		else {  //no pipe
			//Redirect I/O
			char *input_file, *output_file;
			int input_desc, output_desc;
			unsigned io_flag = check_redirection(args, &args_num, &input_file, &output_file);  //bit 1 for output, bit 0 for input
			if(redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
				return 0;
			}
			execvp(args[0], args);
			close_file(io_flag, input_desc, output_desc);
			fflush(stdin);
		}
	}
	else { // parent process
		if(!run_concurrently) { // parent and child run concurrently
		wait(NULL);
		}
	}
	return 1;
}
