#define _CRT_SECURE_NO_WARNINGS
#include "midiStates.h"

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
	printf("%i per quarter note\n", (int)temp);
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
		int len = 0;
		for (int globalLength = 0; globalLength< datSize; globalLength++) {
			printf("%03i_", globalLength + 1);
			printf("%02x\n", data[globalLength].data & 0b11111111);
			bool we = 0;
			if (ifItsEvent(data[globalLength].data)) {
				printf("An Event has occourd: ");
				//for (int i = 0; i != *ev_len+1;i++) {
					//printf("%02x ",event[i]&0xff);
				//}
				identivyEvent();
				printf("\n");
			}
			else {
				//printf("\n\t");
				we = 0;
				if (v_note(data[globalLength].data & 0b11111111)) {
					printf(" , ");
					we = 1;
				}
				if(v_instrument(data[globalLength].data & 0b11111111)||we==1)
				printf("\n");
			}
		}
	}
}

void do_things(const char*fileName) {
	ifItsEvent(1, 1);
	printf("File name: \"%s\"\n",fileName);
	MTHD_hdr wavHeader;
	MTRK_hdr mtrk;
	DATA* data;
	int headerSize[3] = { sizeof(MTHD_hdr) , sizeof(MTRK_hdr) };
	FILE* f = NULL;
	FILE* fout = NULL;
	f = fopen(fileName, "r");
	if (f == NULL) {
		printf("can't open\n");
		exit(0);
	}
	fread(&wavHeader, headerSize[0], 1, f);
	fread(&mtrk, headerSize[1], 1, f);
	conv_Bytes(&headerSize[2], mtrk.chunklength, 4);
	generalData = (DATA*)malloc(headerSize[2] * sizeof(DATA));
	if (generalData != NULL) {
		for (int i = 0; i != headerSize[2] - 10; i++) {
			fread(&generalData[i], sizeof(DATA), 1, f);
		}
		//printf("FINN_Fread!!\n");
	}
	v_Hdr(wavHeader, mtrk, generalData);

	fclose(f);
	free(generalData);
}
int main() {
	do_things("alle_meine_entchen.mid");
	printf("\n\n");
	do_things("_alle_meine_entchen.mid");
	while (_getch() == 'c');
	return 0;
}