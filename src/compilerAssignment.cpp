//===========================================================================================
// Name        : compilerAssignment.cpp
// Author      : Kyle Welsh
// Version     : N/A
// Copyright   : N/A
// Description : Part 1 of the compiler assignment. Simply creating the lexical analyzer.
//===========================================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class LexAnalyzer
{
private:
	vector<string> _lexemes;
	vector<string> _tokens;
	map<string, string> _tokenmap;

	void errorDisplay(bool errorOne, bool errorTwo, bool errorThree, ostream &outfile)
	// pre: 1st parm is symbol not recognized error value : 2nd parm is never ending string error value
	//		3rd parm is invalid variable error value : parm 1, 2, or 3 must be true or function will not work
	//		4th parm is currently opened output file
	// post: corresponding error message is displayed and added onto passed in file
	{
		if (errorOne == true)
		{
			cout << "ERROR: Symbol entered is not recognized!" << endl;
			outfile << "ERROR: Symbol entered is not recognized!\n";
		}
		else if (errorTwo == true)
		{
			cout << "ERROR: String entered was never-ending!" << endl;
			outfile << "ERROR: String entered was never-ending!\n";
		}
		else if (errorThree == true)
		{
			cout << "ERROR: Variable name entered was invalid!" << endl;
			outfile << "ERROR: Variable name entered was invalid!\n";
		}
		else
		{
			cout << "ERROR: Symbol entered is not recognized!" << endl;
			outfile << "ERROR: Symbol entered is not recognized!\n";
		}
		cout << "Error occurred. Source code not scanned completely." << endl;
	}

	bool testIfSymbol(char sCandidate)
	// pre: 1st parm is a symbol one would like to test if valid
	// post: returns true if symbol is valid, false if symbol is not valid
	{
		bool symbol = false;
		if (sCandidate >= ';' && sCandidate <= '>')
		{
			symbol = true;
		}
		else if (sCandidate >= ';' && sCandidate <= '>')
		{
			symbol = true;
		}
		else if (sCandidate >= '(' && sCandidate <= '-')
		{
			symbol = true;
		}
		else if (sCandidate == '/' || sCandidate == '%' || sCandidate == '&')
		{
			symbol = true;
		}
		else if (sCandidate == '!' || sCandidate == '|')
		{
			symbol = true;
		}

		return symbol;
	}

	void checkExistentVariant (string &symbolCandidate, string &line, int &i, bool &symbolFinished, string equalTo)
	// pre: 1st parm is any already valid symbol : 2nd parm is the current line program is looking at
	//		3rd parm is subscript location of current line : 4th parm should be bool variable that initally holds
	//		a value of false and can be changed : 5th parm must be the extended symbol you want to test for
	// post: if symbol can create a provided variant, said symbol is pushed to symbolCandidate. if not, nothing happens
	{
		symbolFinished = true;
		if (symbolCandidate + line[i] == equalTo)
		{
			symbolCandidate.push_back(line[i]);
			i++;
		}
	}

	void checkSymbolExtended(string &symbolCandidate, string &line, int &i, bool &symbolFinished, bool &error)
	// pre: 1st parm is any already valid symbol : 2nd parm is the current line program is looking at
	// 		3rd parm is subscript location of current line : 4th parm should be a bool variable that initially holds
	//		a value of false and can be changed : 5th parm is the current error value of the program
	// post: i is changed to directly after valid symbol candidate. if following (i+1) creates an extended symbol
	//  	 said symbol is created. if not, symbol stays the same. if a symbol must be extended to be valid
	// 		 but is unable to be extended, error is set to true
	{
		if (symbolCandidate >= "(" && symbolCandidate <= "-")
		{
			symbolFinished = true;
		}
		else if (symbolCandidate == ";" || symbolCandidate == "%" || symbolCandidate == "/")
		{
			symbolFinished = true;
		}
		else if (symbolCandidate == "=")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, "==");
		}
		else if (symbolCandidate == "<")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, "<=");
		}
		else if (symbolCandidate == ">")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, ">=");
		}
		else if (symbolCandidate == "!")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, "!=");
		}
		else if (symbolCandidate == "&")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, "&&");
			if (symbolCandidate != "&&")
			{
				error = true;
			}
		}
		else if (symbolCandidate == "|")
		{
			checkExistentVariant(symbolCandidate, line, i, symbolFinished, "||");
			if (symbolCandidate != "||")
			{
				error = true;
			}
		}
	}

	void pushSymbol(string &line, int &i, bool &error)
	// pre: 1st parm is current line program is looking at : 2nd parm is subscript location of sent in line
	//		3rd parm is current error value of the program
	// post: i is changed to directly after valid symbol candidate. if no error, symbol is pushed to _lexemes
	// 		 along with the matching token value pushed to _tokens. if symbol does not exist, error is set to true
	{
		string symbolCandidate = "";
		bool symbolFinished = false;
		while (testIfSymbol(line[i]) && i < line.size() && symbolFinished == false && error == false)
		{
			symbolCandidate.push_back(line[i]);
			i++;
			checkSymbolExtended(symbolCandidate, line,i, symbolFinished, error);
		}

		if (error == false)
		{
			_lexemes.push_back(symbolCandidate);
			_tokens.push_back(_tokenmap[symbolCandidate]);
		}
	}

	void pushString(string &line, int &i, bool &error, istream &infile)
	// pre: 1st parm is current line program is looking at : 2nd parm is subscript location of sent in line
	//		3rd parm is current error value of the program : 4th parm is current source code file
	// post: i is changed to directly after valid string candidate. if no error, string is pushed to _lexemes
	//		 along with s_str pushed to _tokens. if error occurs, error is set to true and nothing is pushed.
	// 		 if string spans more than one line, line is changed to the line the string ends on.
	{
		string stringCandidate = "";
		i++;
		while (error == false && !infile.eof() && line[i] != 34)
		{
			stringCandidate.push_back(line[i]);
			i++;

			if (i >= line.size())
			{
				i = 0;
				getline(infile, line);
				if (infile.eof())
				{
					error = true;
				}
			}
		}

		if (error == false)
		{
			_lexemes.push_back(stringCandidate);
			_tokens.push_back("s_str");
			i++;
		}
	}

	void pushNum(string &line, int &i, bool &error)
	// pre: 1st parm is current line program is looking at : 2nd parm is subscript location of sent in line
	//		3rd parm is current error value of the program
	// post: i is changed to directly after valid num candidate. if no error occurs, valid candidate is pushed to
	// 		 _lexemes along with t_int pushed to _tokens. if error occurs, error is set to true and nothing is pushed
	{
		string numCandidate = "";
		while (isdigit(line[i]) && error == false && i < line.size())
		{
			numCandidate.push_back(line[i]);
			i++;

			if (isalpha(line[i]) && i < line.size())
			{
				error = true;
			}
		}

		if (error == false)
		{
			_lexemes.push_back(numCandidate);
			_tokens.push_back("t_int");
		}
	}

	void pushAlpha(string &line, int &i)
	// pre: 1st parm is current line program is looking at : 2nd parm is subscript location of sent in line
	// post: i is changed to directly after valid variable/type candidate. candidate is tested against
	// 		 token map and is inserted into _lexemes and _tokens vectors if exists. if it does not exist
	//		 token value t_id is pushed to _tokens and variable is pushed to _lexemes
	{
		string alphaCandidate = "";
		int exist;
		while ((isalpha(line[i]) || isdigit(line[i])) && i < line.size())
		{
			alphaCandidate.push_back(line[i]);
			i++;
		}

		exist = _tokenmap.count(alphaCandidate);
		if (exist > 0)
		{
			_lexemes.push_back(alphaCandidate);
			_tokens.push_back(_tokenmap[alphaCandidate]);
		}
		else
		{
			_lexemes.push_back(alphaCandidate);
			_tokens.push_back("t_id");
		}
	}


