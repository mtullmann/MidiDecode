#pragma once

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
		return all15I[16];
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
	char note[4];
	char octNr = 0;
	_not(inp, note, &octNr);
	if (strcmp(note, "NaN"))
	printf("Note: %s%i", note, octNr);
}
void v_instrument(char c) {
	if(strcmp(_inst(c),"NaN"))
	printf("Instrument group: \"%s\"", _inst(c));
}
void conv_Bytes(int* out, char* in, int size, bool msb_flp = 0) {
	*out = 0;
	for (int i = 0; i != size; i++) {
		*out += (((unsigned char)(0xff & in[(!msb_flp) ? size - i - 1 : i])) << (i * 8));
		//	printf("\t%i=(in[%i - %i - 1 = %i] = %02x >>(%i));\n", *out, size, i, size- i-1, 0xff&(in[size - i - 1]),8 * i);
	}
}



static char event[100];
int* ev_len = NULL;
DATA* generalData;
int globalLength = 0;
int identivyEvent() {
	int len = *ev_len + 1;
	switch (len) {
	case 2:
		if (event[0] == (char)0xff) {
			if (event[1] == (char)0x01) {
				printf("Text Event");
				return 2;
			}
			if (event[1] == (char)0x02) {
				printf("Copyright Notice");
				return 3;
			}
			if (event[1] == (char)0x03) {
				printf("Sequence/Track Name");
				return 4;
			}
			if (event[1] == (char)0x04) {
				printf("Instrument Name");
				return 5;
			}
			if (event[1] == (char)0x05) {
				printf("Lyric");
				return 6;
			}
			if (event[1] == (char)0x06) {
				printf("Marker");
				return 7;
			}
			if (event[1] == (char)0x07) {
				printf("Cue Point");
				return 8;
			}
			if (event[1] == (char)0x7F) {
				printf("Sequencer Specific Meta-Event");
				return 15;
			}
		}
		break;
	case 3:
		if (event[0] == (char)0xff) {
			if (event[1] == 0x01 && event[2] == 0x01) {
				printf("Sequence Number");
				return 1;
			}
			if (event[1] == (char)0x20 && event[2] == (char)0x01) {
				printf("MIDI Channel Prefix");
				return 9;
			}
			if (event[1] == (char)0x2F && event[2] == (char)0x00) {
				printf("End of Track");
				return 10;
			}
			if (event[1] == (char)0x51 && event[2] == (char)0x03) {
				printf("Set Tempo (in microseconds per MIDI quarter-note)");
				return 11;
			}
			if (event[1] == (char)0x54 && event[2] == (char)0x05) {
				printf("SMPTE Offset");
				return 12;
			}
			if (event[1] == (char)0x58 && event[2] == (char)0x04) {
				printf("Time Signature");
				return 13;
			}
			if (event[1] == (char)0x59 && event[2] == (char)0x02) {
				printf("Key Signature");
				return 12;
			}
		}
		break;
	}
}
bool ifItsEvent(char inp, bool reset = 0) {
	static int startListen = 0;
	inp = inp & 0xff;
	if (reset)
		startListen = 0;
	ev_len = &startListen;
	if (startListen >= 5) {
		startListen = 0;
	}
	bool sec = 0;
	do{
		sec = 0;
	if (startListen != 0) {
		//printf("\t\tLISTEN: %02x NR: %i\n", inp, startListen);
		event[startListen] = inp & 0xff;
		event[startListen + 1] = 0;
		while (event[startListen] != inp);
		if (startListen == 1)
			switch (inp) {
			case (char)0x01:return 1;
			case (char)0x02:return 1;
			case (char)0x03:return 1;
			case (char)0x04:return 1;
			case (char)0x05:return 1;
			case (char)0x06:return 1;
			case (char)0x07:return 1;
			case (char)0x7F:return 1;
			}
		if (startListen == 2)
			switch (event[1]) {
			case (char)0x00:
				if (inp == (char)0x02)
					return 1;
				break;
			case (char)0x20:
				if (inp == (char)0x01)
					return 1;
				break;
			case (char)0x2F:
				if (inp == (char)0x00)
					return 1;
				break;
			case (char)0x51:
				if (inp == (char)0x03)
					return 1;
				break;
			case (char)0x54:
				if (inp == (char)0x05)
					return 1;
				break;
			case (char)0x58:
				if (inp == (char)0x04)
					return 1;
				break;
			case (char)0x59:
				if (inp == (char)0x02)
					return 1;
				break;
			}
		startListen++;
	}
	else {
		//printf("if %02x is ff\n", 0xff&inp);
		if (inp == (char)0xff) {
			//	printf("LINSTEN Activated:::\n");
				//while (1);
			startListen = 1;
			event[0] = inp;
		}
	}
}while (sec);
	return 0;
}
