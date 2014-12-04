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
	int variableNum;
	int vectorNum;
	ifstream trainfile (filename);

	getline(trainfile,line);
	variableNum = stoi(line);
	getline(trainfile,line);
	vectorNum = stoi(line);

	vector<double> betas(variableNum + 1);
	for (int i = 0; i < betas.size(); i++) {
		betas[i] = 0.0;
	}
	for (int e = 0; e < EPOCHS; e++) {

		vector<double> gradients(variableNum + 1);
		for (int i = 0; i < gradients.size(); i++) {
			gradients[i] = 0.0;
		}
		double z; 
		for (int k = 0; k <= variableNum; k++) {
			for (int i = 1; i <= vectorNum; i++) {

				vector<int> data;
				data.push_back(1);
				z += betas[0];
				getline(trainfile,line);
				string buffer;
				stringstream ss(line);
				for (int j = 1; j <= variableNum; j++) {
					ss >> buffer; 
					int value = stoi(buffer);
					data.push_back(value);
					z += betas[j]*value;
				}
				ss >> buffer;
				int y = stoi(buffer);

				gradients[k] += data[k]*(y - (1/(1 + pow(M_E, -z))));			
			}
		}
		for (int k = 0; k <= variableNum; k++) {
			betas[k] += LEARN_RATE*gradients[k];
		}
	}
	return betas;
}

int main() {

	return 0;
}