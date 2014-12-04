#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>


using namespace std;

void readTrainingData(string filename) {
//vector<vector<vector<int>>> readTrainingData(string filename){

	string line;
	int variableNum;
	int vectorNum;
	ifstream trainfile (filename);

	getline(trainfile,line);
	variableNum = stoi(line);
	//cout << variableNum << endl;
	getline(trainfile,line);
	vectorNum = stoi(line);
	//cout << vectorNum << endl;
	
	vector<vector<vector<int> > > trainingData;

	for (int i = 0; i < vectorNum; i++) {
		getline(trainfile,line);
		stack<int> reversedData;
		string buffer;
		stringstream ss(line);
		while (ss >> buffer) {
			int value = stoi(buffer);
			reversedData.push(value);
		}
		while (reversedData.size() > 0) {
			int check = reversedData.top();
			reversedData.pop();
		}	
	}

	trainfile.close();
	return;
}

int main() {

	readTrainingData("heart-train.txt");
	return 0;

}