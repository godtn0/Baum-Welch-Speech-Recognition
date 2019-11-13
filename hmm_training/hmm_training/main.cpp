#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "transition.h"
#include "typeHeader.h"
#include "hmm_reader.h"
#include "train.h"

using namespace std;

void main() {
	printf("start\n");
	readHmm();
	readTrainingData();
	writeHmm();
}