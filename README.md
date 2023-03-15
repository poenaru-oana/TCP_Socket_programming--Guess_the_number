# TCP_Socket_programming--Guess_the_number

How it works:

Server generates a random number from 1 to 10, and the connected clients try to guess it within 10 tries.

After each guess, client receives one of three messages from the server:

- B - bigger	(the generated number is bigger than the one the client provided)
- S - smaller	(the generated number is smaller than the one the client provided)
- W - win		(client correctly guessed the number)
- L - loss	(client did not guess the number)

The program validates the sent and received data, has a time_out function, and, when necessary, provides error messages and exits with error code 1.

	The server side of the connection was written in C, on a VM, in an Ubuntu server, using vim.
	The client side of the connection was written in Python, on host machine, in Windows, using PyCharm.
	The protocol used for the connection is TCP.

This project was realised during a laboratory test taken for the Computer Networks course in Semester 3.
