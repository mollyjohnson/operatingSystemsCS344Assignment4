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
error
SYNOPSIS
prints error message and exits
DESCRIPTION
takes in string arg (a specified error message) and an int (exit status). prints this message to stderr,
and then exits with the exit status of the int passed in
*/
void Error(const char *msg, int exitStat){
	//print error message specified by the user to stderr and exit w/ exit status specified
	fprintf(stderr, msg); fflush(stdout); exit(exitStat);
}

/*
NAME
intinputvalidation
SYNOPSIS
checks string input to make sure all chars are valid integers
DESCRIPTION
takes in one string argument. will convert each element of the string into chars, and check
if the ascii value for each char is w/in the range for integers (range for integers 0-9 =
ascii values 48-57). will return false if any input was not an integer, otherwise returns true
if all chars were valid integers from 0-9. this function is adapted from my own work created
for OSU CS 162 Winter 2017, 1/19/17
*/
int IntInputValidation(char *inputString){
	//create variables for char to be checked, loop counter, and length of the input string
	char asciiValue;
	int inputLength = strlen(inputString);
	int i;

	//loop through entire string so can check each char
	for(i = 0; i < inputLength; i++){
		//set temp char equal to the current string char
		asciiValue = inputString[i];

		//check if the current char is within the valid ascii values for ints 0-9 (i.e. ascii values 48-57)
		if((asciiValue < 48) || (asciiValue > 57)){
			//if ascii value of char being checked is outside of the range for integers, return false
			return FALSE;
		}
	}
	//if all of the chars in the string to be checked were valid integers, return true
	return TRUE;
}

/*
NAME
charValidation
SYNOPSIS
validates that the chars in a string are valid (upper case letters or a space char)
DESCRIPTION
checks all chars for the length of the string, checking to see if they're valid (valid if
one of the uppercase alphabet letters or a space char, invalid otherwise). if all chars are 
valid, returns true. else if any are invalid, returns false. asci values for A is 65, Z is 90,
the remaining upper-case alphabetical letters are in between these values, and the space char is 32.
adapted from my own work created for OSU CS 162 Winter 2017, 1/19/17
*/
int CharInputValidation(char *inputString){
	//create variables for char to be checked, and length of the input string
	char asciiValue;
	int k;
	int stringLength = strlen(inputString);

	//check each char of the string to see if matches one of the valid chars
	for(k = 0; k < stringLength; k++)
	{
		//check if the current char is within the valid ascii values for uppercase letters (ascii 65-90) or is a space char
		//(also valid for this assignment per the instructions)
		asciiValue = inputString[k];
		//if current char is outside of ascii range for uppercase letters AND is not a space char, return false
		if((asciiValue < 65 || asciiValue > 90) && (asciiValue != 32)){
			return FALSE;
		}
	}
	//if all chars in the string were valid uppercase letters or space chars, return true
	return TRUE;
}

/*
NAME
stringmatch
SYNOPSIS
checks if two strings match each other
DESCRIPTION
receives two strings as parameters. uses strcmp to compare the two strings. if they match,
returns true. else returns false. created this function since can be confusing how strcmp()
returns 0 if strings match instead of 1 (whereas everywhere else in this program, 0 is
FALSE and 1 is TRUE). so decided to create my own function. adapted from my own work for
OSU cs 344 assignment 3 3/3/19
*/
int StringMatch(char *string1, char *string2){
	//use strcmp to see if string one and two are equal. if equal, strcmp will return 0.
	//strcmp will return non-zero if the two strings are not equal.
	if(strcmp(string1, string2) == 0){
		//if the two strings are a match, return true
		return TRUE;
	}
	//if the two strings are not a match, return false
	return FALSE;
}

