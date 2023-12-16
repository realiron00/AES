#include "AES.h"

// use that x^8 = x^4 + x^3 + x + 1 in GF(2^8)
/*****************************************************************************************
 * Xtime : multiplication by x in the finite field GF(2^8)
 * 
 * fx : byte to be multiplied by x
 * 
 * return : fx * x
 *****************************************************************************************/
byte Xtime(byte fx) 
{
	return (((fx >> 7) & 0x01) == 1 ? (fx << 1) ^ 0x1b : fx << 1);
	// if x^7 exists in f(x), multiply x to make x^8 and then change x^8 to x^4 + x^3 + x + 1
    // if x^7 does not exist in f(x), just multiply x
}

/*****************************************************************************************
 * Mul : multiplication in the finite field GF(2^8)
 * 
 * fx : byte to be multiplied
 * gx : byte to be multiplied
 * 
 * return : fx * gx
 *****************************************************************************************/
byte Mul(byte fx, byte gx) 
{
	byte z; // z = f(x) * g(x)
	int coef; // 0 or 1
	z = 0x00;

	//f(x)=a7(x^7)+a6(x^6)+...+a0
	for (int i = 7; i >= 0; i--) {
		coef = (fx >> i) & 0x01; // a7, a6, a5, ... ,a0
		/*
		f(x)*g(x)
		= g(x)*a0 + g(x)*a1*x + g(x)*a2*x^2 + g(x)*a3*x^3 + ... + g(x)*a7*x^7
		= g(x)*a0 + x*( g(x)*a1 + g(x)*a2*x + g(x)*a3*x^2 + ... +g(x)*a7*x^6 )
		= g(x)*a0 + x*( g(x)*a1 + x*( g(x)*a2 + g(x)*a3*x + ... +g(x)*a7*x^5)  )
		= g(x)*a0 + x*( ...   + x*(g(x)*a7) )  )
		*/
		z = Xtime(z);
		if (coef == 1) {
			z = z ^ gx;
		}
	}
	return z;
}

/*****************************************************************************************
 * AddRoundKey : XOR the state with the round key
 * 
 * state : state to be XORed
 * roundkey : round key to be XORed
 *****************************************************************************************/
void AddRoundKey(byte* state, byte* roundkey)
{
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundkey[i];	// xor state and roundkey
	}
}

/*****************************************************************************************
 * SubByte : substitute each byte of the state by the corresponding byte in the S-box
 * 
 * state : state to be substituted
 *****************************************************************************************/
void SubByte(byte* state)
{
	for (int i = 0; i < 16; i++) {
		state[i] = Sbox[state[i]];	// substitute each byte of the state by the corresponding byte in the S-box
	}
}

/*****************************************************************************************
 * InvSubByte : substitute each byte of the state by the corresponding byte in the inverse S-box
 * 
 * state : state to be substituted
 *****************************************************************************************/
void InvSubByte(byte* state)
{
	for (int i = 0; i < 16; i++) {
		state[i] = InvSbox[state[i]];	// substitute each byte of the state by the corresponding byte in the inverse S-box
	}
}

/*****************************************************************************************
 * ShiftRow : shift the rows of the state
 * 
 * state : state to be shifted
 *****************************************************************************************/
void ShiftRow(byte* state)
{
	byte x[16];

	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12	 => s0	s4	s8	s12
	s1	s5	s9	s13		s5	s9	s13	s1
	s2	s6	s10	s14		s10	s14	s2	s6
	s3	s7	s11	s15		s15	s3	s7	s11
	*/

	x[0] = state[0];
	x[1] = state[5];
	x[2] = state[10];
	x[3] = state[15];

	x[4] = state[4];
	x[5] = state[9];
	x[6] = state[14];
	x[7] = state[3];

	x[8] = state[8];
	x[9] = state[13];
	x[10] = state[2];
	x[11] = state[7];

	x[12] = state[12];
	x[13] = state[1];
	x[14] = state[6];
	x[15] = state[11];
	// receive the array that shiftrowed to x

	for (int i = 0; i < 16; i++) {
		state[i] = x[i];	// put the array that shiftrowed to state
	}

}

/*****************************************************************************************
 * InvShiftRow : shift the rows of the state in the inverse direction
 * 
 * state : state to be shifted
 *****************************************************************************************/
void InvShiftRow(byte* state)
{
	byte x[16];

	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12	 => s0	s4	s8	s12
	s1	s5	s9	s13		s13	s1	s5	s9
	s2	s6	s10	s14		s10	s14	s2	s6
	s3	s7	s11	s15		s7	s11	s15	s3
	*/

	x[0] = state[0];
	x[1] = state[13];
	x[2] = state[10];
	x[3] = state[7];

	x[4] = state[4];
	x[5] = state[1];
	x[6] = state[14];
	x[7] = state[11];

	x[8] = state[8];
	x[9] = state[5];
	x[10] = state[2];
	x[11] = state[15];

	x[12] = state[12];
	x[13] = state[9];
	x[14] = state[6];
	x[15] = state[3];

	for (int i = 0; i < 16; i++) {
		state[i] = x[i];
	}
}

/*****************************************************************************************
 * MixColumn : mix the columns of the state
 * 
 * state : state to be mixed
 *****************************************************************************************/
