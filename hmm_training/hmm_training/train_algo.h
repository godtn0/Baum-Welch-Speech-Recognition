#ifndef TRAIN_ALGO_H
#define TRAIN_ALGO_H
#include "typeHeader.h"

void makeObservationPro(inputData X);
void forward(inputData X);
void backward(inputData X);
void acc(inputData X);
void baum_welch();

#endif // !TRAIN_ALGO_H
