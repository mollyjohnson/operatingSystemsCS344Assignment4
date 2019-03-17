/*
Name: Molly Johnson
ONID: johnsmol
CS 344 Winter 2019
Assignment 4
Due: 3/17/19
All information used to create this code is adapted from the OSU CS 344 Winter 2019
lectures and assignment instructions/hints unless otherwise specifically indicated.
Also adapted from my own work previously created on 11/30/18 (I took the class previously
in the Fall 2018 term but am retaking for a better grade).
*/

//include the shared .h file (also has other needed .h files and macro definitions in the SharedFunctions.h file)
#include "SharedFunctions.h"

/*
NAME
main
SYNOPSIS
main function. takes in args from the user, sets up socket connection, receives data from client and sends
data back to the client
DESCRIPTION
takes in the port number (and this program should be run in the background w the "&" added to the end of 
the command) from the user. creates and initializes variables, sets up listening socket connection, performs
error checking, calls other needed functions, receives the plaintext and key file contents from the client (along
with special added chars), encrypts them into ciphertext, and sends the ciphertext back to the client.
*/
int main(int argc, char *argv[])
{
	//check that user entered correct number of args. if not, print error to stderr and exit w non-zero
	//exit value. (args should be executable name and port number. and this program should be run in background
	//by adding "&" to the end of the command)
	if (argc != 2){
		Error("args error! wrong number of args entered. args should be executable name, and port number (and '&' symbol to run in background)\n", 1);
	}

	//create socket variables
	int listenSocketFD, establishedConnectionFD, charsRead;
	socklen_t sizeOfClientInfo;

	//create temp buffer, plain text buffer, key buffer, combined buffer, and cipher text buffer
	char buffer[MAX_BUF];
	char completePlaintext[MAX_BUF];
	char completeKey[MAX_BUF];
	char completeCombinedBuf[MAX_BUF * 2];
	char cipherBuf[MAX_BUF];

	//create exit status variable (will be passed into waitpid() after forking when the parent is waiting
	//for the child process to terminate)
	int childExitStatus = 0;

	//convert port num (argv[3]) passed in from string to int
	//using atoi to convert from string to int in c adapted from:
	//https://www.quora.com/How-do-I-convert-character-value-to-integer-value-in-c-language
	int portNumber = atoi(argv[1]);
	
	//check if user entered 0, negative number, or non-number for the key length. keyLength == 0 means
	//user entered key length of 0, or that atoi failed (atoi also returns 0 if it had an error). or if 
	//IntInputValidation is false, (i.e. if the user enters a negative number or non-numeric input),
	//print error message and exit w non zero exit status.
	if((portNumber == 0) || (IntInputValidation(argv[1]) == FALSE)){
		Error("error! invalid port num. You entered a 0, negative number, or non-number as the port num\n", 1);	
	}

	//memset all buffers to null terminators
	memset(buffer, '\0', sizeof(buffer));
	memset(completePlaintext, '\0', sizeof(completePlaintext));
	memset(completeKey, '\0', sizeof(completeKey));
	memset(cipherBuf, '\0', sizeof(cipherBuf));
	memset(completeCombinedBuf, '\0', sizeof(completeCombinedBuf));

	//set up socket struct
	struct sockaddr_in serverAddress, clientAddress;

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket

	//check for error opening the socket. if socket() returns < 0 was an error. if was an error, print
	//error message to stderr and exit w non zero exit value
	if (listenSocketFD < 0){
		Error("ERROR opening socket", 1);
	}

	//Enable the socket to begin listening
	//bind the socket and check for an error (if bind returns < 0). if an error, print error message to
	//stderr and exit w non zero exit value
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){ // Connect socket to port
		Error("ERROR on binding", 1);
	}

	//call listen() for the socket so can listen for connections
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	//use of a while loop here and forking to allow for multiple concurrent connections to the
	//socket adapted from:
	//https://cboard.cprogramming.com/networking-device-communication/62309-using-fork-accept-multiple-clients.html and
	//https://www.youtube.com/watch?v=BIJGSQEipEE and
	//https://www.daniweb.com/programming/software-development/threads/448522/multiple-clients-single-server-fork-sockets#

	//loop until an exit is called
	while(1){

		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept the connection

		//check if there was an error on accepting the connection (i.e. if accept() returned < 0). if so, print error message to
		//stderr and exit w non zero exit status
		if (establishedConnectionFD < 0){
			Error("ERROR on accept", 1);
		}

		//create spawnpid for forking (spawnpid will be the pid of the forked child)
		pid_t spawnpid = -5;

		//fork off a child process
		spawnpid = fork();

		//create variable to check for num of forks
		int forkCount = 0;

		//check that fork count is below the max (5) to prevent accidental fork bombs.
		if(forkCount >= MAX_FORKS){
			//if fork bomb error, print error message to stderr and exit w non zero exit value
			Error("fork bomb error! exiting!\n", 1);		
		}
		else{ //within max num of forks
			//use switch statement to see what fork() returned.
			switch(spawnpid){
				case -1:
					//if fork returns -1, was an error, no child process created. print error message to stderr and exit
					//w non zero exit value
					Error("Fork Hull Breach!\n", 1);
					break;
				case 0: //i am the child process
					//get sent message from the client. to make sure entire message is received, continue loopint until the
					//special key chars (i.e. end of the combined plaintext buffer and key buffer) has been reached. (search
					//for key special chars instead of enc special ones because if a bad connect has been made, there would
					//be dec special chars at the very end not enc, so would error. special enc/dec chars come immediately
					//after the special key chars, so will still be received okay). special key chars at the end of the
					//combined plaintext and key text files are found when strstr != NULL.
					while(strstr(completeCombinedBuf, KEY_SPECIAL_CHARS) == NULL){ //long as we haven't found terminal chars
						//reset the temp buffer to null terminators each time through the loop so can receive the data
						//from this iteration of recv() in the loop without having any old data in it.
						memset(buffer, '\0', sizeof(buffer));

						//read data from the socket and put it into the temp buffer
						charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer) - 1, 0); // Read the client's message from the socket

						//concat the temp buffer (i.e. whatever text was received from this iteration of the loop) onto
						//the complete combined plaintext + key buffer
						strcat(completeCombinedBuf, buffer);

						//check that reading from the socket did not have an error (i.e. that recv didn't return < 0). if
						//it did, print error message to stderr and exit w non zero exit value
						if (charsRead < 0){
							Error("ERROR reading from socket\n", 1);
						}
					}

					//check if the client attempting to connect is valid (otp_enc) or invalid (otp_dec). if
					//the connection isn't valid, set the cipherbuf text to be the BAD_CONNECT message and add
					//on the plaintext special chars (since if was an error otp_dec would be expecting to receive
					//a plaintext file back and use strstr to read it, otp_dec is NOT expecting ciphertext special
					//chars like otp_enc would be).
					if(IsValidConnection(completeCombinedBuf, ENC_SPECIAL_CHARS) == FALSE){
						//set cipher buf text to BAD_CONNECT
						strcpy(cipherBuf, BAD_CONNECT);
						//concat plaintext special chars onto the ciphertext buffer
						strcat(cipherBuf, PLAIN_SPECIAL_CHARS);
					}
					else{ //if connection was valid (i.e. otp_enc was trying to connect not otp_dec)

						//separate the combined plaintext+keytext buffer into two separate plaintext and key buffers
						SeparateEncBuffers(completeCombinedBuf, completePlaintext, completeKey);

						//double check no invalid chars were received in the key file. if are invalid chars,
						//print error message to stderr but don't exit parent process per assignment instructions. exit
						//child process w non zero exit value
						if(CharInputValidation(completeKey) == FALSE){
							Error("error! invalid character(s) in your key file\n", 1);
						}
						//double check no invalid chars were received in the plaintext file. if are invalid chars,
						//print error message to stderr but don't exit parent process per assignment instructions.
						//exit child process w non zero exit value
						if(CharInputValidation(completePlaintext) == FALSE){
							Error("error! invalid character(s) in your plaintext file\n", 1);
						}

						//double check that the key file length is at least as long as the plaintext file length. if
						//is invalid length, print error message to stderr but don't exit parent process per assignment
						//instructions. exit child process w non zero exit value
						if(IsValidLength(completePlaintext, completeKey) == FALSE){
							Error("error! key file is shorter than the plaintext file\n", 1);
						}
		
						//do encryption and copy encrypted text to ciphertext string
						Encrypt(completePlaintext, completeKey, cipherBuf);

						//double check that the encrypted ciphertext doesn't contain any invalid chars. if are any
						//invalid chars, print error message to stderr but don't exit parent process per assignment
						//instructions. exit child process w non zero exit value
						if(CharInputValidation(cipherBuf) == FALSE){
							Error("error! invalid character(s) in your cipher text file\n", 1);
						}

						//add cipher special chars to the cipher text string so client will know when enture message
						//has been received
						strcat(cipherBuf, CIPHER_SPECIAL_CHARS);
					}

					//send cipher text buffer back to client
					charsRead = send(establishedConnectionFD, cipherBuf, strlen(cipherBuf), 0);

					//check that send() was successful (will return < 0 if an error). if not successful, print
					//error message to stderr but don't exit parent process per assignment instructions. exit
					//child process w non zero exit value
					if(charsRead < 0){
						Error("otp_enc_d(server): ERROR writing to socket\n", 1);
					}

					//check that entire length of the cipher text buffer was put into the send buffer to be sent.
					//if sent less than the length of the cipher text buffer, print error message to stderr but
					//don't exit parent process per assignment instructions. exit child process w non zero exit value
					if(charsRead < strlen(cipherBuf)){
						Error("otp_enc_d (server): WARNING: not all data written to socket!\n", 1);
					}

					//create check send variable for verified sending with ioctl
					int checkSend = -5; //holds amount of bytes remaining in send buffer

					//use do-while loop to loop until socket buffer has no more bytes to send
					do{
						//use ioctl() to see how many bytes are left to be sent from the send buffer for this socket
						ioctl(establishedConnectionFD, TIOCOUTQ, &checkSend); //check the send buffer for this socket
					}while(checkSend > 0); //loop forever until send buffer for this socket is empty				

					//if ioctl returned < 0 and exited the loop due to that, print error message to stderr but don't
					//exit parent process per the assignment instructions. exit child process w non zero exit value
					if(checkSend < 0){
						Error("otp_enc_d (server): ioctl error\n", 1);
					}

					//exit the child process
					exit(0);
					break;
				default: //i am the parent process
					//close the existing connected socket that's currently connected w otp_enc
					close(establishedConnectionFD); // Close the existing socket which is connected to the client

					//use waitpid() to have the parent process wait for the specified child process with the spawnpid
					//process id to terminate (prevents zombies). pass in the exit status for the child as well as the
					//last parameter as a 0 (since don't want these child processes to run in the background so am NOT
					//using the WNOHANG option with waitpid here)
					spawnpid = waitpid(spawnpid, &childExitStatus, 0);
					break;
			}
			//increment the fork counter to prevent fork bombing the server
			forkCount++;
		}
	}

	close(listenSocketFD); // Close the listening socket

	return 0; 
}