/*
NAME
getfilecontents
SYNOPSIS
takes text input from a file and puts the text into a string
DESCRIPTION
receives two string parameters, one file name and one a string buffer for the file's text contents to be put into.
file is opened (and checked for errors opening the file), text contents are read in and put into the buffer string.
fgets adds a newline char, so this is removed from the buffer string. file is then closed.
*/
void GetFileContents(char *fileName, char *buffer){
	//fopen use adapted from my own work for OSU cs344 assignment 2 10/25/18

	//set up file descriptor and open with fopen. "r" option is used because this
	//file should be only read, not written to.
	FILE *textFile;
	textFile = fopen(fileName, "r");

	//check that fopen() did not have an error (will return null if there was an error). 
	if(textFile == NULL){
		//if the input file wasn't able to be successfully opened w fopen(), print error
		//message to stderr and exit w non zero exit status
		Error("error! could not open your input text file correctly\n", 1);
	}

	//use fgets() to get the entire text from the textfile (MAX_BUF size so all text
	//contents are read in). puts this text input into the buffer string. check to make sure
	//that fgets() doesn't have an error (returning NULL would indicate an error)
	if(fgets(buffer, MAX_BUF, textFile) == NULL){
		//if fgets() gave an error, print error message to stderr and exit w non zero exit status
		Error("error, could not open your input text file correctly\n", 1);
	}	

	//fgets() adds a newline, if string is more than just a newline char, strip off the newline char
	//check that the buffer string isn't just a newline char (as this would create an error
	//when the newline char gets stripped)
	if(StringMatch(buffer, "\n") == FALSE){
		//if string had more than a newline char, replace the newline char with a null terminator
		buffer[strcspn(buffer, "\n")] = '\0';
	}
	//else string only contains newline char for some reason, removing newline as above would give error 
	else{ 
		//if there was only a newline char in the string, close the file and print error message to
		//stderr, exit w non zero exit status
		fclose(textFile);
		Error("error, your input text file only contains a newline char\n", 1);
	}
	//if ifle was opened successfully and file contents were read in correctly and the resulting string
	//contained more than a newline char, can now close the text file as all data has been successfully obtained.
	fclose(textFile);
}

/*
NAME
isvalidlength
SYNOPSIS
checks if the size of one string is at least as long as the other one
DESCRIPTION
receives two string parameters, one composed of text file contents (like plaintext or ciphertext) and the
other composed of contents from the key file. gets lengths of both w strlen() and then checks to make
sure that the key buffer length is at least as long as the plaintext or ciphertext length. returns true
if the key is at least that long, false otherwise.
*/
int IsValidLength(char *textFileIn, char *keyBufIn){
	//get string lengths of both the text file contents string (cipher or plain) and the key file contents string
	int textLength = strlen(textFileIn);
	int keyLength = strlen(keyBufIn);

	//if the key length is less than the text length (plain or cipher), can't use this key due to length problem, return false
	if(keyLength < textLength){
		return FALSE;
	}
	//if key length was at least as long as the text (cipher or plain) length, return true
	return TRUE;
}

/*
NAME
separateencbuffers
SYNOPSIS
separates a combined plaintext + key buffer into separate plaintext and key strings
DESCRIPTION
receives three string parameters, a combined string buffer, plain buffer, and key buffer.
uses strtok and the special plaintext and keytext delimiters to parse through the combined
buffer and separate the plaintext portion out and put it into the plaintext buffer and separate
the key portion out and put it into the key text buffer using strcpy
*/
void SeparateEncBuffers(char *combinedBuf, char *plainBuf, char *keyBuf){
	//strtok use adapted from my own work in OSU cs 344 Assignment 3 smallsh 3/3/19

	//create token string
	char *token;

	//use strtok to go through the combined buffer until plaintext special chars are reached. token
	//will then contain the entire plaintext string
	token = strtok(combinedBuf, PLAIN_SPECIAL_CHARS);

	//copy the token (i.e. full plaintext string) into the plaintext buffer w strcpy
	strcpy(plainBuf, token);

	//use strtok to go through the remaining combined buffer until key special chars are reached. token will
	//then contain the entire key text string
	token = strtok(NULL, KEY_SPECIAL_CHARS);

	//copy the token (i.e. the full key text string) into the keytext buffer w strcpy
	strcpy(keyBuf, token);
}

