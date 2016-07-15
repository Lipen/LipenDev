#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;


void operate(int fd) {
	char c;
	bool finished = false;
	string cmd, answer;
	char* pch;
	char* path;
	char* protocol;
	char buf[1024];
	ssize_t r;

	while (!finished) {
		cmd = answer = "";

		// Read line from client
		while (!finished) {
			finished = read(fd, &c, 1) != 1;

			if (c == '\n') break;
			else if (c != '\r') cmd += c;
		}

		// Process command (if exist, and if no error)
		if (!finished && !cmd.empty()) {
			if (cmd == "hello") {
				answer = "hey!";
			}
			else if (cmd == "quit") {
				shutdown(fd, 2);
				close(fd);
				finished = true;
			}
			else {
				char* cmd_ = new char[cmd.size()];
				strcpy(cmd_, cmd.c_str());
				pch = strtok(cmd_, " \r\n");
				if (strlen(pch) >= 3 && pch[0]=='G' && pch[1]=='E' && pch[2]=='T') {
					path = strtok(NULL, " \r\n");
					protocol = strtok(NULL, " \r\n");

					std::ifstream t(path);

					if (t) {
						answer += "HTTP/1.0 200 OK\r\n\r\n";
						std::stringstream buffer;
						buffer << t.rdbuf();
						answer += buffer.str();
					}
					else {
						answer += "HTTP/1.0 404 Not Found\r\n\r\n";
						//
						answer += path;
					}
				}
			}
		}

		// Send answer
		if (!finished && !answer.empty()) {
			answer += "\n";
			write(fd, answer.c_str(), answer.size());
		}
	}

	exit(0);
}

void mainloop() {
	int sock, fd;
	struct sockaddr_in sa;
	socklen_t n;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock != -1) {

		memset(&sa, 0, sizeof(sa));

		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		sa.sin_port = htons(5555);

		if (bind(sock, (struct sockaddr*)&sa, sizeof(sa)) != -1) {
			while (1) {
				if (!listen(sock, 5)) {
					n = sizeof(sa);

					if ((fd = accept(sock, (struct sockaddr*)&sa, &n)) != -1) {
						// TODO: log here

						if (!fork()) {
							operate(fd);
						}
					}
				}
			}
		}
	}
}

void sighandler(int signum) {
	waitpid(0, NULL, WNOHANG);
}


int main(int argc, char* argv[]) {
	pid_t pid, sid;
	struct sigaction sa;

	pid = fork();

	if (pid < 0) {
		perror("Bad fork");
	}
	else if (pid > 0) {
		/* Parent */
		cout << "OK. pid = " << pid << endl;
	}
	else {
		/* Child */
		sid = setsid();
		chdir("/");
		umask(0);

		// Close descriptors
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		// Handle child death
		memset(&sa, 0, sizeof(sa));
	    sa.sa_handler = &sighandler;
	    sigaction(SIGCHLD, &sa, 0);

		// TODO: log here
		mainloop();

		exit(0);
	}
}
