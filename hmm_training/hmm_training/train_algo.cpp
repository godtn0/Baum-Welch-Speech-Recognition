#include "train_algo.h"
#include "typeHeader.h"

Alpha a;
Beta b;

double observationPro[N_PHONES][N_STATE][MAX_TIME];

void makeObservationPro(inputData X) {
	for (int p = 0; p < 21; p++) {
		for (int s = 0; s < phonelist[p].nOfState - 2; s++) {
			for (int t = 0; t < X.T; t++) {
				observationPro[p][s][t] = mixGaussian(phonelist[p].state[s], X.x[t]);
			}
		}
	}
}

void initObservationPro(inputData X) {
	for (int p = 0; p < 21; p++) {
		for (int s = 0; s < phonelist[p].nOfState - 2; s++) {
			for (int t = 0; t < X.T; t++) {
				observationPro[p][s][t] = 0;
			}
		}
	}
}

void forward(inputData X) {
	//printf("done\n");
	vector<TransitionType>::iterator trans;

	for (trans = intransition[1][0].begin(); trans != intransition[1][0].end(); trans++) {
		//printf("done\n");
		if (trans->dest_phone == 0) {
			a.alpha[1][0][0] = trans->prob + observationPro[transcript[1]->indexOfPhone][0][0];
		}
	}
	//printf("done\n");
	a.assigned[1][0][0] = 1;
	for (int t = 1; t < X.T; t++) {
		for (int p = 1; p < X.numOfPhones + 1; p++) {
			for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
				double tmp1[MAX_TIME] = { 0, };
				int n = 0;
				for (trans = intransition[p][s].begin(); trans != intransition[p][s].end(); trans++) {
					int dp = trans->dest_phone;
					int ds = trans->dest_state;
					if (a.assigned[dp][ds][t - 1] == 1) {
						tmp1[n] = a.alpha[dp][ds][t - 1] + trans->prob + observationPro[transcript[p]->indexOfPhone][s][t];
						a.assigned[p][s][t] = 1;
						n++;
					}
				}
				a.alpha[p][s][t] = logSum(tmp1, n);
				/*if (a.assigned[p][s][t]) {
				printf("alpha[%d][%d][%d] : %lf\n", p, s, t, a.alpha[p][s][t]);
				}*/
				/*
				if (a.alpha[p][s][t] > 0) {
				system("pause");
				}*/
			}
		}
	}
	//printf("alpha done\n");
}
void backward(inputData X) {
	vector<TransitionType>::iterator trans;
	int N = X.numOfPhones;
	int lastStaten = transcript[N]->nOfState - 3;
	for (trans = outtransition[N][lastStaten].begin(); trans != outtransition[N][lastStaten].end(); trans++) {
		if (trans->dest_phone == N + 1) {
			b.beta[N][lastStaten][X.T - 1] = trans->prob;
			//printf("backward test : %f \n", trans->prob);
		}
	}
	b.assigned[N][lastStaten][X.T - 1] = 1;
	for (int t = X.T - 2; t >= 0; t--) {
		for (int p = 1; p < N + 1; p++) {
			for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
				double tmp1[MAX_TIME] = { 0, };
				int n = 0;
				for (trans = outtransition[p][s].begin(); trans != outtransition[p][s].end(); trans++) {
					int dp = trans->dest_phone;
					int ds = trans->dest_state;
					if (b.assigned[dp][ds][t + 1] == 1 && dp < N + 1) {
						tmp1[n] = b.beta[dp][ds][t + 1] + trans->prob + observationPro[transcript[dp]->indexOfPhone][ds][t+1];
						b.assigned[p][s][t] = 1;
						n++;
					}
				}
				b.beta[p][s][t] = logSum(tmp1, n);
				/*if (b.assigned[p][s][t]) {
				printf("beta[%d][%d][%d] : %f\n", p, s, t, b.beta[p][s][t]);
				}*/
				//printf("mixGaussian : %f\n", mixGaussian(transcript[p]->state[s], X.x[t]));
				/*if (beta[p][s][t] > 0) {
				system("pause");
				}*/
			}
		}
	}
	//printf("beta done\n");
}
double gamma[MAX_INPUT_PHONES][N_STATE][MAX_TIME] = { 0, };
double gammaSum[N_PHONES][N_STATE] = { 0, };
double gammaGaussian[MAX_INPUT_PHONES][N_STATE][2][MAX_TIME] = { 0, };
double gammaGaussianSum[N_PHONES][N_STATE][2] = { 0, };
int assigned[MAX_INPUT_PHONES][N_STATE][MAX_TIME] = { 0, };
vector<TransitionType> ksi[MAX_INPUT_PHONES][N_STATE][MAX_TIME];
//vector<TransitionType> ksiSum[MAX_INPUT_PHONES][N_STATE];
double ksiSum[N_PHONES][N_STATE][N_PHONES][N_STATE];
double gaussianMean[N_PHONES][N_STATE][2][39];
double gaussianVar[N_PHONES][N_STATE][2][39];



