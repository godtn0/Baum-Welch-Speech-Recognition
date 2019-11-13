#include "train.h"
#include "file_util.h"
#include "transition.h"
#include "train_algo.h"

inputData readData(char * path) {
	FILE * fp = fopen(path, "r");
	inputData X = { 0, };
	int d;
	fscanf(fp, "%d %d", &X.T, &d);
	for (int i = 0; i < X.T; i++) {
		for (int j = 0; j < d; j++) {
			fscanf(fp, "%lf", &X.x[i][j]);
		}
	}
	fclose(fp);
	return X;
}
void readTrainingData() {
	FILE * fp = fopen("trn_mono.txt", "r");
	char buffer[100] = { 0, };
	int trash = 0;
	inputData X = { 0, };
	int count = 0;
	int n = 0;
	while (fgets(buffer, 100, fp) != NULL) {
		if (fgets(buffer, 100, fp) == NULL) {
			break;
		}
		removeQuatation(buffer, strlen(buffer) - 1);
		changeTotxt(buffer);
		X = readData(buffer);
		int i = 0;
		while (1) {
			fscanf(fp, "%s", buffer);
			if (!strcmp(buffer, ".")) {
				break;
			}
			strcpy(X.phonesSequence[i++], buffer);
		}
		X.numOfPhones = i;
		makeTranscript(X);
		makeTransition(X);
		makeLogScale(X);
		makeObservationPro(X);
		acc(X);
		initAllParameter(X);

		count++;
		if (count % 37 == 0) {
			n += 3;
			printf("%d%% trained\n", n);
		}
	}

	baum_welch();


	printf("done\n");
	fclose(fp);
}
