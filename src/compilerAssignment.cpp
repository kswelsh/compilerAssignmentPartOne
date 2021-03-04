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

	bool testIfSymbol(char sCandidate)
	// pre: parm is a symbol one would like to test is valid
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
	// pre: first parm is a valid symbol, second parm is the current line of source code, third parm is current
	// subscript location of said line, fourth parm is a bool value to note if symbol is complete, fifth parm
	// is what series of symbols one is testing for
	// post: if symbol can create a provided variant, said symbol is created, if not, nothing happens
	{
		symbolFinished = true;
		if (symbolCandidate + line[i] == equalTo)
		{
			symbolCandidate.push_back(line[i]);
			i++;
		}
	}

	void checkSymbolVariation(string &symbolCandidate, string &line, int &i, bool &symbolFinished, bool &error)
	// pre: first parm is any symbol that can be an extend symbol, such as == or !=, second parm is the current line
	// of source code, parm three is the current subscript location on said line, fourth parm is a bool value to note if
	// symbol is complete, fifth parm is bool error value
	// post: if following (i+1) creates an extended symbol, said symbol is created, if not, symbol stays the same
	// error is set to true if single symbol alone is an error, such as '&', symbolFinished is set to true once
	// provided symbol is complete
	{
		if (symbolCandidate >= "(" && symbolCandidate <= "-")
		{
			symbolFinished = true;
		}
		else if (symbolCandidate == ";")
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
	// pre: first parm is a line of code one would like to test for a valid symbol, parm
	// two is the current subscript location of provided line, parm three refers to an bool error value
	// post: if symbol/group of symbols is valid, tests against token map and pushes to _lexemes and
	// _tokens vector, if not, error is set to true and, i is moved to directly
	// after the pushed symbol(s)
	{
		string symbolCandidate = "";
		bool symbolFinished = false;
		while (testIfSymbol(line[i]) && i <= line.size() && symbolFinished == false && error == false)
		{
			symbolCandidate.push_back(line[i]);
			i++;
			checkSymbolVariation(symbolCandidate, line,i, symbolFinished, error);
		}

		if (error == false)
		{
			_lexemes.push_back(symbolCandidate);
			_tokens.push_back(_tokenmap[symbolCandidate]);
		}
	}

	void pushString(string &line, int &i, bool &error, istream &infile)
	// pre: first parm is a line of code one would like to create a string from, parm two is the
	// current subscript location of provided line and must be the location directly after a quotation mark
	// third parm is an bool error value, parm four refers to input file with source code
	// of provided source code
	// post: if string is valid, string is pushed to the _lexemes vector and s_str is pushed to the _tokens vector
	// if string never ends, error is set to true, i is changed to directly after the pushed string
	{
		string stringCandidate = "";
		bool isString = true;
		i++;
		while (error == false && !infile.eof() && line[i] != 34)
		{
			stringCandidate.push_back(line[i]);
			i++;

			if (i >= line.size())
			{
				i = 0;
				getline(infile, line);
			}
		}

		if (infile.eof() && line[i] != 34)
		{
			error = true;
		}
		else
		{
			_lexemes.push_back(stringCandidate);
			_tokens.push_back("s_str");
			i++;
		}
	}

	void pushNum(string &line, int &i, bool &error)
	// pre: first parm is a line of code one would like to test for a number, parm two is the
	// current subscript location of provided line, parm three is an bool error value
	// post: created num is pushed to the _lexemes vector and token t_int is pushed to the _tokens vector
	// if num creates an invalid variable, error is set to true, i is changed to directly after the pushed num
	{
		string numCandidate = "";
		while (isdigit(line[i]) && error == false && i <= line.size())
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
	// pre: first parm is a line of code one would like to test for a valid variable name or token
	// parm two is the current subscript location of provided line
	// post: created variable is tested against the token map, if an token exists, corresponding
	// token is pushed to the _lexemes and _tokens vectors, if does not exist, variable is
	// pushed as id token to both vectors, i is moved to directly after the pushed characters
	{
		string alphaCandidate = "";
		int exist;
		while ((isalpha(line[i]) || isdigit(line[i])) && i <= line.size())
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
	LexAnalyzer(istream& infile)
	// pre: parameter refers to open data file consisting of token and lexeme pairs i.e.
	// s_and and t_begin begin t_int 27.  Each pair appears on its own input line.
	// post: tokenmap has been populated
	{
		string lexemeInput;
		string tokenInput;
		if (!infile)
		{
			cout << "ERROR: Lexeme/token data file is not open! Exiting program." << endl;
			exit(-1);
		}

		infile >> tokenInput >> lexemeInput;
		while (!infile.eof())
		{
			_tokenmap.insert(make_pair(lexemeInput, tokenInput));
			infile >> tokenInput >> lexemeInput;
		}
	}

	void scanFile(istream& infile, ostream& outfile)
	// pre: 1st parameter refers to an open text file that contains source
	// code in the language, 2nd parameter refers to an open empty output file
	// post: If no error, the token and lexeme pairs for the given input
	// file have been written to the output file.  If there is an error,
	// the incomplete token/lexeme pairs, as well as an error message have
	// written to the output file.  A success or fail message has printed
	// to the console.
	{
		string inputLine;
		bool error = false;
		bool errorNeverendingString = false;
		bool errorNonExistentSymbol = false;
		bool errorInvalidVarName = false;
		int i = 0;

		if (!infile)
		{
			cout << "ERROR: Source code file is not open! Exiting program." << endl;
			exit(-1);
		}
		if (!outfile)
		{
			cout << "ERROR: Output file is not open! Exiting program." << endl;
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

		for (int i = 0; i < _tokens.size(); i++)
		{
			cout << _tokens[i] << " : " << _lexemes[i] << "\n";
		}

		if (error == true)
		{
			if (errorNonExistentSymbol == true)
			{
				cout << "ERROR: Symbol entered is not recognized!\n";
			}
			else if (errorNeverendingString == true)
			{
				cout << "ERROR: String entered was never-ending!\n";
			}
			else if (errorInvalidVarName == true)
			{
				cout << "ERROR: Variable name entered was invalid!\n";
			}
			else
			{
				cout << "ERROR: Symbol entered is not recognized!\n";
			}

			cout << "Error occurred... source code not scanned completely." << endl;
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

	//cout << "Enter the name of the source code file: " << endl;
	//cin >> sourceCodeFileName;
	//cout << "Enter the name of the token/lexeme data file: " << endl;
	//cin >> tokenLexemeDataFileName;
	//cout << "Enter the name of the output file: " << endl;
	//cin >> outputFileName;

	outputFileName = "test.txt";
	ifstream tokenLexemeDataFile("tokenlexemedata.txt");
	ifstream sourceCodeFile("sourceCode.txt");
	ofstream outputFile;
	LexAnalyzer scanner(tokenLexemeDataFile);

	tokenLexemeDataFile.close();
	outputFile.open(outputFileName);
	scanner.scanFile(sourceCodeFile, outputFile);
	outputFile.close();
	sourceCodeFile.close();

	return 0;
}
