#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

int main() {
	//Variables
	string line = "";
	string token = "";
	string reservedWords[] = { "class", "constructor", "method", "function", "int", "boolean", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this" };
	string booleans[] = { "true", "false" };
	bool isStrConst = false;
	bool isLongComment = false;
	bool isComment = false;
	vector<string> lexemeVec;
	vector <string> categoryVec;
	vector<string> strConstRef;

	//Open file to be read
	fstream readFile;
	readFile.open("SquareGame.jack");

	//Create .xml file to be written in

	//Create file
	fstream writeFile;
	writeFile.open("SquareGameTokens.xml", ios::out);


	//Read file
	while (getline(readFile, line)) {

		//Ignore comments before program
		//Ignore comments contained within a long comment

		if (isLongComment == true) {
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '*' && line[i + 1] == '/') {
					isLongComment = false;
					continue;
				}
				continue;
			}
			continue;
		}

		//Check for beginning of a long comment
		if (line[0] == '/' && line[1] == '*') {
			isLongComment = true;
			continue;
		}

		// Check if beginning of the current line has a '/'
		if (line[0] == '/') {
			continue;
		}
		

		//Tokenize
		//Break program down into tokens

		for (int i = 0; i < line.length(); i++) {

			if (line[i] == '/' && line[i + 1] == '/') {
				isComment = true;
				break;
			}

			// Check for string constant
			if (isStrConst == true) { //If currently traversing chars within a string constant
				if (line[i] == '"') { //If current char is closeing quotation
					isStrConst = false;
					lexemeVec.push_back(token);

					//Keep track of string constant placement in lexeme vector
					strConstRef.push_back(token);

					token = "";
					continue;
				}
				token += line[i]; //If still traversing string constant: concatenate char to token
				continue;
			}
			if (line[i] == '"') { //If current char is and open quotation
				if (!token.empty()) {
					lexemeVec.push_back(token);
					token = "";
				}
				isStrConst = true; //Currently traversing string constant
				continue;
			}

			// Check for white space
			if (isspace(line[i])) {
				if (!token.empty()) {
					lexemeVec.push_back(token);
					token = "";
					continue;
				}
				else {
					continue;
				}
			}

			// Check for symbol
			if (ispunct(line[i])) {
				if (!token.empty()) {
					lexemeVec.push_back(token);
					token = line[i];
					lexemeVec.push_back(token);
					token = "";
					continue;
				}
				else {
					token = line[i];
					lexemeVec.push_back(token);
					token = "";
					continue;
				}
			}

			// Check for words (booleans, identifiers, reserved words)
			if (isalpha(line[i]) || line[i] == '_') {
				token += line[i];
				continue;
			}

			// Check for integer
			if (isdigit(line[i])) {
				token += line[i];
				continue;
			}

		}

		if (isComment == true) {
			continue;
		}
	}
	
	//Assign each token a token category
	for (int i = 0; i < lexemeVec.size(); i++) { //Traverse through each token
		bool boolStrConst = false;
		bool boolReservedWord = false;
		bool boolBoolean = false;
		token = lexemeVec[i];

		if (token[0] == '_') {
			categoryVec.push_back("identifier");
			continue;
		}

		if (ispunct(token[0])) {
			categoryVec.push_back("symbol");
			continue;
		}

		if (isdigit(token[0])) {
			categoryVec.push_back("integer");
			continue;
		}

		if (isalpha(token[0])) {
			for (int j = 0; j < strConstRef.size(); j++) {
				if (token == strConstRef[j]) {
					categoryVec.push_back("string constant");
					boolStrConst = true;
					break;
				}
			}

			for (int j = 0; j < sizeof(reservedWords) / sizeof(reservedWords[0]); j++) {
				if (token == reservedWords[j]) {
					categoryVec.push_back("reserved word");
					boolReservedWord = true;
					break;
				}
			}

			for (int j = 0; j < sizeof(booleans) / sizeof(booleans[0]); j++) {
				if (token == booleans[j]) {
					categoryVec.push_back("boolean");
					boolBoolean = true;
					break;
				}
			}
			
			if (boolStrConst == false && boolReservedWord == false && boolBoolean == false) {
				categoryVec.push_back("identifier");
				continue;
			}
			
		}
	}

	//Write output

	for (int i = 0; i < lexemeVec.size(); i++) {
		writeFile << "<" << categoryVec[i] << "> " << lexemeVec[i] << " </" << categoryVec[i] << ">" << endl;
	}

	//Close files
	readFile.close();
	writeFile.close();
	
	return 0;
}