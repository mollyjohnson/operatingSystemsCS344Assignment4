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
main function. takes in args from the user, sets up socket, sends data to server and
receives data back from server
DESCRIPTION
takes in the plaintext file name, key file name, and the port num. creates and initializes variables,
sets up socket connection, performs user input and socket error checking, calls other needed functions,
sends the plain and key file contents (along with special added chars) to the server, receives ciphertext
data back from the server. prints to stdout.
*/
int main(int argc, char *argv[]){

	//check that user entered correct number of args. if not, print error to stderr and exit w non-zero
	//exit value. (args should be executable name, plaintext file, key file, and port num)
	if (argc != 4){
		Error("args error! wrong number of args entered. args should be executable name, plaintext file, key file, and port number.\n", 1);
	}

	//create socket structs and variables
	int socketFD, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	//create key text buffer, plain text buffer, and cipher text buffer
	char keyBuf[MAX_BUF];
	char plainBuf[MAX_BUF];
	char cipherBuf[MAX_BUF];
	char tempBuf[MAX_BUF];
	char combinedBuf[MAX_BUF * 2];

	//memset all buffers to null terminators
	memset(keyBuf, '\0', sizeof(keyBuf));
	memset(plainBuf, '\0', sizeof(plainBuf));
	memset(cipherBuf, '\0', sizeof(cipherBuf));
	memset(tempBuf, '\0', sizeof(tempBuf));
	memset(combinedBuf, '\0', sizeof(combinedBuf));
    
	//set plaintext and key file names to the proper user arguments
	char *plainFileName = argv[1];
	char *keyFileName = argv[2];

	//convert port num (argv[3]) passed in from string to int
	//using atoi to convert from string to int in c adapted from:
	//https://www.quora.com/How-do-I-convert-character-value-to-integer-value-in-c-language
	int portNum = atoi(argv[3]);

	//check if user entered 0, negative number, or non-number for the key length. keyLength == 0 means
	//user entered key length of 0, or that atoi failed (atoi also returns 0 if it had an error). or if 
	//IntInputValidation is false, (i.e. if the user enters a negative number or non-numeric input),
	//print error message and exit w non zero exit status.
	if((portNum == 0) || (IntInputValidation(argv[3]) == FALSE)){
		Error("error! invalid port num. You entered a 0, negative number, or non-number as the port num\n", 1);	
	}

	//get the plaintext file contents (AND STRIP OFF NEWLINE CHAR) and put into plain buffer
	GetFileContents(plainFileName, plainBuf);

	//get the keytext file contents (AND STRIP OFF NEWLINE CHAR) and put into key buffer
	GetFileContents(keyFileName, keyBuf);

	//check that all plaintext chars are valid. if not, print error message to stderr and exit w/
	//non-zero exit status
	if(CharInputValidation(plainBuf) == FALSE){
		char message[256];
		strcpy(message, "otp_enc error: ");
		strcat(message, argv[1]);
		strcat(message, " input contains bad characters\n");
		Error(message, 1);
	}

	//check that all keytext chars are valid. if not, print error message to stderr and exit w/
	//non-zero exit status
	if(CharInputValidation(keyBuf) == FALSE){
		char message[256];
		strcpy(message, "otp_enc error: ");
		strcat(message, argv[2]);
		strcat(message, " input contains bad characters\n");
		Error(message, 1);
	}

	//check that the key buffer length is valid (i.e. is at least as long as the plaintext buffer).
	//if it's not, print error message to stderr and exit w/ non-zero exit status
	if(IsValidLength(plainBuf, keyBuf) == FALSE){
		char message[256];
		strcpy(message, "Error: key '");
		strcat(message, argv[2]);
		strcat(message, "' is too short\n");
		Error(message, 1);
	}

	//concat the special chars onto the plaintext and key strings to be looked for 
	//by the server using strstr to make sure all data gets sent across in full
	strcat(plainBuf, PLAIN_SPECIAL_CHARS);
	strcat(keyBuf, KEY_SPECIAL_CHARS);

	//concat the plain buf (plus special chars) and key buf (plus special chars) into one
	//combined buffer
	strcat(combinedBuf, plainBuf);
	strcat(combinedBuf, keyBuf);
	
	//concat the special char unique to enc to the combined plaintext + key text buffer to make
	//sure that the enc client and enc_d server are connecting (and not enc and dec_d)
	strcat(combinedBuf, ENC_SPECIAL_CHARS);

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNum); // Store the port number
	serverHostInfo = gethostbyname(HOST_ADDRESS); // Convert the machine name into a special form of address

	//check that the host exists. if not, print error message to stderr and exit w non zero exit value
	if(serverHostInfo == NULL){ 
		Error("otp_enc (client): ERROR, no such host\n", 1);
	}
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket

	//check that the socket could be opened. if not, print error message to stderr and
	////exit w non zero exit value
	if(socketFD < 0){
		Error("otp_enc (client): ERROR opening socket", 1);
	}
	
	// Connect to server and check that the client could connect to the server on the specified port
	if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){ // Connect socket to address
		//if connect returned < 0 was an error. create error message about not being able to connect to the
		//server on the specified port, print error message to stderr, and exit w exit val of 2
		char message[256];
		strcpy(message, "otp_enc (client): ERROR connecting to otp_enc_d (server on port ");
		strcat(message, argv[3]); strcat(message, ")\n");
		Error(message, 2);
	}

	// Send plaintext message to server w send() function
	charsWritten = send(socketFD, combinedBuf, strlen(combinedBuf), 0); // Write to the server

	//check that the chars were actually written to the send buffer to be sent. if send returned < 0
	//was an error, print error message to stderr and exit w non zero exit value
	if(charsWritten < 0){
		Error("otp_enc (client): ERROR writing to socket\n", 1);
	}

	//also check that the full buffer was actually written to the send buffer to be sent. if send returned
	//a value less than the length of the combined bufer string, print error message to stderr and
	//exit w non zero exit value
	if(charsWritten < strlen(combinedBuf)){
		Error("otp_enc (client): WARNING: not all data written to socket!\n", 1);
	}

	//create check send variable for verified sending with ioctl
	int checkSend = -5; //holds amount of bytes remaining in send buffer

	//use do-while loop to loop until the socket buffer has no more bytes to send
	do{
		//use ioctl() to see how many bytes are left to be sent from the send buffer for this socket
		ioctl(socketFD, TIOCOUTQ, &checkSend); //check the send buffer for this socket
	}while(checkSend > 0); //loop forever until send buffer for this socket is empty

	//if ioctl returned < 0 and exited the loop due to that, print error message to stderr and exit
	//with non zero exit value
	if(checkSend < 0){
		Error("otp_enc (client): ioctl error\n", 1); //check if we actually stopped the loop bc of the error
	}
		
	// Get return message from server. to make sure entire message is received, continue looping until the special
	// cipher chars at the end of the cipher text file are found (i.e. strstr != NULL for those special cipher chars).
	while(strstr(cipherBuf, CIPHER_SPECIAL_CHARS) == NULL){
		//reset the temporary buffer to null terminators each time through the loop so can receive the data from this recv()
		//in the loop without having any old data in it
		memset(tempBuf, '\0', sizeof(tempBuf));

		//read data from the socket and put it into the temporary buffer
		charsRead = recv(socketFD, tempBuf, sizeof(tempBuf) - 1, 0); // Read data from the socket, leaving \0 at end

		//concat the temporary buffer (i.e. whatever text was received from this iteration of the loop) onto the complete
		//ciphertext buffer
		strcat(cipherBuf, tempBuf);

		//check that reading from the socket did not have an error (i.e. that recv didn't return < 0). if it did, print
		//error message to stderr and exit w non zero exit value.
		if(charsRead < 0){
			Error("ERROR reading from socket\n", 1);
		}
	}

	//use strstr to find the location of the ciphertext special chars. remove terminating cipher special chars by find the terminating location
	int termLocation = strstr(cipherBuf, CIPHER_SPECIAL_CHARS) - cipherBuf;

	//replace the char at the terminating location (i.e. where the ciphertext special char is) with a null terminator
	cipherBuf[termLocation] = '\0';
	
	//check for bad connection (like if otp_enc attempted to connect to otp_dec_d instead of otp_enc_d). if it did try to make
	//a bad connection, otp_dec_d will return "BAD_CONNECT" instead of ciphertext. check for this with stringmatch().
	if(StringMatch(cipherBuf, BAD_CONNECT) == TRUE){
		//if the otp_dec_d server did return a bad connection message since otp_enc tried to connect to it, create an error message
		//string, print to stderr and exit with an exit value of 2.
		char message[256];
		strcpy(message, "otp_enc: Error: could not contact otp_enc_d on port ");
		strcat(message, argv[3]); strcat(message, ". ");
		strcat(message, "otp_enc cannot use otp_dec_d.\n");
		Error(message, 2);
	}
	
	//print the ciphertext w a newline char to stdout per assignment instructions
	printf("%s\n", cipherBuf); fflush(stdout);
	
	close(socketFD); // Close the socket

	return 0;
}