void MixColumn(byte* state)
{
	byte MixCol[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01},
					  {0x01, 0x01, 0x02, 0x03} , {0x03, 0x01, 0x01, 0x02} };
	byte x[4][4];
	byte temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			x[i][j] = state[i * 4 + j];
		}
	} // turn the state into a 4x4 matrix
	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12
	s1	s5	s9	s13	
	s2	s6	s10	s14
	s3	s7	s11	s15
	*/

	for (int i = 0; i < 4; i++) {
		/* state[i]=s[i], MixCol[i][j]=m[i][j]
		s[i * 4 + j] += m[j][k] * x[i][k](k=0~k=3)
		*/
		for (int j = 0; j < 4; j++) {
			temp = 0x00; // temp=s[i * 4 + j]
			for (int k = 0; k < 4; k++) {
				temp ^= Mul(x[i][k], MixCol[j][k]); // Add the products of m[j][k] and x[i][k] to temp(XOR)
			}
			state[i * 4 + j] = temp;
		}
	}
}

/*****************************************************************************************
 * InvMixColumn : mix the columns of the state in the inverse direction
 * 
 * state : state to be mixed
 *****************************************************************************************/
void InvMixColumn(byte* state)
{
	byte InvMixCol[4][4] = { {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d},
					  {0x0d, 0x09, 0x0e, 0x0b} , {0x0b, 0x0d, 0x09, 0x0e}};

	byte x[4][4];
	byte temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			x[i][j] = state[i * 4 + j];
		}
	}

	for (int i = 0; i < 4; i++) {
		/* state[i]=s[i], InvMixCol[i][j]=im[i][j]
		s[i * 4 + j] += im[j][k] * x[i][k](k=0~k=3)
		*/
		for (int j = 0; j < 4; j++) {
			temp = 0x00; // temp=s[i * 4 + j]
			for (int k = 0; k < 4; k++) {
				temp ^= Mul(x[i][k], InvMixCol[j][k]); // Add the products of im[j][k] and x[i][k] to temp(XOR)
			}
			state[i * 4 + j] = temp;
		}
	}
}

/*****************************************************************************************
 * KeySchedule : generate the round keys
 * 
 * Key : key to be scheduled
 * rk : round keys
 *****************************************************************************************/
void KeySchedule(byte Key[16], byte rk[11][16])
{
	for (int i = 0; i < 16; i++) {
		rk[0][i] = Key[i];
	}

	byte x[4];

	for (int i = 1; i < 11; i++) {
		x[0] = rk[i-1][13];
		x[1] = rk[i - 1][14];
		x[2] = rk[i - 1][15];
		x[3] = rk[i - 1][12];

		for (int j = 0; j < 4; j++) {
			x[j] = Sbox[x[j]];
		}

		rk[i][0] = rk[i - 1][0] ^ x[0] ^ rcon[i-1];
		rk[i][1] = rk[i - 1][1] ^ x[1];
		rk[i][2] = rk[i - 1][2] ^ x[2];
		rk[i][3] = rk[i - 1][3] ^ x[3];

		for (int j = 4; j < 16; j++)
		{
			rk[i][j] = rk[i - 1][j] ^ rk[i][j - 4];
		}
	}
}

/*****************************************************************************************
 * Dec_KeySchedule : generate the round keys in the inverse direction
 * 
 * Key : key to be scheduled
 * rk : round keys
 ******************************************************************************************/
void Dec_KeySchedule(byte Key[16], byte rk[11][16])
{
	byte erk[11][16];

	KeySchedule(Key, erk);

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 16; j++) {
			rk[i][j] = erk[10 - i][j];
		}
	}
}

/*****************************************************************************************
 * AES_Encrypt : encrypt the plaintext
 * 
 * PT : plaintext to be encrypted
 * RK : round keys
 * CT : ciphertext
 *****************************************************************************************/
void AES_Encrypt(byte* PT, byte RK[11][16], byte* CT)
{
	byte state[16];

	// round 0
	for (int i = 0; i < 16; i++) {
		state[i] = PT[i]; // input the plaintext to the state
	}

	AddRoundKey(state, RK[0]);
	// round 0: use the first round key, execute addroundkey once

	for (int i = 1; i <= 9; i++) { // round 1~9
		SubByte(state);
		ShiftRow(state);
		MixColumn(state);
		AddRoundKey(state, RK[i]);
		// round 1~9: subbytes, shiftrows, mixcolumns, addroundkey
	}

	// round 10
	SubByte(state);
	ShiftRow(state);
	AddRoundKey(state, RK[10]);
	// round 10: subbytes, shiftrows, addroundkey

	for (int i = 0; i < 16; i++) {
		CT[i] = state[i]; // output the ciphertext
	}
}

/*****************************************************************************************
 * AES_Decrypt : decrypt the ciphertext
 * 
 * CT : ciphertext to be decrypted
 * RK : round keys
 * PT : plaintext
 *****************************************************************************************/
void AES_Decrypt(byte* CT, byte RK[11][16], byte* PT)
{
	byte state[16];

	// round 0
	for (int i = 0; i < 16; i++) {
		state[i] = CT[i]; // input the ciphertext to the state
	}
	AddRoundKey(state, RK[10]);
	// round 0: use the last round key, execute addroundkey once

	for (int i = 9; i > 0; i--) { // round 1~9
		InvShiftRow(state);
		InvSubByte(state);
		AddRoundKey(state, RK[i]);
		InvMixColumn(state);
		// round 1~9: inverse shiftrows, inverse subbytes, addroundkey, inverse mixcolumns
	}

	// round 10
	InvShiftRow(state);
	InvSubByte(state);
	// The round key used in encryption's 0th round is the original key value, received as is
	AddRoundKey(state, RK[0]);
	// round 10: inverse shiftrows, inverse subbytes, addroundkey
	
	for (int i = 0; i < 16; i++) {
		PT[i] = state[i]; // output the plaintext
	}
}