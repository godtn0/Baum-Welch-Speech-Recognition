#ifndef TYPEHEADER_H
#define TYPEHEADER_H
#include <math.h>
#include <vector>
#include < stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define N_DIMENSION 39
#define N_PDF 2
#define N_STATE 3
#define MAX_TRANSCRIPT_NUM 40
#define MAX_TIME 600
#define MAX_INPUT_PHONES 40
#define N_PHONES 21
#define MAX_STATES_NUM 100

using namespace std;

typedef struct {
	double weight;
	double mean[N_DIMENSION];
	double var[N_DIMENSION];
} pdfType;
typedef struct {
	pdfType pdf[N_PDF];
} stateType;
typedef struct {
	int nOfState;
	int indexOfPhone;
	char name[5];
	double tp[N_STATE + 2][N_STATE + 2];
	stateType state[N_STATE];
}Phone;

typedef struct {
	double x[MAX_TIME][N_DIMENSION];
	char phonesSequence[MAX_INPUT_PHONES][10];
	int T, numOfPhones;
}inputData;

typedef struct {
	double alpha[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
	int assigned[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
}Alpha;
typedef struct {
	double beta[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
	int assigned[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
}Beta;

typedef struct {
	int dest_phone;
	int dest_state;
	double prob;
}TransitionType;

extern vector<TransitionType> intransition[MAX_INPUT_PHONES + 2][N_STATE];
extern vector<TransitionType> outtransition[MAX_INPUT_PHONES + 2][N_STATE];



extern Phone phonelist[21];

extern Phone * transcript[MAX_TRANSCRIPT_NUM + 2];

void makeTranscript(inputData X);
void makeLogScale(inputData X);
double gaussian(pdfType pdf, double x[]);
double logSum(double p[], int size);
double mixGaussian(stateType state, double x[]);
double partGaussian(stateType state, double x[], int gaussianNum);
//
extern Alpha a;
extern Beta b;

//extern double gamma[MAX_INPUT_PHONES][N_STATE][MAX_TIME] = { 0, };
//extern double gammaSum[N_PHONES][N_STATE] = { 0, };
//extern double gammaGaussian[MAX_INPUT_PHONES][N_STATE][2][MAX_TIME] = { 0, };
//extern double gammaGaussianSum[N_PHONES][N_STATE][2] = { 0, };
//extern int assigned[N_PHONES][N_STATE][MAX_TIME] = { 0, };
//extern vector<TransitionType> ksi[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
////vector<TransitionType> ksiSum[MAX_INPUT_PHONES][N_STATE];
//extern double ksiSum[N_PHONES][N_STATE][N_PHONES][N_STATE];
//extern double gaussianMean[N_PHONES][N_STATE][2][39];
//extern double gaussianVar[N_PHONES][N_STATE][2][39];

void initAllParameter(inputData X);

#endif