void acc(inputData X) {
	forward(X);
	backward(X);
	double pro = 0;
	int N = X.numOfPhones;
	int lastStaten = transcript[X.numOfPhones]->nOfState - 3;
	pro = a.alpha[N][lastStaten][X.T - 1] + log(transcript[N]->tp[lastStaten + 1][lastStaten + 2]);
	//printf("pro : %lf, %lf, %lf\n", a.alpha[N][lastStaten][X.T - 1], log(transcript[N]->tp[lastStaten + 1][lastStaten + 2]), pro);

	for (int t = 0; t < X.T; t++) {
		double test = 0;
		for (int p = 1; p < N + 1; p++) {
			for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
				if (a.assigned[p][s][t] && b.assigned[p][s][t]) {
					gamma[p][s][t] = exp(a.alpha[p][s][t] + b.beta[p][s][t] - pro);
					if (p == 1 && s == 0 && t == 0) {
						gammaSum[transcript[p]->indexOfPhone][s] -= gamma[p][s][t];
					}
					gammaSum[transcript[p]->indexOfPhone][s] += gamma[p][s][t];
					test += exp(a.alpha[p][s][t] + b.beta[p][s][t] - pro);
					//printf("alpha : %f, beta : %f, p : %f\n", a.alpha[p][s][t], b.beta[p][s][t], pro);
					//printf("gamma[%d][%d][%d] : %lf\n", p, s, t, gamma[p][s][t]);
					assigned[p][s][t] = 1;
				}
			}
		}
		//printf("gamma sum =1 : %f\n", test);
	}
	// ksi part
	TransitionType tmp = { 0, };
	vector<TransitionType>::iterator trans;
	
	ksiSum[0][0][transcript[1]->indexOfPhone][0] = gamma[1][0][0];
	//printf("ksiSum of (0,0) -> (1,0) : %f\n", ksiSum[0][0][transcript[1]->indexOfPhone][0]);

	for (int t = 1; t < X.T; t++) {
		double test = 0;
		for (int p = 1; p < N+1; p++) {
			for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
				if (t < X.T - 1) {
					for (trans = outtransition[p][s].begin(); trans != outtransition[p][s].end(); trans++) {
						if(trans->dest_phone < N+1 && a.assigned[p][s][t] && b.assigned[trans->dest_phone][trans->dest_state][t + 1]) {
							tmp.dest_phone = trans->dest_phone;
							tmp.dest_state = trans->dest_state;
							tmp.prob = trans->prob + a.alpha[p][s][t] + b.beta[tmp.dest_phone][tmp.dest_state][t + 1] + observationPro[transcript[tmp.dest_phone]->indexOfPhone][tmp.dest_state][t+1] - pro;
							tmp.prob = exp(tmp.prob);
							ksi[p][s][t].push_back(tmp);
							test += tmp.prob;
							ksiSum[transcript[p]->indexOfPhone][s][transcript[tmp.dest_phone]->indexOfPhone][tmp.dest_state] += tmp.prob;
							/*if (exp(tmp.prob) > 0) {
								printf("test ksi : %.20lf\n", tmp.prob);
								printf("time %d : (%d, %d) -> (%d, %d)\n", t, p, s, tmp.dest_phone, tmp.dest_state);
							}*/
						}
					}
				}
				double gaussiantest = 0;
				for (int k = 0; k < 2; k++) {  // for each Gaussian k
					if (assigned[p][s][t]) {
						gammaGaussian[p][s][k][t] = gamma[p][s][t] * exp(partGaussian(transcript[p]->state[s], X.x[t], k) - observationPro[transcript[p]->indexOfPhone][s][t]);
						/*if (k == 0) {
							double test[2] = { partGaussian(transcript[p]->state[s], X.x[t], k) , partGaussian(transcript[p]->state[s], X.x[t], 1) };
							if (logSum(test, 2) != observationPro[transcript[p]->indexOfPhone][s][t]) {
								printf("problem! : %lf, %lf\n", logSum(test, 2), observationPro[transcript[p]->indexOfPhone][s][t]);
							}
						}*/
						//printf("%lf\n", partGaussian(transcript[p]->state[s], X.x[t], k) - observationPro[transcript[p]->indexOfPhone][s][t]);  // testing point
						gammaGaussianSum[transcript[p]->indexOfPhone][s][k] += gammaGaussian[p][s][k][t];
						for (int d = 0; d < 39; d++) {
							gaussianMean[transcript[p]->indexOfPhone][s][k][d] += gammaGaussian[p][s][k][t] * X.x[t][d];
							gaussianVar[transcript[p]->indexOfPhone][s][k][d] += gammaGaussian[p][s][k][t] * X.x[t][d] * X.x[t][d];
						}
					}
				}
			}
		}
		//printf("time %d test ksi : %lf\n",t, test);
	}
	//printf("%lf, %lf\n", gaussianMean[20][1][0][10], gaussianVar[20][1][0][10]);
	//printf("accumulate done\n");
}



