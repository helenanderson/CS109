#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;

main() {
	string filename = "simple-train.txt";
	string line;
	int variableNum;
	int vectorNum;
	ifstream trainfile (filename);

	getline(trainfile,line);
	variableNum = stoi(line);
	getline(trainfile,line);
	vectorNum = stoi(line);

	double betas[variableNum];

	return 0;
}