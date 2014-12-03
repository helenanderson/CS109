#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


using namespace std;

int main() {

	string line;
	int variableNum;
	int vectorNum;
	ifstream trainfile ("simple-train.txt");
	if (trainfile.is_open()) {
		getline(trainfile,line);
		variableNum = stoi(line);
		cout << variableNum << endl;
		getline(trainfile,line);
		vectorNum = stoi(line);
		cout << vectorNum << endl;
		trainfile.close();
	}

	else cout << "Unable to open file." << endl;

	return 0;

}