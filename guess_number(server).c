//server generates a random number from 1 to 10, and client tries to guess it within 10 tries
//after each guess, client receives one of three messages from the server: 
//	B - bigger	(the generated number is bigger than the one the client provided)
//	S - smaller	(the generated number is smaller than the one the client provided)
//	W - win		(client correctly guessed the number)
//	L - loss	(client did not guess the number)

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int c;

void time_out(int signal) {
	int32_t result = -1;
	result = htonl(result);

	printf("TIME OUT.\n");

	send(c, &result, sizeof(int32_t), 0);

	close(c);
	exit(1);
}

void handle_client(int32_t snum) {
	char cnums[3];
	int32_t cnum;
	int code, tries = 0;
	char res;

	if (c < 0) {
		fprintf(stderr, "ERROR ON ESTABLISHING CONNECTION WITH THE CLIENT\n");
		exit(1);
	}

	signel(SIGALRM, time_out);
	alarm(10);

	res = 'K';
	while (res != 'W' && tries != 10) {
		code = recv(c, &cnums, 3, 0);
		cnum = atoi(cnums);
		tries++;

		if (code == sizeof(int32_t))
			alarm(10);
		else {
			res = 'E';
			break;
		}

		printf("Client guess: %d\n", cnum);

		if (cnum = snum) {
			res = 'W';
			send(c, &res, 1, 0);
			printf("client guessed.\n");
			break;
		}
		else if (cnum < snum) {
			res = 'B';
			send(c, &res, 1, 0);
		}
		else if (cnum > snum) {
			res = 'S';
			send(c, &res, 1, 0);
		}

	}

	alarm(0);

	if (res == 'E') {
		send(c, &res, 1, 0);
		close(c);
		exit(1);
	}

	if (tries >= 10) {
		res = 'L';
		printf("client lost.\n");
		send(c, &res, 1, 0);
	}

	close(c);
	exit(0);
}

int main() {
	int code;
	int s;
	int32_t snum;

	struct sockaddr_in client, server;

	time_t t1;
	srand((unsigned)time(&t1));
	snum = rand() % 11;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		fprintf(stderr, "ERROR ON CREATE SERVER SOCKET!\n");
		return 1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(1500);

	code = bind(s, (struct sockaddr*)&server, sizeof(server));
	if (code < 0) {
		fprintf(stderr, "ERROR ON BIND SERVER SOCKET!\n");
		return 1;
	}

	fprintf("Server generated number %d\n", snum);

	listen(s, 4);

	while (1) {

		memset((struct sockaddr*)&client, 0, sizeof(client));

		printf("Waiting for connection...");

		int len = sizeof(client);
		c = accept(s, (struct sockaddr*)&client, (socklen_t*)&len);

		printf("Incoming from %s:%d\n", inet_ntoa(client.sin_addr), nthos(client.sin_port));

		if (fork() == 0) {
			handle_client(snum);
		}
	}

}