public:
	LexAnalyzer(istream &infile)
	// pre: parameter refers to open data file consisting of token and lexeme pairs i.e.
	// 		s_and and t_begin begin t_int 27.  Each pair appears on its own input line.
	// post: tokenmap has been populated
	{
		string lexemeInput;
		string tokenInput;

		if (!infile)
		{
			cout << "ERROR: Lexeme/token data file is not open or failed to open! Exiting program." << endl;
			exit(-1);
		}

		infile >> tokenInput >> lexemeInput;
		while (!infile.eof())
		{
			_tokenmap.insert(make_pair(lexemeInput, tokenInput));
			infile >> tokenInput >> lexemeInput;
		}
	}

	void scanFile(istream &infile, ostream &outfile)
	// pre: 1st parameter refers to an open text file that contains source
	// 		code in the language, 2nd parameter refers to an open empty output file
	// post: If no error, the token and lexeme pairs for the given input
	// 		 file have been written to the output file.  If there is an error,
	// 		 the incomplete token/lexeme pairs, as well as an error message have
	// 		 written to the output file.  A success or fail message has printed
	// 		 to the console.
	{
		string inputLine;
		bool error = false;
		bool errorNeverendingString = false;
		bool errorNonExistentSymbol = false;
		bool errorInvalidVarName = false;
		int i = 0;

		if (!infile)
		{
			cout << "ERROR: Source code file is not open or failed to open! Exiting program." << endl;
			exit(-1);
		}
		if (!outfile)
		{
			cout << "ERROR: Output file is not open or failed to open! Exiting program." << endl;
			exit(-1);
		}

		getline(infile, inputLine);
		while (!infile.eof() && error == false)
		{

			if (isalpha(inputLine[i]))
			{
				pushAlpha(inputLine, i);
			}
			else if (isdigit(inputLine[i]))
			{
				pushNum(inputLine, i, error);
				errorInvalidVarName = error;
			}
			// ASCII value of " is 34
			else if (inputLine[i] == 34)
			{
				pushString(inputLine, i, error, infile);
				errorNeverendingString = error;
			}
			else if(testIfSymbol(inputLine[i]))
			{
				pushSymbol(inputLine, i, error);
				errorNonExistentSymbol = error;
			}
			else if(isspace(inputLine[i]))
			{
				i++;
			}
			else
			{
				error = true;
			}


			if (i >= inputLine.size())
			{
				getline(infile, inputLine);
				while (inputLine.length() == 0 && !infile.eof())
				{
					getline(infile, inputLine);
				}
				i = 0;
			}
		}

		for (int i = 0; i < _tokens.size(); i++)
		{
			outfile << _tokens[i] << " : " << _lexemes[i] << "\n";
		}

		if (error == true)
		{
			errorDisplay(errorNonExistentSymbol, errorNeverendingString, errorInvalidVarName, outfile);
		}
		else
		{
			cout << "Source code scanning was a success." << endl;
		}
	}
};

int main()
{
	string sourceCodeFileName;
	string tokenLexemeDataFileName;
	string outputFileName;
	ofstream outputFile;

	cout << "Enter the name of the source code file: " << endl;
	cin >> sourceCodeFileName;
	cout << "Enter the name of the token/lexeme data file: " << endl;
	cin >> tokenLexemeDataFileName;
	cout << "Enter the name of the output file: " << endl;
	cin >> outputFileName;

	ifstream tokenLexemeDataFile(tokenLexemeDataFileName);
	LexAnalyzer scanner(tokenLexemeDataFile);
	tokenLexemeDataFile.close();

	ifstream sourceCodeFile(sourceCodeFileName);
	outputFile.open(outputFileName);
	scanner.scanFile(sourceCodeFile, outputFile);
	outputFile.close();
	sourceCodeFile.close();

	if (outputFile.is_open() == true)
	{
		cout << "WARNING: Output file failed to close!" << endl;
	}

	return 0;
}
