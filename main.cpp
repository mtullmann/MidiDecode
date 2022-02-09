#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#define BETWEEN(over,x,under) ((over<=x)&&(x<=under))

using namespace std;

struct MTHD_hdr {
	char mthd[4];
	char chunklength[4];
	char format[2];
	char tracks[2];
	char perQuarterNote[2];
};
struct MTRK_hdr {
	char mthd[4];
	char chunklength[4];
};
struct DATA {
	char data;
};
char* _inst(char nr) {
	static char all15I[][56] = {
		"Piano",
		"Chromatic Percussion",
		"Organ",
		"Guitar",
		"Bass",
		"Strings",
		"Ensemble",
		"Brass",
		"Reed",
		"Pipe",
		"Synth Lead",
		"Synth Pad",
		"Synth Effects",
		"Ethnic",
		"Percussive",
		"Sound Effects",
		"NaN"
	};
	if (nr > 128 || nr < 1)
		return all15I[17];
	return all15I[nr / 8];
}
void _not(char inp, char* note, char* octNr) {
	*note = inp % 12;
	switch (*note) {
	case 0: strcpy(note, "C"); break;
	case 1: strcpy(note, "C#"); break;
	case 2: strcpy(note, "D"); break;
	case 3: strcpy(note, "D#"); break;
	case 4: strcpy(note, "E"); break;
	case 5: strcpy(note, "F"); break;
	case 6: strcpy(note, "F#"); break;
	case 7: strcpy(note, "G"); break;
	case 8: strcpy(note, "G#"); break;
	case 9: strcpy(note, "A"); break;
	case 10: strcpy(note, "A#"); break;
	case 11: strcpy(note, "H"); break;
	default:strcpy(note, "NaN"); break;
	}
	*octNr = (inp / 12) - 1;
}
void v_note(char inp) {
	char note[3];
	char octNr;
	_not(inp, note, &octNr);
	printf("Note: %s%2i\n", note, octNr);
}
void conv_Bytes(int* out, char* in, int size, bool msb_flp = 0) {
	*out = 0;
	for (int i = 0; i != size; i++) {
		*out += (in[(!msb_flp) ? size - i - 1 : i] << (i * 8));
		//printf("\t%i=(in[%i - %i - 1 = %i] = %i >>(%i));\n", *out, size, i, size- i-1, in[size - i - 1],8 * i);
	}
}
void v_Hdr(MTHD_hdr hd, MTRK_hdr rk, DATA* data) {
	printf("Header: ");
	for (char i = 0; i != 4; i++)
		printf("%c", hd.mthd[i]);
	printf("\n");
	int temp = 0;
	conv_Bytes(&temp, hd.chunklength, 4);
	printf("Chunk length: %i\n", temp);
	conv_Bytes(&temp, hd.format, 2);
	printf("Format: %i\n", temp);
	conv_Bytes(&temp, hd.tracks, 2);
	printf("Tracks: %i\n", temp);
	conv_Bytes(&temp, hd.perQuarterNote, 2, 1);
	printf("%i per quarter note\n", temp);
	printf("\n\n");

	printf("Header: ");
	for (char i = 0; i != 4; i++)
		printf("%c", rk.mthd[i]);
	printf("\n");
	conv_Bytes(&temp, rk.chunklength, 4);
	printf("Chunk length: %i\n", temp);
	int datSize = temp;
	if (data != NULL) {
		printf("Data:... \n");

		for (int i = 0; i != datSize; i++) {
			printf("%03i_", i + 1);
			printf("%02x\n", data[i].data&0b11111111);
		}
	}
}


int main() {

	char file[] = "alle_meine_entchen.mid";
	MTHD_hdr wavHeader;
	MTRK_hdr mtrk;
	DATA* data;
	int headerSize[3] = { sizeof(MTHD_hdr) , sizeof(MTRK_hdr) };
	FILE* f = NULL;
	FILE* fout = NULL;
	f = fopen(file, "r");
	if (f == NULL) {
		printf("can't open\n");
		exit(0);
	}
	fread(&wavHeader, headerSize[0], 1, f);
	fread(&mtrk, headerSize[1], 1, f);
	conv_Bytes(&headerSize[2], mtrk.chunklength, 4);
	data = (DATA*)malloc(headerSize[2] * sizeof(DATA));
	if (data != NULL) {
		for (int i = 0; i != headerSize[2] - 10; i++) {
			fread(&data[i], sizeof(DATA), 1, f);
		}
		printf("FINN_Fread!!\n");
	}
	v_Hdr(wavHeader, mtrk, data);

	_fcloseall();
	free(data);
	return 0;
}