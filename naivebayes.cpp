#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;

vector<vector<vector<int> > > train(string filename, bool laplace){

	//read training file
	string line;
	ifstream trainfile (filename);

	getline(trainfile,line);
	int variableNum = stoi(line);
	getline(trainfile,line);
	int vectorNum = stoi(line);

	//if using laplace, add four to vector count
	if (laplace) {
		vectorNum += 4;
	}
	
	//initialize all values in 3D vector
	vector<vector<vector<int> > > trainingData;
	vector<vector<int> > twodplaceholder;
	vector<int> onedplaceholder;
	
	for (int i = 0; i < variableNum + 1; i++) {

		trainingData.push_back(twodplaceholder);		
		for (int j = 0; j < 2; j++) {
			trainingData[i].push_back(onedplaceholder);
			for (int k = 0; k < 2; k++) {
				trainingData[i][j].push_back(0);
			}
		}
	}


	for (int i = 0; i < vectorNum; i++) {
		getline(trainfile,line);
		stack<int> reversedData;
		string buffer;
		stringstream ss(line);
		while (ss >> buffer) {
			int value = stoi(buffer);
			reversedData.push(value);
		}
		
		int y = reversedData.top();
		reversedData.pop();
		trainingData[variableNum][0][y]++;

		for (int i = 1; i <= variableNum; i++) {
			int x = reversedData.top();
			reversedData.pop();

			trainingData[variableNum - i][x][y]++;
		}	
	}

	trainfile.close();
	return trainingData;
}

void test(string filename, vector<vector<vector<int> > > trainingData) {

	int classcount[2] = {0, 0};
	int correctcount[2] = {0, 0}; 
	int ycount[2] = {0, 0};
	double yprob[2] = {0.0, 0.0};

	string line;
	int variableNum;
	int vectorNum;
	ifstream testfile (filename);

	getline(testfile,line);
	variableNum = stoi(line);
	getline(testfile,line);
	vectorNum = stoi(line);

	ycount[0] = trainingData[variableNum][0][0];
	ycount[1] = trainingData[variableNum][0][1];

	yprob[0] = (double)ycount[0]/(double)vectorNum;
	yprob[1] = (double)ycount[1]/(double)vectorNum;

	for (int i = 0; i < vectorNum; i++) {
		getline(testfile, line);
		vector<int> testValues;
		string buffer;
		stringstream ss(line);
		
		while (ss >> buffer) {
			int value = stoi(buffer);
			testValues.push_back(value);	
		}
		int y = testValues[variableNum];
		classcount[y]++;
		double prob[2] = {0.0, 0.0};
		for (int j = 0; j < 2; j++) {
			double probGivenY = 1.0;
			for (int k = 0; k < testValues.size () - 1; k++) {
				int value = testValues[k];
				int XYIntersectCount = trainingData[k][value][j];
				probGivenY *= XYIntersectCount;
			}
			double denom = pow(ycount[j], variableNum);
			probGivenY /= (double)denom;
			prob[j] = probGivenY;
		}
		int yGuess = 0;
		if (prob[1] > prob[0]) {
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


void appendRowsToFile(string fileName){
	//Get number of variables from file
	string line;
	ifstream trainfile (fileName);
	getline(trainfile,line);
	int varNum = stoi(line);
	trainfile.close();

	//append four rows to file
	ofstream file;
	file.open(fileName, ios::app);
	file << "\n";
	for (int i = 0; i < varNum; i++) {
		//last digit does not have a space afterwards
		if (i == varNum -1) {
			file << "0";
		} else {
			file << "0 ";
		}
	}
	file << ": 0 \n";

	for (int i = 0; i < varNum; i++) {
		//last digit does not have a space afterwards
		if (i == varNum -1) {
			file << "0";
		} else {
			file << "0 ";
		}
	}
	file << ": 1 \n";

	for (int i = 0; i < varNum; i++) {
		//last digit does not have a space afterwards
		if (i == varNum -1) {
			file << "1";
		} else {
			file << "1 ";
		}
	}
	file << ": 0 \n";

	for (int i = 0; i < varNum; i++) {
		//last digit does not have a space afterwards
		if (i == varNum -1) {
			file << "1";
		} else {
			file << "1 ";
		}
	}
	file << ": 1 \n";

	file.close();
}

//creates a copy of the training file
void copyFile(string fileName, string copyFileName) {
	std::ifstream in(fileName); 
    std::ofstream out(copyFileName); 
    out << in.rdbuf(); 
    in.close();
    out.close();
}

int main() {


	cout << "Running the algorithm using Maximum Liklihood Estimators..." << endl;

	vector<vector<vector<int> > > simpleVector = train("simple-train.txt", false);
	test("simple-test.txt", simpleVector);

	vector<vector<vector<int> > > voteVector = train("vote-train.txt", false);
	test("vote-test.txt", voteVector);

	vector<vector<vector<int> > > heartVector = train("heart-train.txt", false);
	test("heart-test.txt", heartVector);



	cout << "Running the algorithm using Laplace Estimators..." << endl;


    copyFile("simple-train.txt", "simple-train-copy.txt");
    copyFile("vote-train.txt", "vote-train-copy.txt");
    copyFile("heart-train.txt", "heart-train-copy.txt");

	appendRowsToFile("simple-train-copy.txt");
	appendRowsToFile("vote-train-copy.txt");
	appendRowsToFile("heart-train-copy.txt");

	vector<vector<vector<int> > > laplaceSimpleVector = train("simple-train-copy.txt", true);
	test("simple-test.txt", laplaceSimpleVector);

	vector<vector<vector<int> > > laplaceVoteVector = train("vote-train-copy.txt", true);
	test("vote-test.txt", laplaceVoteVector);

	vector<vector<vector<int> > > laplaceHeartVector = train("heart-train-copy.txt", true);
	test("heart-test.txt", laplaceHeartVector);

	return 0;

}

