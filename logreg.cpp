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

vector<double> train(string filename) {

	string line;
	ifstream trainfile (filename);

	getline(trainfile,line);
	int variableNum = stoi(line);
	getline(trainfile,line);
	int vectorNum = stoi(line);

	vector<double> betas(variableNum + 1);
	for (int i = 0; i < betas.size(); i++) {
		betas[i] = 0.0;
	} 

	//read in data
	vector<double> allYs;
	vector<vector<double> > alldata; 
	for (int i = 0; i < vectorNum; i++) {
		vector<double> onedata;
		alldata.push_back(onedata);
		alldata[i].push_back(1.0);		
		getline(trainfile,line);
		string buffer;
		stringstream ss(line);
		for (int k = 0; k < variableNum; k++) {
			ss >> buffer; 
			int value = stoi(buffer);
			alldata[i].push_back(value);

		}
		ss >> buffer;
		int y = stoi(buffer);
		allYs.push_back(y);

	}
/*
	for (int i = 0; i < alldata.size(); i++) {
		for (int j = 0; j < alldata[0].size(); j++) {
			cout << alldata[i][j];
		}
		cout << endl;
	}

	for (int i = 0 ; i < allYs.size(); i++) {
		cout << allYs[i] << " ";
	}
	cout << endl; 

	*/

	for (int e = 0; e < EPOCHS; e++) {

		vector<double> gradients(variableNum + 1);

		//cache z's
		vector<double> zVals;
		for (int i = 0; i < vectorNum; i++) {
			double z = 0.0;
			zVals.push_back(z);
			for (int j = 0; j < variableNum + 1; j++) {
				double toAdd = betas[j]*alldata[i][j];
				zVals[i] += toAdd;
			}
		}
		

		for (int k = 0; k <= variableNum; k++) {
			double gradient = 0.0;
			gradients.push_back(gradient);
			for (int i = 0; i < vectorNum; i++) {
				gradients[k] += alldata[i][k]*(allYs[i] - (1/(1 + pow(M_E, -zVals[i]))));

			}
		}
		
		for (int k = 0; k <= variableNum; k++) {
			betas[k] += LEARN_RATE*gradients[k];
		}
	}
	return betas;
}


void test(string filename, vector<double> trainData) {	

	int classcount[2] = {0, 0};
	int correctcount[2] = {0, 0}; 

	string line;
	int variableNum;
	int vectorNum;
	ifstream testfile (filename);

	getline(testfile,line);
	variableNum = stoi(line);
	getline(testfile,line);
	vectorNum = stoi(line);

	double z = 0.0;

	for (int i = 1; i <= vectorNum; i++) {
		vector<double> data;
		data.push_back(1);
		z += trainData[0];
		getline(testfile,line);
		string buffer;
		stringstream ss(line);
		for (int j = 1; j <= variableNum; j++) {
			ss >> buffer; 
			int value = stoi(buffer);
			data.push_back(value);
			z += trainData[j]*value;
		}
		ss >> buffer;
		int y = stoi(buffer);
		classcount[y]++;

		double p = (1/(1 + pow(M_E, -z)));
		cout << p << endl;
		int yGuess = 0;
		if (p > 0.5) {
			yGuess = 1;
		}
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

	//vector<double> voteVector = train("vote-train.txt");
	//test("vote-test.txt", voteVector);

	vector<double> heartVector = train("heart-train.txt");
	test("heart-test.txt", heartVector);

	return 0;
}