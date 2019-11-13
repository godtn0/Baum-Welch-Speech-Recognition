#include "typeHeader.h"

void makeTranscript(inputData X) {
	for (int i = 1; i < X.numOfPhones + 1; i++) {
		for (int j = 0; j < N_PHONES; j++) {
			if (!strcmp(X.phonesSequence[i - 1], phonelist[j].name)) {
				transcript[i] = &phonelist[j];
			}
		}
	}
}

void makeLogScale(inputData X) {
	vector<TransitionType>::iterator trans;
	for (int p = 1; p < X.numOfPhones + 1; p++) {
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			for (trans = outtransition[p][s].begin(); trans != outtransition[p][s].end(); trans++) {
				trans->prob = log(trans->prob);
			}
			for (trans = intransition[p][s].begin(); trans != intransition[p][s].end(); trans++) {
				trans->prob = log(trans->prob);
			}
		}
	}
}

double gaussian(pdfType pdf, double x[]) {
	double result = 0;
	for (int i = 0; i < 39; i++) {
		result -= log(pdf.var[i]) / 2.0 + (x[i] - pdf.mean[i])*(x[i] - pdf.mean[i]) / (2 * pdf.var[i]);
	}
	return result;
}

double logSum(double p[], int size) {
	if (size == 0) {
		return 0;
	}
	double max = -999999999999999;
	for (int i = 0; i < size; i++) {
		if (p[i] > max) {
			max = p[i];
		}
	}
	double result = max;
	double l = 0.0;
	for (int i = 0; i < size; i++) {
		l += exp(p[i] - max);
	}

	result += log(l);
	return result;
}

double mixGaussian(stateType state, double x[]) {
	double v1 = log(state.pdf[0].weight) + gaussian(state.pdf[0], x);
	double v2 = log(state.pdf[1].weight) + gaussian(state.pdf[1], x);
	//printf("%f, %f\n", v1, v2);
	double p[2] = { v1,v2 };
	//printf("logSum : %f\n", logSum(p, 2));
	return logSum(p, 2);
}

double partGaussian(stateType state, double x[], int gaussianNum) {
	return log(state.pdf[gaussianNum].weight) + gaussian(state.pdf[gaussianNum], x);
}


