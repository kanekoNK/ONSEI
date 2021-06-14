#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <math.h>

double data1[100][150][15];
double data2[100][150][15];
double data3[100][150][15];
double data4[100][150][15];


char fname[4][10]={
                        {"city011"},
                        {"city012"},
                        {"city021"},
                        {"city022"}
                        };

char Fout[6][10]={
		{"out1.csv"},
		{"out2.csv"},
		{"out3.csv"},	
		{"out4.csv"},
		{"out5.csv"},
		{"out6.csv"}};
		

int i,j,k,sum = 1;
char str3[30];
char name[100][15];
double T[6][100][100];

double Fscan(double data[100][150][15]);
double min(double Mi[]);
double DP(double dataA[100][150][15], double dataB[100][150][15], int BN);
double NC(int BN);

int main(){
	int DPct = 0;
	double NPC[6];
	//ファイルの読み込み*****************
	Fscan(data1);
	i++;
	Fscan(data2);
	i++;
	Fscan(data3);
	i++;
	Fscan(data4);
	//***********************************同一話者
	printf("同一話者 city011&city011\n");
	DP(data1,data1,DPct);
	NC(DPct);
	DPct++;
	printf("同一話者 city011&city012\n");
	DP(data1,data2,DPct);
	NC(DPct);
	DPct++;
	//***********************************別話者
	printf("別話者 city011&city021\n");
	DP(data1,data3,DPct);
	NC(DPct);
	DPct++;
	printf("別話者 city011&city022\n");
	DP(data1,data4,DPct);
	NC(DPct);
	DPct++;
	printf("別話者 city012&city021\n");
	DP(data2,data3,DPct);
	NC(DPct);
	DPct++;
	printf("別話者 city012&city022\n");
	DP(data2,data4,DPct);
	NC(DPct);
}

double NC(int BN){
	int AA, BB, CC=0;
	double RI;
	int PA[2];
	for(AA=0; AA<100; AA++){
		RI = 100;
		for(BB=0; BB<100; BB++){
			if(RI > T[BN][AA][BB]){
				RI = T[BN][AA][BB];
				PA[0] = AA;
				PA[1] = BB;
			}
		}
		if(PA[0] == PA[1]){
			CC++;	
		}
	}
	printf("[%d]認識率%d%\n", BN, CC);
	return 0;
}

double DP(double dataA[100][150][15], double dataB[100][150][15], int BN){
	double d[150][150]={0};
	double g[150][150]={0};
	double x, Px;
	double Mi[3];
	int AA,BB;
	FILE *fp;
	fp = fopen(Fout[BN],"w");
	for(AA=0; AA<100; AA++){
		for(BB=0; BB<100; BB++){
			for(i=0; i < dataA[AA][0][0]; i++){
				for(j=0; j < dataB[BB][0][0]; j++){
					for(k=0; k<15; k++){
						x += pow(dataA[AA][i][k]-dataB[BB][j][k], 2.0);
					}
					d[i][j] = sqrt(x);
					x=0;
				}
			}
			g[0][0] = d[0][0];
			for(i =1; i<dataA[AA][0][0]; i++){
				g[i][0] = g[i-1][0]  + d[i][0];
			}
			for(j=1; j<dataB[BB][0][0]; j++){
				g[0][j] = g[0][j-1] + d[0][j];
			}
			for(i=1; i < dataA[AA][0][0]; i++){
				for(j=1; j < dataB[BB][0][0]; j++){
					Mi[0]=g[i-1][j] + d[i][j];
					Mi[1]=g[i-1][j-1] + 2*d[i][j];
					Mi[2]=g[i][j-1] + d[i][j];
					g[i][j] = min(Mi);
				}
			}
			
			T[BN][AA][BB] = g[i-1][j-1] / (i-1+j-1);
			fprintf(fp, "%lf, ", T[BN][AA][BB]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	printf("[%d]演算終了\n", BN);
}

double min(double Mi[]){
	double min = Mi[0];
	int Q;
	for(Q=0; Q<3; Q++){
		if(min > Mi[Q]){
			min = Mi[Q];
		}
	}
	return min;
}

//ファイルの読み込み関数
double Fscan(double data[100][150][15]){ 
	char an[] = "_";
	char tx[] = ".txt";
	FILE *fp;
	for(sum = 1; sum < 101; sum++){;
		sprintf(str3,"%s/%s%s%03d%s",fname[i],fname[i],an,sum,tx);
		fp = fopen(str3, "r");
		if(fp == NULL){
			printf("%s file not open!\n", str3);
			return -1;
		}else{
			fscanf(fp,"%s", str3);
			fscanf(fp,"%s", name[sum-1]);
			fscanf(fp,"%lf",&data[sum-1][0][0]);
			for(j=1; j<data[sum-1][0][0]; j++){
				for(k =0; k<15; k++){
					fscanf(fp,"%lf,", &data[sum-1][j][k]);
				}
			}
		}
		fclose(fp);	
	}
	printf("%s ファイルの読み込み終了\n",fname[i]);
}
