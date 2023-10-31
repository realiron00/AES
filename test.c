#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "AES.h"

typedef unsigned char byte;

int main()
{
	//테스트벡터
	byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
					0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	//테스트 평문
	byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
					0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	//테스트 암호 키
	byte ct[16];
    byte rk[11][16];

    KeySchedule(key, rk);

	//테스트 암호문
	AES_Encrypt(pt, rk, ct); //AES 암호화 실행

	printf("CT = ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", ct[i]);
	}
	printf("\n"); //암호문 출력

	printf("==============================================================\n");

	byte npt[16];
	AES_Decrypt(ct, rk, npt); //AES 복호화 실행

	printf("PT = ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", npt[i]);
	}
	printf("\n"); //평문 출력
}