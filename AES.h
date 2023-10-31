#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef unsigned char byte;

/*****************************************************************************************
 * Xtime : multiplication by x in the finite field GF(2^8)
 * 
 * fx : byte to be multiplied by x
 * 
 * return : fx * x
 *****************************************************************************************/
byte Xtime(byte fx);

/*****************************************************************************************
 * Mul : multiplication in the finite field GF(2^8)
 * 
 * fx : byte to be multiplied
 * gx : byte to be multiplied
 * 
 * return : fx * gx
 *****************************************************************************************/
byte Mul(byte fx, byte gx);

/*****************************************************************************************
 * AddRoundKey : XOR the state with the round key
 * 
 * state : state to be XORed
 * roundkey : round key to be XORed
 *****************************************************************************************/
void AddRoundKey(byte* state, byte* roundkey);

/*****************************************************************************************
 * SubByte : substitute each byte of the state by the corresponding byte in the S-box
 * 
 * state : state to be substituted
 *****************************************************************************************/
void SubByte(byte* state);

/*****************************************************************************************
 * InvSubByte : substitute each byte of the state by the corresponding byte in the inverse S-box
 * 
 * state : state to be substituted
 *****************************************************************************************/
void InvSubByte(byte* state);

/*****************************************************************************************
 * ShiftRow : shift the rows of the state
 * 
 * state : state to be shifted
 *****************************************************************************************/
void ShiftRow(byte* state);

/*****************************************************************************************
 * InvShiftRow : shift the rows of the state in the inverse direction
 * 
 * state : state to be shifted
 *****************************************************************************************/
void InvShiftRow(byte* state);

/*****************************************************************************************
 * MixColumn : mix the columns of the state
 * 
 * state : state to be mixed
 *****************************************************************************************/
void MixColumn(byte* state);

/*****************************************************************************************
 * InvMixColumn : mix the columns of the state in the inverse direction
 * 
 * state : state to be mixed
 *****************************************************************************************/
void InvMixColumn(byte* state);

/*****************************************************************************************
 * KeySchedule : generate the round keys
 * 
 * Key : key to be scheduled
 * rk : round keys
 *****************************************************************************************/
void KeySchedule(byte Key[16], byte rk[11][16]);

/*****************************************************************************************
 * AES_Encrypt : encrypt the plaintext
 * 
 * PT : plaintext to be encrypted
 * RK : round keys
 * CT : ciphertext
 *****************************************************************************************/
void AES_Encrypt(byte* PT, byte RK[11][16], byte* CT);

/*****************************************************************************************
 * AES_Decrypt : decrypt the ciphertext
 * 
 * CT : ciphertext to be decrypted
 * RK : round keys
 * PT : plaintext
 *****************************************************************************************/
void AES_Decrypt(byte* CT, byte RK[11][16], byte* PT);