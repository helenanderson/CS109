#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;

//void readTrainingData(string filename) {
vector<vector<vector<int> > > train(string filename, bool laplace){

	string line;
	int variableNum;
	int vectorNum;
	ifstream trainfile (filename);

	getline(trainfile,line);
	variableNum = stoi(line);
	//cout << variableNum << endl;
	getline(trainfile,line);
	vectorNum = stoi(line);

	//if using laplace, add four vectors
	if (laplace) {
		vectorNum += 4;
	}
	//cout << vectorNum << endl;
	
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

	//cout << trainingData[0][0][0] << endl;

	for (int i = 0; i < vectorNum; i++) {
		getline(trainfile,line);
		stack<int> reversedData;
		string buffer;
		stringstream ss(line);
		while (ss >> buffer) {
			int value = stoi(buffer);
			reversedData.push(value);
		}
		
		//vector<vector<int> > yTable = trainingData[variableNum];
		int y = reversedData.top();
		reversedData.pop();
		//cout << yTable[0][0] << endl;
		//yTable[0][y]++;
		trainingData[variableNum][0][y]++;

		//while (reversedData.size() > 0) {
		for (int i = 1; i <= variableNum; i++) {
			//vector<vector<int> > xTable = trainingData[variableNum - i];
			int x = reversedData.top();
			reversedData.pop();
			//xTable[x][y]++;
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
	//cout << variableNum << endl;
	getline(testfile,line);
	vectorNum = stoi(line);

	ycount[0] = trainingData[variableNum][0][0];
	ycount[1] = trainingData[variableNum][0][1];

	//cout << "ycounts" << endl;
	//cout << ycount[0] << endl;
	//cout << ycount[1] << endl;
	yprob[0] = (double)ycount[0]/(double)vectorNum;
	yprob[1] = (double)ycount[1]/(double)vectorNum;

	for (int i = 0; i < vectorNum; i++) {
		//cout << "Vector " << i << endl;
		getline(testfile, line);
		vector<int> testValues;
		string buffer;
		stringstream ss(line);
		
		while (ss >> buffer) {
			int value = stoi(buffer);
			testValues.push_back(value);	
		}
		int y = testValues[variableNum];
		//cout << "y class: " << y << endl;
		classcount[y]++;
		double prob[2] = {0.0, 0.0};
		for (int j = 0; j < 2; j++) {
			//int givenY = j;
			double probGivenY = 1.0;
			//int YEqualsiCount = trainingData[variableNum][0][i];
			//cout << "ycount: " << YEqualsiCount << endl;
			//cout << YEqualsiCount << endl;
			//double probYEqualsi = YEqualsiCount/((double)vectorNum); 
			//cout << probYEqualsi << endl;
			for (int k = 0; k < testValues.size () - 1; k++) {
				//cout << "x at pos " << k << endl;
				//cout << "test y: " << j << endl;
				int value = testValues[k];
				int XYIntersectCount = trainingData[k][value][j];
				//cout << "count at (" << value << "," << j << "): " << XYIntersectCount << endl;
				probGivenY *= XYIntersectCount;
			}
			//cout << probGivenY << endl;
			//cout << "multiplied counts: " << probGivenY << endl;
			//cout << "yprob: " << yprob[j] << endl;
			//probGivenY *= yprob[j];
			//cout << "multiplied by prob: " << probGivenY << endl;
			//cout << "ycount: " << ycount[j] << endl;
			//cout << "variableNum: " << variableNum << endl;
			double denom = pow(ycount[j], variableNum);
			//cout << "denom: " << denom << endl;
			probGivenY /= (double)denom;
			prob[j] = probGivenY;
		}
		//cout << "prob[0]: " << prob[0] << endl;
		//cout << "prob[1]: " << prob[1] << endl;
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

	cout << endl; 
	cout << "Running the algorithm using Maximum Liklihood Estimators..." << endl;

	vector<vector<vector<int> > > simpleVector = train("simple-train.txt", false);
	test("simple-test.txt", simpleVector);

	vector<vector<vector<int> > > voteVector = train("vote-train.txt", false);
	test("vote-test.txt", voteVector);

	vector<vector<vector<int> > > heartVector = train("heart-train.txt", false);
	test("heart-test.txt", heartVector);

	cout << endl; 
	cout << endl;



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


	//cout << crazyVector.size() << endl;

	/*for (int i = 0; i < crazyVector.size(); i++) {
		vector<vector<int> > table = crazyVector[i]; 
		vector<int> row = table[0];
		for (int j = 0; j < table.size(); j++) {
			for (int k = 0; k < row.size(); k++) {
				cout << table[j][k] << " ";
			}
			cout << endl;
		}
		cout << "//" << endl;
	}*/

	//read in test file


	
	return 0;

}

