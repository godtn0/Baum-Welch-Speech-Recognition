#ifndef TRANSITION_H
#define TRANSITION_H
#include "typeHeader.h"
#include <vector>
#define _CRT_SECURE_NO_WARNINGS
#define N_DIMENSION 39
#define N_PDF 2
#define N_STATE 3
#define MAX_TRANSCRIPT_NUM 40
#define MAX_TIME 600
#define MAX_INPUT_PHONES 40
#define N_PHONES 21
#define MAX_STATES_NUM 100


void makeTransition(inputData X);
void printIntransition(inputData X);
void printOuttransition(inputData X);

#endif
