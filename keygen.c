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
main function. takes in key length arg, creates key chars of that length.
DESCRIPTION
takes in key length arg from user. converts it from char to int.
uses srand and rand to get random index and select a random char from
the valid chars allowed (all upper case letters plus the space char). adds them
to a string, after selecting the right num of valid chars to add to the string, will print
the string plus a newline char at the end to stdout.
*/
int main(int argc, char *argv[])
{
	//create string of max size to hold key chars and memset to all null terminator
	char keyBuf[MAX_BUF];
	memset(keyBuf, '\0', sizeof(keyBuf));
	
	//check the number of args from user (should be two, one that's the program name ("keygen",
	//and the other should be the keyLength)
	if (argc != 2)
	{
		//if wrong num of args, print error message to stderr and exit w/ non-zero exit value (1)
		Error("error! invalid number of arguments. arg should be the keygen executable name and the  key length\n", 1);
	}
	
	//convert length (argv[1]) passed in from string to int
	//using atoi to convert from string to int in c adapted from:
	//https://www.quora.com/How-do-I-convert-character-value-to-integer-value-in-c-language
	int keyLength = atoi(argv[1]);

	//check if user entered 0, negative number, or non-number for the key length. keyLength == 0 means
	//user entered key length of 0, or that atoi failed (atoi also returns 0 if it had an error). or if 
	//IntInputValidation is false, (i.e. if the user enters a negative number or non-numeric input),
	//print error message and exit w non zero exit status.
	if((keyLength == 0) || (IntInputValidation(argv[1]) == FALSE))
	{
		//if user entered invalid key length, print error message to stderr and exit w non-zero exit value (1)
		Error( "error! invalid key length. you entered a 0, negative number, or non-number as the key length\n", 1);
	}

	//create array of the allowed chars (all upper case alphabet letters plus 
	//the space character to be used as a pool to pull randomly from, per the
	//assignment instructions).
	static char validChars[NUM_CHARS] = { 'A', 'B', 'C', 'D', 'E', 'F', 
			'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
			'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' };

	//seed srand one time, then use rand() with mod later to get rand nums
	//random number formula used after seeding:
	//randNum = (rand() % (maxRandNum - minRandNum + 1) + minRandNum)
	//method for obtaining random numbers adapted from my own work (10/25/18) and
	//https:stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
	srand(time(NULL));
	int randIndex;
	int k;

	//loop number of times equal to the length the key is supposed to be as set by the user,
	//get rand valid characters for each array index and add to the string until the string is
	//of the user's specified keyLength
	for(k = 0; k < keyLength; k++)
	{
		//get random index (used to get random char from the array of valid chars).
		//formula for getting random number between a min and max adapted from my own
		//OSU CS 162 Winter 2017 course notes
		randIndex = (rand() % (MAX_INDEX - MIN_INDEX + 1) + MIN_INDEX);
		
		//add the random char to the keyBuf string at the current index
		keyBuf[k] = validChars[randIndex];
	}

	//output the key string to stdout and flush. also output newline as last char per the instructions
	printf("%s\n", keyBuf); fflush(stdout);

	return 0;
}