/*
NAME
separatedecbuffers
SYNOPSIS
separates a combined ciphertext + key buffer into separate ciphertext and key strings
DESCRIPTION
receives three string parameters, a combined string buffer, cipher buffer, and key buffer.
uses strtok and the special ciphertext and key text delimiters to parse through the combined
buffer and separate the ciphertext poriton out and put it into the ciphertext buffer and
separate the key portion out and put it into the key text buffer using strcpy
*/
void SeparateDecBuffers(char *combinedBuf, char *cipherBuf, char *keyBuf){
	//strtok use adapted from my own work in OSU cs 344 Assignment 3 smallsh 3/3/19

	//create token string
	char *token;

	//use strtok to go through the combined buffer until ciphertext special chars are reached. token
	//will then contain the entire ciphertext string.
	token = strtok(combinedBuf, CIPHER_SPECIAL_CHARS);

	//copy the token (i.e. the full ciphertext string) into the ciphertext buffer w strcpy
	strcpy(cipherBuf, token);

	//use strtok to go through the remaining combined buffer until key special chars are reached. token
	//will then contain the entire key text string
	token = strtok(NULL, KEY_SPECIAL_CHARS);

	//copy the token (i.e. the full key text string) into the keytext buffer w strcpy
	strcpy(keyBuf, token);
}

/*
NAME
isvalidconnection
SYNOPSIS
checks if the string contains the special chars that would indicate a valid connection
DESCRIPTION
receives two strings in, one string buffer containing the message to be searched, and another
that contains the special characters that, if found in the message being searched, will indicate
that a valid connection was made (i.e. that otp_enc connected to otp_enc_d and not otp_dec_d and that
otp_dec connected to otp_dec_d and not otp_enc_d). if the special characters indicating a valid connection
are found using strstr, the connection is valid so function returns true. if the special chars indicating
a valid connection are not found, will return false.
*/
int IsValidConnection(char *bufferIn, char *specialChars){

	//if string of special chars we're searching for in the message buffer to indicate a valid connection is not found, return false (since connection isn't valid)
	if(strstr(bufferIn, specialChars) == NULL){
		return FALSE;
	}
	//else if string of special chars we're searching for in the message buffer to indicate a valid connection is found, return true (since connection is valid)
	return TRUE;
}

