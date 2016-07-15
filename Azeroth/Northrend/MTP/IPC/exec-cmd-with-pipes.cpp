#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


vector<string> split(const string &s, const string &delimiter) {
	vector<string> tokens;
	size_t last = 0;
	size_t next = 0;
	size_t n = delimiter.size();

	while ((next = s.find(delimiter, last)) != string::npos) {
		tokens.push_back(s.substr(last, next-last));
		last = next + n;
	}
	tokens.push_back(s.substr(last));

	return tokens;
}


void loop_pipe(string &line) {
	vector<string> commands = split(line, " | ");
	size_t n = commands.size();
	size_t i = 0;

	int pfd[2];
	pid_t pid;
	int fd_in = STDIN_FILENO;

	while (i < n) {
		pipe(pfd);

		if ((pid = fork()) == -1) {
			cout << "pid == -1." << endl;
			perror("Couldn`t fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {  /* Child */
			dup2(fd_in, STDIN_FILENO);
			if (i+1 < n) {
				dup2(pfd[1], STDOUT_FILENO);
			}

			close(pfd[0]);

			vector<string> arguments = split(commands[i], " ");
			size_t an = arguments.size();
			char** args_cptr = new char*[an + 1];
			for (size_t j = 0; j < an; ++j) {
				args_cptr[j] = const_cast<char*>(arguments[j].c_str());
			}
			args_cptr[an] = NULL;

			// cout << "execvp( " << args_cptr[0] << ", ...)" << endl;
			execvp(args_cptr[0], args_cptr);
			// exit(EXIT_FAILURE);
		}
		else {
			wait(NULL);
			close(pfd[1]);
			fd_in = pfd[0];
			++i;
		}
	}
}


int main(/*int argc, char* argv[]*/) {
	string line;
	std::getline(cin, line);

	// cout << "Processing " << line << "..." << endl;

	FILE* fd = fopen("result.out", "w");
	dup2(fileno(fd), STDOUT_FILENO);

	loop_pipe(line);

	fclose(fd);

	return 0;
}
