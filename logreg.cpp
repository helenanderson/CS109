#define _USE_MATH_DEFINES
#define EPOCHS 10000
#define LEARN_RATE 0.0001

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;

vector<double> computeZVals(vector<vector<double> > data, vector<double> betas, int variableNum, int vectorNum){
	vector<double> zVals;
	for (int i = 0; i < vectorNum; i++) {
		double z = 0.0;
		zVals.push_back(z);
		for (int j = 0; j <= variableNum; j++) {
			double toAdd = betas[j]*data[i][j];
			zVals[i] += toAdd;
		}
	}
	return zVals;
}

void readDataFile(string filename, int &variableNum, int &vectorNum, vector<vector<double> > &data, vector<double> &YVals) {
	string line;
	ifstream file (filename);
	getline(file,line);
	variableNum = stoi(line);
	getline(file,line);
	vectorNum = stoi(line);

	for (int i = 0; i < vectorNum; i++) {
		vector<double> onedata;
		data.push_back(onedata);
		data[i].push_back(1.0);		
		getline(file,line);
		string buffer;
		stringstream ss(line);
		for (int k = 0; k < variableNum; k++) {
			ss >> buffer; 
			int value = stoi(buffer);
			data[i].push_back(value);
		}
		ss >> buffer;
		int y = stoi(buffer);
		YVals.push_back(y);
	}
}

vector<double> train(string filename) {
	int variableNum;
	int vectorNum; 
	vector<double> allYs;
	vector<vector<double> > alldata; 

	readDataFile(filename, variableNum, vectorNum, alldata, allYs);


	vector<double> betas(variableNum + 1);
	for (int i = 0; i < betas.size(); i++) {
		betas[i] = 0.0;
	} 

	for (int e = 0; e < EPOCHS; e++) {

		vector<double> zVals = computeZVals(alldata, betas, variableNum, vectorNum);
		
		vector<double> gradients(variableNum + 1);
		for (int k = 0; k <= variableNum; k++) {
			double gradient = 0.0;
			gradients.push_back(gradient);
			for (int i = 0; i < vectorNum; i++) {
				double toAdd = alldata[i][k]*(allYs[i] - (1/(1 + pow(M_E, -zVals[i]))));
				gradients[k] += toAdd;
			}
		}
		
		for (int k = 0; k <= variableNum; k++) {
			betas[k] += (LEARN_RATE*gradients[k]);
		}
	}
	return betas;
}


void test(string filename, vector<double> trainData) {	

	int classcount[2] = {0, 0};
	int correctcount[2] = {0, 0}; 
	int variableNum;
	int vectorNum;
	vector<double> ydata;
	vector<vector<double> > testdata; 

	readDataFile(filename, variableNum, vectorNum, testdata, ydata);

	for (int i = 0; i < ydata.size(); i++) {
		int y = ydata[i];
		classcount[y]++;
	}

	vector<double> zVals = computeZVals(testdata, trainData, variableNum, vectorNum);

	for (int i = 0; i < vectorNum; i++) {
		double p = (1/(1 + pow(M_E, -zVals[i])));
		int yGuess = 0;
		if (p > 0.5) {
			yGuess = 1;
		}
		int y = ydata[i];
		if (yGuess == y) {
			correctcount[y]++;
		}
	}

	cout << filename << ":" << endl;
	cout << "Class 0: tested " << classcount[0] << ", correctly classified " << correctcount[0] << endl;
	cout << "Class 1: tested " << classcount[1] << ", correctly classified " << correctcount[1] << endl;
	cout << "Overall: tested " << classcount[0] + classcount[1] << ", correctly classified " << correctcount[0] + correctcount[1] << endl;
	cout << "Accuracy = " <<  (double)(correctcount[0] + correctcount[1])/(double)(classcount[0] + classcount[1]) << endl;
}

int main() {

	vector<double> simpleVector = train("simple-train.txt");
	test("simple-test.txt", simpleVector);

	vector<double> voteVector = train("vote-train.txt");
	test("vote-test.txt", voteVector);

	vector<double> heartVector = train("heart-train.txt");
	test("heart-test.txt", heartVector);

	return 0;
}