/*
NAME
encrypt
SYNOPSIS
encrypts a given plaintext string into a ciphertext string using a given key string
DESCRIPTION
takes in a plaintext string, key string, and ciphertext string. creates array of valid
chars. for the length of the plaintext string, will loop through and check the current
plaintext char and key char, seeing what char the plaintext and key chars are and then
setting them to their corresponding index in the valid chars array, and then using modular
addition on the indices of the current plaintext and key chars to find what the current
ciphertext char should be. the ciphertext char calculated after modular addition will then
be put into the ciphertext string, until the end of the plaintext string at which point
all plaintext chars will have been converted into their corresponding ciphertext chars using
the provided key.
*/
void Encrypt(char *completePlaintextIn, char *completeKeyIn, char *cipherTextIn){

	//create array of the valid chars (valid chars are all uppercase letters plus
	//the space char per the assignment instructions)
	static char validChars[NUM_CHARS] = { 'A', 'B', 'C', 'D', 'E', 'F', 
			'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
			'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' };

	//create variables for the current plaintext char, index of the current plaintext char in 
	//the valid chars array, current key char, index of the current key char in the valid chars array,
	//current char (a temp char variable), and the index of the current cipher char
	char curPlainChar;
	int curPlainCharIdx;
	char curKeyChar;
	int curKeyCharIdx;
	char curChar;
	int cipherCharIdx;

	//find the length of the plaintext string using strlen
	int plainLength = strlen(completePlaintextIn);

	//create counter variables for the two for-loops
	int k, m;

	//loop through each char for the length of the plaintext string
	for(k = 0; k < plainLength; k++){
		//set the current plaintext char to a temp plaintext char variable
		curPlainChar = completePlaintextIn[k];

		//set the current key char to a temp key char varaible
		curKeyChar = completeKeyIn[k];

		//loop through twice (since we want to get the current char of both the
		//plaintext string as well as the key string). so when m == 0, we're getting the
		//current char of the plaintext string, and when m == 1, we're getting the current
		//char of the key string
		for(m = 0; m < 2; m++){
			//if m == 0, set temp curChar variable to the temp current plaintext char variable
			if(m == 0){
				curChar = curPlainChar;
			}
			//else if m == 1, set temp curChar variable to the temp current key char variable
			else{ //m == 1
				curChar = curKeyChar;
			}

			//use switch statement to check what valid char the current temp char (either plain char
			//or key char) is and what it's corresponding index in the valid chars array should be. set
			//the plaintext char idx or key char idx to the appropriate index integer for that char in
			//the valid chars array as appropriate.
			switch(curChar){
				case 'A':
					if(m == 0){ //if m == 0, we're checking plaintext chars
						//if cur char is 'A' and we're checking plaintext chars, set plain idx to 0
						curPlainCharIdx = 0;
					}
					else{ //else if m == 1, we're checking key chars
						//if cur char is 'A' and we're checking key chars, set key idx to 0
						curKeyCharIdx = 0;
					}	
					break;
				case 'B':
					if(m == 0){
						//if cur char is 'B' and we're checking plaintext chars, set plain idx to 1
						curPlainCharIdx = 1;
					}
					else{
						//if cur char is 'B' and we're checking key chars, set key idx to 1
						curKeyCharIdx = 1;
					}
					break;
				case 'C':
					if(m == 0){
						//if cur char is 'C' and we're checking plaintext chars, set plain idx to 2
						curPlainCharIdx = 2;
					}
					else{
						//if cur char is 'C' and we're checking key chars, set key idx to 2
						curKeyCharIdx = 2;
					}
					break;
				case 'D':
					if(m == 0){
						//if cur char is 'D' and we're checking plaintext chars, set plain idx to 3
						curPlainCharIdx = 3;
					}
					else{
						//if cur char is 'D' and we're checking key chars, set key idx to 3
						curKeyCharIdx = 3;
					}
					break;
				case 'E':
					if(m == 0){
						//if cur char is 'E' and we're checking plaintext chars, set plain idx to 4
						curPlainCharIdx = 4;
					}
					else{
						//if cur char is 'E' and we're checking key chars, set key idx to 4
						curKeyCharIdx = 4;
					}
					break;
				case 'F':
					//if cur char is 'F' and we're checking plaintext chars, set plain idx to 5
					if(m == 0){
						curPlainCharIdx = 5;
					}
					else{
						//if cur char is 'F' and we're checking key chars, set key idx to 5
						curKeyCharIdx = 5;
					}
					break;
				case 'G':
					if(m == 0){
						//if cur char is 'G' and we're checking plaintext chars, set plain idx to 6
						curPlainCharIdx = 6;
					}
					else{
						//if cur char is 'G' and we're checking key chars, set key idx to 6
						curKeyCharIdx = 6;
					}
					break;
				case 'H':
					if(m == 0){
						//if cur char is 'H' and we're checking plaintext chars, set plain idx to 7
						curPlainCharIdx = 7;
					}
					else{
						//if cur char is 'H' and we're checking key chars, set key idx to 7
						curKeyCharIdx = 7;
					}
					break;
				case 'I':
					if(m == 0){
						//if cur char is 'I' and we're checking plaintext chars, set plain idx to 8
						curPlainCharIdx = 8;
					}
					else{
						//if cur char is 'I' and we're checking key chars, set key idx to 8
						curKeyCharIdx = 8;
					}
					break;
				case 'J':
					if(m == 0){
						//if cur char is 'J' and we're checking plaintext chars, set plain idx to 9
						curPlainCharIdx = 9;
					}
					else{
						//if cur char is 'J' and we're checking key chars, set key idx to 9
						curKeyCharIdx = 9;
					}			
					break;
				case 'K':
					if(m == 0){
						//if cur char is 'K' and we're checking plaintext chars, set plain idx to 10
						curPlainCharIdx = 10;
					}
					else{
						//if cur char is 'K' and we're checking key chars, set key idx to 10
						curKeyCharIdx = 10;
					}
					break;
				case 'L':
					if(m == 0){
						//if cur char is 'L' and we're checking plaintext chars, set plain idx to 11
						curPlainCharIdx = 11;
					}
					else{
						//if cur char is 'L' and we're checking key chars, set key idx to 11
						curKeyCharIdx = 11;
					}
					break;
				case 'M':
					if(m == 0){
						//if cur char is 'M' and we're checking plaintext chars, set plain idx to 12
						curPlainCharIdx = 12;
					}
					else{
						//if cur char is 'M' and we're checking key chars, set key idx to 12
						curKeyCharIdx = 12;
					}
					break;
				case 'N':
					if(m == 0){
						//if cur char is 'N' and we're checking plaintext chars, set plain idx to 13
						curPlainCharIdx = 13;
					}
					else{
						//if cur char is 'N' and we're checking key chars, set key idx to 13
						curKeyCharIdx = 13;
					}
					break;
				case 'O':
					if(m == 0){
						//if cur char is 'O' and we're checking plaintext chars, set plain idx to 14
						curPlainCharIdx = 14;
					}
					else{
						//if cur char is 'O' and we're checking key chars, set key idx to 14
						curKeyCharIdx = 14;
					}
					break;
				case 'P':
					if(m == 0){
						//if cur char is 'P' and we're checking plaintext chars, set plain idx to 15
						curPlainCharIdx = 15;
					}
					else{
						//if cur char is 'P' and we're checking key chars, set key idx to 15
						curKeyCharIdx = 15;
					}
					break;
				case 'Q':
					if(m == 0){
						//if cur char is 'Q' and we're checking plaintext chars, set plain idx to 16
						curPlainCharIdx = 16;
					}
					else{
						//if cur char is 'Q' and we're checking key chars, set key idx to 16
						curKeyCharIdx = 16;
					}
					break;
				case 'R':
					if(m == 0){
						//if cur char is 'R' and we're checking plaintext chars, set plain idx to 17
						curPlainCharIdx = 17;
					}
					else{
						//if cur char is 'R' and we're checking key chars, set key idx to 17
						curKeyCharIdx = 17;
					}
					break;
				case 'S':
					if(m == 0){
						//if cur char is 'S' and we're checking plaintext chars, set plain idx to 18
						curPlainCharIdx = 18;
					}
					else{
						//if cur char is 'S' and we're checking key chars, set key idx to 18
						curKeyCharIdx = 18;
					}
					break;
				case 'T':
					if(m == 0){
						//if cur char is 'T' and we're checking plaintext chars, set plain idx to 19
						curPlainCharIdx = 19;
					}
					else{
						//if cur char is 'T' and we're checking key chars, set key idx to 19
						curKeyCharIdx = 19;
					}
					break;
				case 'U':
					if(m == 0){
						//if cur char is 'U' and we're checking plaintext chars, set plain idx to 20
						curPlainCharIdx = 20;
					}
					else{
						//if cur char is 'U' and we're checking key chars, set key idx to 20
						curKeyCharIdx = 20;
					}
					break;
				case 'V':
					if(m == 0){
						//if cur char is 'V' and we're checking plaintext chars, set plain idx to 21
						curPlainCharIdx = 21;
					}
					else{
						//if cur char is 'V' and we're checking key chars, set key idx to 21
						curKeyCharIdx = 21;
					}
					break;
				case 'W':
					if(m == 0){
						//if cur char is 'W' and we're checking plaintext chars, set plain idx to 22
						curPlainCharIdx = 22;
					}
					else{
						//if cur char is 'W' and we're checking key chars, set key idx to 22
						curKeyCharIdx = 22;
					}
					break;
				case 'X':
					if(m == 0){
						//if cur char is 'X' and we're checking plaintext chars, set plain idx to 23
						curPlainCharIdx = 23;
					}
					else{
						//if cur char is 'X' and we're checking key chars, set key idx to 23
						curKeyCharIdx = 23;
					}
					break;
				case 'Y':
					if(m == 0){
						//if cur char is 'Y' and we're checking plaintext chars, set plain idx to 24
						curPlainCharIdx = 24;
					}
					else{
						//if cur char is 'Y' and we're checking key chars, set key idx to 24
						curKeyCharIdx = 24;
					}
					break;
				case 'Z':
					if(m == 0){
						//if cur char is 'Z' and we're checking plaintext chars, set plain idx to 25
						curPlainCharIdx = 25;
					}
					else{
						//if cur char is 'Z' and we're checking key chars, set key idx to 25
						curKeyCharIdx = 25;
					}
					break;
				case ' ':
					if(m == 0){
						//if cur char is ' ' and we're checking plaintext chars, set plain idx to 26
						curPlainCharIdx = 26;
					}
					else{
						//if cur char is ' ' and we're checking key chars, set key idx to 26
						curKeyCharIdx = 26;
					}
					break;
				default:
					//else if the current plain or key char doesn't match any valid chars, is a major error
					//(since char validity was checked prior to the calling of this function), print error
					//message to stderr and exit w non zero exit value
					Error("encryption error! one of your chars is not valid!\n", 1);
			}
		}

		//use modular addition to find current cipher text index (for the valid char array w indices 0-26) from the
		//current plaintext index and key index. 

		//if adding the current plaintext char idx and cur key char idx is greater than the number of possible
		//valid char array chars (27, indices 0-26), subtract 27 from the sum of the plainchar idx and key idx to
		//get the correct cipher char idx
		if((curPlainCharIdx + curKeyCharIdx) >= 27){
			cipherCharIdx = curPlainCharIdx + curKeyCharIdx - 27;
		}
		//else if adding the cur plaintext char idx and cur key char idx isn't > largets idx (26), don't subtract anything
		else{
			cipherCharIdx = curPlainCharIdx + curKeyCharIdx;
		}
		
		//set the current char in the ciphertext string to the valid char corresponding to the cipherchar index
		//as previously calculated through modular addition
		cipherTextIn[k] = validChars[cipherCharIdx];
	}
}

