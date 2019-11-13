#include <string.h>
#include "file_util.h"


void changeTotxt(char * str) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '.') {
			str[i + 1] = 't';
			str[i + 2] = 'x';
			str[i + 3] = 't';
			str[i + 4] = 0;
			break;
		}
	}
}

void removeQuatation(char * str, int length) {
	for (int i = 0; i < length - 1; i++) {
		str[i] = str[i + 1];
	}
	str[length - 2] = 0;
}
