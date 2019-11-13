#include "typeHeader.h"
#include "transition.h"

vector<TransitionType> intransition[MAX_INPUT_PHONES + 2][N_STATE];
vector<TransitionType> outtransition[MAX_INPUT_PHONES + 2][N_STATE];


Phone * transcript[MAX_TRANSCRIPT_NUM + 2];

void makeTransition(inputData X) {
	TransitionType t = { 0, };
	//printf("----------out transition-----------\n");
	for (int p = 1; p < X.numOfPhones; p++) {  // before last phone
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			for (int d = 0; d < transcript[p]->nOfState - 2; d++) {  // in phone
				t.prob = transcript[p]->tp[s + 1][d + 1];
				t.dest_phone = p;
				t.dest_state = d;
				if (t.prob > 0) {
					outtransition[p][s].push_back(t);
				}
			}
			for (int d = 0; d < transcript[p + 1]->nOfState - 2; d++) {  // phone to next phone
				t.prob = transcript[p]->tp[s + 1][transcript[p]->nOfState - 1] * transcript[p + 1]->tp[0][d + 1];
				t.dest_phone = p + 1;
				t.dest_state = d;
				if (t.prob > 0) {
					outtransition[p][s].push_back(t);
				}
			}
			if (transcript[p + 1]->indexOfPhone == 17) {  //  next phone "sp" can be skip
				t.dest_phone = p + 2;
				t.dest_state = 0;
				t.prob = transcript[p]->tp[s + 1][transcript[p]->nOfState - 1] * transcript[p + 2]->tp[0][1] * phonelist[17].tp[0][2];
				if (t.prob > 0) {
					outtransition[p][s].push_back(t);
				}
			}
		}
	}
	for (int s = 0; s < transcript[X.numOfPhones]->nOfState - 2; s++) {  // in last phone
		for (int d = 0; d < transcript[X.numOfPhones]->nOfState - 2; d++) {
			t.prob = transcript[X.numOfPhones]->tp[s + 1][d + 1];
			t.dest_phone = X.numOfPhones;
			t.dest_state = d;
			if (t.prob > 0) {
				outtransition[X.numOfPhones][s].push_back(t);
			}
		}
	}
	t.dest_phone = X.numOfPhones + 1;
	t.dest_state = 0;
	t.prob = transcript[X.numOfPhones]->tp[transcript[X.numOfPhones]->nOfState - 2][transcript[X.numOfPhones]->nOfState - 1];
	if (t.prob > 0)
		outtransition[X.numOfPhones][transcript[X.numOfPhones]->nOfState - 3].push_back(t);

	//printf("\n-------------in transition-------------\n");
	for (int p = 2; p < X.numOfPhones + 1; p++) {  // after first phone
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			for (int d = 0; d < transcript[p]->nOfState - 2; d++) {  // in phone
				t.prob = transcript[p]->tp[d + 1][s + 1];
				t.dest_phone = p;
				t.dest_state = d;
				if (t.prob > 0) {
					intransition[p][s].push_back(t);
				}
			}
			for (int d = 0; d < transcript[p - 1]->nOfState - 2; d++) {  // from previous phone
				t.prob = transcript[p - 1]->tp[d + 1][transcript[p - 1]->nOfState - 1] * transcript[p]->tp[0][s + 1];
				t.dest_phone = p - 1;
				t.dest_state = d;
				if (t.prob > 0) {
					intransition[p][s].push_back(t);
				}
			}
			if (transcript[p - 1]->indexOfPhone == 17) {  // previous phone is "sp" skip.
				t.dest_phone = p - 2;
				t.dest_state = transcript[p - 2]->nOfState - 3;
				t.prob = transcript[p - 2]->tp[transcript[p-2]->nOfState-2][transcript[p-2]->nOfState - 1] * transcript[p]->tp[0][s + 1] * phonelist[17].tp[0][2];
				if (t.prob > 0)  intransition[p][s].push_back(t);
			}
		}
	}
	for (int s = 0; s < transcript[1]->nOfState - 2; s++) {  // first phone
		for (int d = 0; d < transcript[1]->nOfState - 2; d++) {
			t.prob = transcript[1]->tp[d + 1][s + 1];
			t.dest_phone = 1;
			t.dest_state = d;
			if (t.prob > 0) {
				intransition[1][s].push_back(t);
			}
		}
	}
	t.prob = transcript[1]->tp[0][1];
	t.dest_phone = 0;
	t.dest_state = 0;
	if (t.prob > 0) intransition[1][0].push_back(t);
}

// print all of transitions both in and out

void printIntransition(inputData X) {
	printf("---------intransition---------\n");
	vector<TransitionType>::iterator trans;
	int n = 0;
	for (int p = 1; p < X.numOfPhones + 1; p++) {
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			for (trans = intransition[p][s].begin(); trans != intransition[p][s].end(); trans++) {
				printf("%d phone %d state to %d phone %d state : %f\n", trans->dest_phone, trans->dest_state, p, s, trans->prob);
				n++;
			}
		}
	}
	printf("\n");
	printf("%d °³\n", n);
}
void printOuttransition(inputData X) {
	printf("---------outtransition--------\n");
	vector<TransitionType>::iterator trans;
	int n = 0;
	for (int p = 1; p < X.numOfPhones + 1; p++) {
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			for (trans = outtransition[p][s].begin(); trans != outtransition[p][s].end(); trans++) {
				printf("%d phone %d state to %d phone %d state : %f\n", p, s, trans->dest_phone, trans->dest_state, trans->prob);
				n++;
			}
		}
	}
	printf("%d °³\n", n);
	printf("\n");
}