void baum_welch() {
	vector<TransitionType>::iterator trans;
	for (int p = 0; p < N_PHONES; p++) {
		for (int s = 0; s < phonelist[p].nOfState - 2; s++) {
			double sum = 0;
			// in phone
			for (int ds = 0; ds < phonelist[p].nOfState - 2; ds++) {
				phonelist[p].tp[s + 1][ds + 1] = ksiSum[p][s][p][ds] / gammaSum[p][s];
				sum += phonelist[p].tp[s + 1][ds + 1];
			}
			phonelist[p].tp[s + 1][phonelist[p].nOfState - 1] = 1 - sum;
			//printf("%lf, %lf\n", sum, phonelist[p].tp[s + 1][phonelist[p].nOfState - 1]);
			for (int k = 0; k < 2; k++) {
				phonelist[p].state[s].pdf[k].weight = gammaGaussianSum[p][s][k]/gammaSum[p][s];
				//printf("%lf\n", gammaGaussianSum[p][s][k]);
				for (int d = 0; d < 39; d++) {
					phonelist[p].state[s].pdf[k].mean[d] = gaussianMean[p][s][k][d] / gammaGaussianSum[p][s][k];
					phonelist[p].state[s].pdf[k].var[d] = gaussianVar[p][s][k][d] / gammaGaussianSum[p][s][k] - phonelist[p].state[s].pdf[k].mean[d] * phonelist[p].state[s].pdf[k].mean[d];
				}
			}
		}
	}
}

void initAllParameter(inputData X) {
	for (int p = 1; p < X.numOfPhones + 1; p++) {
		for (int s = 0; s < transcript[p]->nOfState - 2; s++) {
			outtransition[p][s].clear();
			intransition[p][s].clear();
			for (int t = 0; t < X.T; t++) {
				ksi[p][s][t].clear();
				gamma[p][s][t] = 0;
				gammaGaussian[p][s][0][t] = 0;
				gammaGaussian[p][s][1][t] = 0;
				a.alpha[p][s][t] = 0;
				a.assigned[p][s][t] = 0;
				b.beta[p][s][t] = 0;
				b.assigned[p][s][t] = 0;
				assigned[p][s][t] = 0;
			}
		}
	}
	initObservationPro(X);
}