#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "AES.h"

typedef unsigned char byte;

/*****************************************************************************************
 * FileSize: return the size of the file
 * 
 * fname: the name of the file
 * 
 * return: the size of the file
 *****************************************************************************************/
int FileSize(const char* fname);

/*****************************************************************************************
 * File_Padding: add padding to the file
 * 
 * input: the name of the input file
 * output: the name of the output file
 *****************************************************************************************/
void File_Padding(const char* input, const char* output);

/*****************************************************************************************
 * erase_padding: erase the padding of the file
 * 
 * pad_name: the name of the file with padding
 * origin_name: the name of the file without padding
 *****************************************************************************************/
void erase_padding(const char* pad_name, const char* origin_name);

/*****************************************************************************************
 * file_AES_CBC: encrypt the file with AES CBC mode
 * 
 * pt_name: the name of the plaintext file
 * IV: the initial vector
 * Key: the key
 * ct_name: the name of the ciphertext file
 *****************************************************************************************/
void file_AES_CBC(const char *pt_name, byte IV[16], byte Key[16], const char *ct_name);

/*****************************************************************************************
 * Inv_file_AES_CBC: decrypt the file with AES CBC mode
 * 
 * ct_name: the name of the ciphertext file
 * IV: the initial vector
 * Key: the key
 * pt_name: the name of the plaintext file
 *****************************************************************************************/
void Inv_file_AES_CBC(const char *ct_name, byte IV[16], byte Key[16], const char *pt_name);