/*
NAME
decrypt
SYNOPSIS
decrypts a given ciphertext string int a plaintext string using a given key string
DESCRIPTION
takes in a ciphertext string, key string, and plaintext string. creates array of valid
chars. for the length of the ciphertext string, will loop through and check the current
ciphertext char and key char, seeing what char the ciphertext and key chars are and then
setting them to their corresponding index in the valid chars array, and then using modular
subtraction on the indices of the current ciphertext and key chars to find what the current
plaintext char should be. the plaintext char calculated after modular subtraction will then
be put into the plaintext string, until the end of the ciphertext string at which point
all ciphertext chars will have been converted into their corresponding plaintext chars using
the provided key.
*/
void Decrypt(char *cipherTextIn, char *keyTextIn, char *plainTextIn){
	//create array of the valid chars (valid chars are all uppercase letters plus
	//the space char per the assignment instructions)
	static char validChars[NUM_CHARS] = { 'A', 'B', 'C', 'D', 'E', 'F', 
			'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
			'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' };

	//create variables for the current ciphertext char, index of the current ciphertext char in 
	//the valid chars array, current key char, index of the current key char in the valid chars array,
	//current char (a temp char variable), and the index of the current plain char
	char curCipherChar;
	int curCipherCharIdx;
	char curKeyChar;
	int curKeyCharIdx;
	char curChar;
	int plainCharIdx;

	//find the length of the ciphertext string using strlen
	int cipherLength = strlen(cipherTextIn);

	//create counter variables for the two for-loops
	int k, m;

	//loop through each char for the length of the ciphertext string
	for(k = 0; k < cipherLength; k++){
		//set the current ciphertext char to a temp ciphertext char variable
		curCipherChar = cipherTextIn[k];

		//set the current key char to a temp key char variable
		curKeyChar = keyTextIn[k];

		//loop through twice (since we want to get the current char of both the
		//ciphertext string as well as the key string). so when m == 0, we're getting the
		//current char of the ciphertext string, and when m == 1, we're getting the current
		//char of the key string
		for(m = 0; m < 2; m++){
			//if m == 0, set temp curChar variable to the temp current ciphertext char variable
			if(m == 0){
				curChar = curCipherChar;
			}
			//else if m == 1, set temp curChar variable to the temp current key char variable
			else{ //m == 1
				curChar = curKeyChar;
			}
			//use switch statement to check what valid char the current temp char (either cipher char
			//or key char) is and what it's corresponding index in the valid chars array should be. set
			//the ciphertext char idx or key char idx to the appropriate index integer for that char in
			//the valid chars array as appropriate.
			switch(curChar){
				case 'A':
					if(m == 0){ //if m == 0, we're checking ciphertext chars
						//if cur char is 'A' and we're checking ciphertext chars, set cipher idx to 0
						curCipherCharIdx = 0;
					}
					else{ //else if m == 1, we're checking key chars
						//if cur char is 'A' and we're checking key chars, set key idx to 0
						curKeyCharIdx = 0;
					}	
					break;
				case 'B':
					if(m == 0){
						//if cur char is 'B' and we're checking ciphertext chars, set cipher idx to 1
						curCipherCharIdx = 1;
					}
					else{
						//if cur char is 'B' and we're checking key chars, set key idx to 1
						curKeyCharIdx = 1;
					}
					break;
				case 'C':
					if(m == 0){
						//if cur char is 'C' and we're checking ciphertext chars, set cipher idx to 2
						curCipherCharIdx = 2;
					}
					else{
						//if cur char is 'C' and we're checking key chars, set key idx to 2
						curKeyCharIdx = 2;
					}
					break;
				case 'D':
					if(m == 0){
						//if cur char is 'D' and we're checking ciphertext chars, set cipher idx to 3
						curCipherCharIdx = 3;
					}
					else{
						//if cur char is 'D' and we're checking key chars, set key idx to 3
						curKeyCharIdx = 3;
					}
					break;
				case 'E':
					if(m == 0){
						//if cur char is 'E' and we're checking ciphertext chars, set cipher idx to 4
						curCipherCharIdx = 4;
					}
					else{
						//if cur char is 'E' and we're checking key chars, set key idx to 4
						curKeyCharIdx = 4;
					}
					break;
				case 'F':
					if(m == 0){
						//if cur char is 'F' and we're checking ciphertext chars, set cipher idx to 5
						curCipherCharIdx = 5;
					}
					else{
						//if cur char is 'F' and we're checking key chars, set key idx to 5
						curKeyCharIdx = 5;
					}
					break;
				case 'G':
					if(m == 0){
						//if cur char is 'G' and we're checking ciphertext chars, set cipher idx to 6
						curCipherCharIdx = 6;
					}
					else{
						//if cur char is 'G' and we're checking key chars, set key idx to 6
						curKeyCharIdx = 6;
					}
					break;
				case 'H':
					if(m == 0){
						//if cur char is 'H' and we're checking ciphertext chars, set cipher idx to 7
						curCipherCharIdx = 7;
					}
					else{
						//if cur char is 'H' and we're checking key chars, set key idx to 7
						curKeyCharIdx = 7;
					}
					break;
				case 'I':
					if(m == 0){
						//if cur char is 'I' and we're checking ciphertext chars, set cipher idx to 8
						curCipherCharIdx = 8;
					}
					else{
						//if cur char is 'I' and we're checking key chars, set key idx to 8
						curKeyCharIdx = 8;
					}
					break;
				case 'J':
					if(m == 0){
						//if cur char is 'J' and we're checking ciphertext chars, set cipher idx to 9
						curCipherCharIdx = 9;
					}
					else{
						//if cur char is 'J' and we're checking key chars, set key idx to 9
						curKeyCharIdx = 9;
					}			
					break;
				case 'K':
					if(m == 0){
						//if cur char is 'K' and we're checking ciphertext chars, set cipher idx to 10
						curCipherCharIdx = 10;
					}
					else{
						//if cur char is 'K' and we're checking key chars, set key idx to 10
						curKeyCharIdx = 10;
					}
					break;
				case 'L':
					if(m == 0){
						//if cur char is 'L' and we're checking ciphertext chars, set cipher idx to 11
						curCipherCharIdx = 11;
					}
					else{
						//if cur char is 'L' and we're checking key chars, set key idx to 11
						curKeyCharIdx = 11;
					}
					break;
				case 'M':
					if(m == 0){
						//if cur char is 'M' and we're checking ciphertext chars, set cipher idx to 12
						curCipherCharIdx = 12;
					}
					else{
						//if cur char is 'M' and we're checking key chars, set key idx to 12
						curKeyCharIdx = 12;
					}
					break;
				case 'N':
					if(m == 0){
						//if cur char is 'N' and we're checking ciphertext chars, set cipher idx to 13
						curCipherCharIdx = 13;
					}
					else{
						//if cur char is 'N' and we're checking key chars, set key idx to 13
						curKeyCharIdx = 13;
					}
					break;
				case 'O':
					if(m == 0){
						//if cur char is 'O' and we're checking ciphertext chars, set cipher idx to 14
						curCipherCharIdx = 14;
					}
					else{
						//if cur char is 'O' and we're checking key chars, set key idx to 14
						curKeyCharIdx = 14;
					}
					break;
				case 'P':
					if(m == 0){
						//if cur char is 'P' and we're checking ciphertext chars, set cipher idx to 15
						curCipherCharIdx = 15;
					}
					else{
						//if cur char is 'P' and we're checking key chars, set key idx to 15
						curKeyCharIdx = 15;
					}
					break;
				case 'Q':
					if(m == 0){
						//if cur char is 'Q' and we're checking ciphertext chars, set cipher idx to 16
						curCipherCharIdx = 16;
					}
					else{
						//if cur char is 'Q' and we're checking key chars, set key idx to 16
						curKeyCharIdx = 16;
					}
					break;
				case 'R':
					if(m == 0){
						//if cur char is 'R' and we're checking ciphertext chars, set cipher idx to 17
						curCipherCharIdx = 17;
					}
					else{
						//if cur char is 'R' and we're checking key chars, set key idx to 17
						curKeyCharIdx = 17;
					}
					break;
				case 'S':
					if(m == 0){
						//if cur char is 'S' and we're checking ciphertext chars, set cipher idx to 18
						curCipherCharIdx = 18;
					}
					else{
						//if cur char is 'S' and we're checking key chars, set key idx to 18
						curKeyCharIdx = 18;
					}
					break;
				case 'T':
					if(m == 0){
						//if cur char is 'T' and we're checking ciphertext chars, set cipher idx to 19
						curCipherCharIdx = 19;
					}
					else{
						//if cur char is 'T' and we're checking key chars, set key idx to 19
						curKeyCharIdx = 19;
					}
					break;
				case 'U':
					if(m == 0){
						//if cur char is 'U' and we're checking ciphertext chars, set cipher idx to 20
						curCipherCharIdx = 20;
					}
					else{
						//if cur char is 'U' and we're checking key chars, set key idx to 20
						curKeyCharIdx = 20;
					}
					break;
				case 'V':
					if(m == 0){
						//if cur char is 'V' and we're checking ciphertext chars, set cipher idx to 21
						curCipherCharIdx = 21;
					}
					else{
						//if cur char is 'V' and we're checking key chars, set key idx to 21
						curKeyCharIdx = 21;
					}
					break;
				case 'W':
					if(m == 0){
						//if cur char is 'W' and we're checking ciphertext chars, set cipher idx to 22
						curCipherCharIdx = 22;
					}
					else{
						//if cur char is 'W' and we're checking key chars, set key idx to 22
						curKeyCharIdx = 22;
					}
					break;
				case 'X':
					if(m == 0){
						//if cur char is 'X' and we're checking ciphertext chars, set cipher idx to 23
						curCipherCharIdx = 23;
					}
					else{
						//if cur char is 'X' and we're checking key chars, set key idx to 23
						curKeyCharIdx = 23;
					}
					break;
				case 'Y':
					if(m == 0){
						//if cur char is 'Y' and we're checking ciphertext chars, set cipher idx to 24
						curCipherCharIdx = 24;
					}
					else{
						//if cur char is 'Y' and we're checking key chars, set key idx to 24
						curKeyCharIdx = 24;
					}
					break;
				case 'Z':
					if(m == 0){
						//if cur char is 'Z' and we're checking ciphertext chars, set cipher idx to 25
						curCipherCharIdx = 25;
					}
					else{
						//if cur char is 'Z' and we're checking key chars, set key idx to 25
						curKeyCharIdx = 25;
					}
					break;
				case ' ':
					if(m == 0){
						//if cur char is ' ' and we're checking ciphertext chars, set cipher idx to 26
						curCipherCharIdx = 26;
					}
					else{
						//if cur char is ' ' and we're checking key chars, set key idx to 26
						curKeyCharIdx = 26;
					}
					break;
				default:
					//else if the current cipher or key char doesn't match any valid chars, is a major error
					//(since char validity was checked prior to the calling of this function), print error
					//message to stderr and exit w non zero exit value
					Error("decryption error! one of your chars is not valid!\n", 1);
			}
		}

		//use modular subtraction to find current plaintext index (for the valid char array w indices 0-26) from the
		//current ciphertext index and key index

		//if difference after subtracting the key index from the ciphertext index is negative (i.e. out of range of the valid
		//char array indices which start at 0), add 27 to the difference to get the correct plaintext char idx
		if((curCipherCharIdx - curKeyCharIdx) < 0){
			plainCharIdx = curCipherCharIdx - curKeyCharIdx + 27;
		}
		//else if subtracting the key char idx from the cipher char idx is not <0, don't add anything
		else{
			plainCharIdx = curCipherCharIdx - curKeyCharIdx;
		}

		//set the current char in the plaintext string to the valid char corresponding to the plainchar index
		//as previously calculated through modular addition
		plainTextIn[k] = validChars[plainCharIdx];
	}
}
