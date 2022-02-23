#define _CRT_SECURE_NO_WARNINGS
#include "midiStates.h"

void v_Hdr(MTHD_hdr hd, MTRK_hdr *rk, DATA* data[100]) {
	printf("Header: ");
	for (char i = 0; i != 4; i++)
		printf("%c", hd.mthd[i]);
	printf("\n");
	int temp = 0;
	int tracks = 0;
	conv_Bytes(&temp, hd.chunklength, 4);
	printf("Chunk length: %i\n", temp);
	conv_Bytes(&temp, hd.format, 2);
	printf("Format: %i\n", temp);
	conv_Bytes(&temp, hd.tracks, 2);
	printf("Tracks: %i\n", temp);
	tracks = temp;
	conv_Bytes(&temp, hd.perQuarterNote, 2, 1);
	printf("%i per quarter note\n", (int)temp);
	for (int lx = 0; lx != tracks; lx++) {
		printf("\n\n");

		printf("Header: ");
		for (char i = 0; i != 4; i++)
			printf("%c", rk[lx].mthd[i]);
		printf("\n");
		conv_Bytes(&temp, rk[lx].chunklength, 4);
		printf("Chunk length: %i\n", temp);
		int datSize = temp;
		if (data[lx] != NULL) {
			printf("Data:... \n");
			int len = 0;
			for (int globalLength = 0; globalLength < datSize; globalLength++) {
				printf("%03i_", globalLength + 1);
				printf("%02x\n", data[lx][globalLength].data & 0b11111111);
				bool we = 0;
				if (ifItsEvent(data[lx][globalLength].data)) {
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
					/*if (v_note(data[lx][globalLength].data & 0b11111111)) {
						printf(" , ");
						we = 1;
					}
					if (v_instrument(data[lx][globalLength].data & 0b11111111) || we == 1)
						printf("\n");
					*/
				}
				_decodDat(NULL, NULL, NULL,NULL);
			}
		}
	}
}

void do_things(const char* fileName) {
	ifItsEvent(1, 1);
	printf("\n\nFile name: \"%s\"\n", fileName);
	MTHD_hdr wavHeader;
	MTRK_hdr mtrk[100];
	DATA* data[100];
	int headerSize[3] = { sizeof(MTHD_hdr) , sizeof(MTRK_hdr) };
	int tracksSize[100] = { 0 };
	FILE* f = NULL;
	int tracks;
	FILE* fout = NULL;
	f = fopen(fileName, "r");
	if (f == NULL) {
		printf("can't open\n");
		exit(0);
	}
	fread(&wavHeader, headerSize[0], 1, f);
	conv_Bytes(&tracks, wavHeader.tracks, 2);
	for (int i = 0; tracks != i; i++) {
		globalElem = i;
		fread(&mtrk[i], headerSize[1], 1, f);
		conv_Bytes(&tracksSize[i], mtrk[i].chunklength, 4);
		generalData[i] = (DATA*)malloc(tracksSize[i] * sizeof(DATA)*5);
		if (generalData[i] != NULL) {
			for (int ii = 0; ii != tracksSize[i]; ii++) {
				fread(&(generalData[i][ii]), sizeof(DATA), 1, f);
			}
			//printf("FINN_Fread!!\n");
		}
	}


	v_Hdr(wavHeader, mtrk, generalData);

	fclose(f);
	/*for (int i = 0; i != 100; i++) {
		if(generalData[i] != NULL)
		free(generalData[i]);

	}*/
}
int main() {
	do_things("Midis\\alle_meine_entchen.mid");
	printf("\n\n");
	do_things("Midis\\_alle_meine_entchen.mid");
	//do_things("Midis\\Super Mario Land - Birabuto Kingdom.mid");

	printf("FIN...\n\nPress \'s\' to Exit.\n\n");
	//while (_getch() != 'c');
	return 0;
}