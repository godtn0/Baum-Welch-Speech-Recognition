#include "hmm_reader.h"
#include <stdio.h>
#include <string.h>
#include "typeHeader.h"
#include "file_util.h"

Phone phonelist[21] = { 0, };


void readHmm() {
	FILE * fp = NULL;
	fp = fopen("hmm_train3.txt", "r");
	char trash[100] = { 0, };
	int trash_num = 0;
	char buffer[100];
	int i = 0;
	while (i<N_PHONES) {
		//printf("---------------------------------\n");
		//printf("index : %d\n", i);
		int state_n = 0;
		int gaussian_k = 0;
		int mixture_n = 0;
		phonelist[i].indexOfPhone = i;
		fscanf(fp, "%s %s", trash, phonelist[i].name);  //~h name
														//printf("%s\n", phonelist[i].name);
		removeQuatation(phonelist[i].name, strlen(phonelist[i].name));
		fscanf(fp, "%s", trash);  //<BEGINHMM>
		fscanf(fp, "%s %d", trash, &(phonelist[i].nOfState));  //<NUMSTATES> nOfState
															   //printf("nOfState : %d\n", phonelist[i].nOfState);
		for (int n = 0; n < phonelist[i].nOfState - 2; n++) {  //save gaussian
															   //printf("---state n : %d---\n", n+2);
			fscanf(fp, "%s %d", trash, &state_n);  //<STATE> state_n
			fscanf(fp, "%s %d", trash, &mixture_n);  //<NUMMIXES> mixture_n
													 //printf("mixture_n : %d\n", mixture_n);
			for (int f = 0; f < mixture_n; f++) { //<MEAN>, <VARIANCE>
				fscanf(fp, "%s %d %lf", trash, &trash_num, &(phonelist[i].state[n].pdf[f].weight)); //<MIXTURE> weight
																								   //printf("%dth gaussian weight : %lf\n", f+1, phonelist[i].state[n].pdf[f].weight);
				fscanf(fp, "%s %d", trash, &gaussian_k);
				//printf("dimmension of gaussian : %d\n", gaussian_k);
				//printf("mean : ");
				for (int k = 0; k < gaussian_k; k++) {
					fscanf(fp, "%lf", &(phonelist[i].state[n].pdf[f].mean[k]));
					//printf("%lf  ", phonelist[i].state[n].pdf[f].mean[k]);
				}
				//printf("\n");
				fscanf(fp, "%s %d", trash, &gaussian_k);
				//printf("var : ");
				for (int k = 0; k < gaussian_k; k++) {
					fscanf(fp, "%lf", &(phonelist[i].state[n].pdf[f].var[k]));
					//printf("%lf  ", phonelist[i].state[n].pdf[f].var[k]);
				}
				//printf("\n");
			}
		}
		fscanf(fp, "%s %d", trash, &trash_num);
		//printf("tp : \n");
		for (int row = 0; row < phonelist[i].nOfState; row++) {
			for (int col = 0; col < phonelist[i].nOfState; col++) {
				fscanf(fp, "%lf ", &(phonelist[i].tp[row][col]));
				//printf("%lf ", phonelist[i].tp[row][col]);
			}
			//printf("\n");
		}
		fgets(trash, 100, fp);  //<ENDHMM>
		i++;
		//printf("---------------------------------\n");
	}
	//printf("%lf\n", phonelist[2].tp[1][1]);
	fclose(fp);
}

void writeHmm() {
	FILE * fp = NULL;
	fp = fopen("hmm_train4.txt", "w");
	for (int p = 0; p < 21; p++) {
		fprintf(fp, "~h ");
		fprintf(fp, "\"%s\"\n", phonelist[p].name);
		fprintf(fp, "<BEGINHMM>\n");
		fprintf(fp, "<NUMSTATES> %d\n", phonelist[p].nOfState);
		for (int s = 0; s < phonelist[p].nOfState - 2; s++) {
			fprintf(fp, "<STATE> %d\n", s + 2);
			fprintf(fp, "<NUMMIXES> 2\n");
			for (int k = 0; k < 2; k++) {
				fprintf(fp, "<MIXTURE> %d %lf\n", k + 1, phonelist[p].state[s].pdf[k].weight);
				fprintf(fp, "<MEAN> 39\n");
				for (int d = 0; d < 38; d++) {
					fprintf(fp, "%lf ", phonelist[p].state[s].pdf[k].mean[d]);
				}
				fprintf(fp, "%lf\n", phonelist[p].state[s].pdf[k].mean[38]);
				fprintf(fp, "<VARIANCE> 39\n");
				for (int d = 0; d < 38; d++) {
					fprintf(fp, "%lf ", phonelist[p].state[s].pdf[k].var[d]);
				}
				fprintf(fp, "%lf\n", phonelist[p].state[s].pdf[k].var[38]);
			}
		}
		fprintf(fp, "<TRANSP> %d\n", phonelist[p].nOfState);
		for (int s = 0; s < phonelist[p].nOfState; s++) {
			for (int d = 0; d < phonelist[p].nOfState-1; d++) {
				fprintf(fp, "%lf ", phonelist[p].tp[s][d]);
			}
			fprintf(fp, "%lf\n", phonelist[p].tp[s][phonelist[p].nOfState - 1]);
		}
		fprintf(fp, "<ENDHMM>\n");
	}
	fclose(fp);
}