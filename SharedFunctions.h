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

//include guards (to make sure things don't get defined twice, will check if is defined and
//if not will define FUNCTIONS_H)
#ifndef FUNCIONS_H
#define FUNCTIONS_H

//include all other needed header files
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/ioctl.h>

//macro definitions

//define true and false since raw C has no bools
#define TRUE 1
#define FALSE 0

//per assignment 4 instructions, use localhost as the address
#define HOST_ADDRESS "localhost"

//num chars are alphabet plus space char (so 26 + 1, which is 27)
#define NUM_CHARS 27

//indices for valid chars are thus from 0 to num of valid chars - 1 (which is 26)
#define MIN_INDEX 0
#define MAX_INDEX 26

//assignment 4 instructions say program only has to be capable of working with the
//plaintext files as assigned, max plaintext is ~70000 chars, so set the max buf
//to a num slightly larger than this, 100000 chars
#define MAX_BUF 100000

//to make sure all data is received, am adding special chars to tell the program
//when all data has been stopped. rather than remembering which special chars go with
//which type of text, created macros to make identifying each one simpler
#define PLAIN_SPECIAL_CHARS "@"
#define KEY_SPECIAL_CHARS "#"
#define CIPHER_SPECIAL_CHARS "&"

//to prevent bad connections (i.e. otp_enc trying to connect to otp_dec_d or otp_dec
//trying to connect to otp_enc_d), also have special chars to tell the program which
//client has tried to connect to which server. again, rather than remembering which
//special chars go with which type of text, created macros to make identifying each one
//simpler
#define ENC_SPECIAL_CHARS "%"
#define DEC_SPECIAL_CHARS "!"

//have bad connect text to help identify when a bad connection was attempted (i.e. otp_enc trying
//to connect to otp_dec_d or otp_dec trying to connect to otp_enc_d ). won't have a problem with
//the programs thinking this is valid text since there's an underscore (and only valid chars are
//capital letters plus the space char) so that edge case won't be a problem.
#define BAD_CONNECT "BAD_CONNECT"

//set max number of forks to prevent fork bombing
#define MAX_FORKS 5

//function declarations
void Error(const char *, int);
int IntInputValidation(char *);
int CharInputValidation(char *);
int StringMatch(char *, char *);
void GetFileContents(char *, char*);
int IsValidLength(char *, char *); 
void SeparateEncBuffers(char *, char *, char *);
void SeparateDecBuffers(char *, char *, char *);
int IsValidConnection(char *, char *);
void Encrypt(char *, char *, char *); 
void Decrypt(char *, char *, char *);

#endif
