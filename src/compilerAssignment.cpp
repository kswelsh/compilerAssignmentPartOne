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
	vector<string> _lexemes;  		// source code file lexemes
	vector<string> _tokens;   		// source code file tokens
	map<string, string> _tokenmap;   // valid lexeme/token pairs

	bool testIfSymbol(char sCandidate)
	{
		bool symbol = false;
		if (sCandidate >= ';' && sCandidate <= '>')
		{
			symbol = true;
		}
		else if (sCandidate >= '{' && sCandidate <= '}')
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
		else if (sCandidate == '!')
		{
			symbol = true;
		}

		return symbol;
	}

	bool testIfAlpha(char aCandidate)
	{
		bool alpha = false;
		if (aCandidate >= 'a' && aCandidate <= 'z')
		{
			alpha = true;
		}
		else if (aCandidate >= 'A' && aCandidate <= 'z')
		{
			alpha = true;
		}

		return alpha;
	}

	void ifSymbolVariant (string &symbolCandidate, string &line, int &i, bool &symbolFinished, string equalTo)
	{
		symbolFinished = true;
		if (symbolCandidate + line[i] == equalTo)
		{
			symbolCandidate.push_back(line[i]);
			i++;
		}
	}

	void checkSymbolVariation(string &symbolCandidate, string &line, int &i, bool &symbolFinished, bool &error)
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
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, "==");
		}
		else if (symbolCandidate == "<")
		{
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, "<=");
		}
		else if (symbolCandidate == ">")
		{
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, ">=");
		}
		else if (symbolCandidate == "!")
		{
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, "!=");
		}
		else if (symbolCandidate == "&")
		{
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, "&&");
			if (symbolCandidate != "&&")
			{
				error = true;
			}
		}
		else if (symbolCandidate == "|")
		{
			ifSymbolVariant(symbolCandidate, line, i, symbolFinished, "||");
			if (symbolCandidate != "||")
			{
				error = true;
			}
		}
	}

	void pushString(string &line, int &i, bool &error, istream &infile)
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
	{
		string numCandidate = "";
		while (line[i] >= '0' && line[i] <= '9' && error == false && i <= line.size())
		{
			numCandidate.push_back(line[i]);
			i++;

			if (testIfAlpha(line[i]) && i < line.size())
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
	{
		string alphaCandidate = "";
		int exist;
		while (testIfAlpha(line[i]) && i <= line.size())
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

	void pushSymbol(string &line, int &i, bool &error)
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
			cout << "ERROR: Failed opening file! Exiting program." << endl;
			exit(-1);
		}

		infile >> tokenInput >> lexemeInput;
		while (!infile.eof())
		{
			_tokenmap.insert(make_pair(lexemeInput, tokenInput));
			infile >> tokenInput >> lexemeInput;
		}

		//map<string, string>::iterator mitr;
		//for (mitr = _tokenmap.begin(); mitr != _tokenmap.end(); mitr++)
		//{
		//	cout << mitr->first << " : " << mitr->second << endl;
		//}
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
		int i = 0;

		if (!infile)
		{
			cout << "ERROR: Failed opening file! Exiting program." << endl;
			exit(-1);
		}

		getline(infile, inputLine);
		while (!infile.eof() && error == false)
		{

			if (testIfAlpha(inputLine[i]))
			{
				pushAlpha(inputLine, i);
			}
			else if (inputLine[i] >= '0' && inputLine[i] <= '9')
			{
				pushNum(inputLine, i, error);
			}
			// ASCII value of " is 34
			else if (inputLine[i] == 34)
			{
				pushString(inputLine, i, error, infile);
			}
			else if(testIfSymbol(inputLine[i]))
			{
				pushSymbol(inputLine, i, error);
			}
			// ASCII value of 'tab' is 9
			else if(inputLine[i] == ' ' || inputLine[i] == 9)
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
				i = 0;
			}
		}

		for (int i = 0; i < _tokens.size(); i++)
		{
			cout << _tokens[i] << " : " << _lexemes[i] << endl;
		}
	}
};

int main()
{
	string line;
	string test;
	ifstream tLData("tokenlexemedata.txt");
	ifstream sCode("sourceCode.txt");
	ofstream oFile("outputfile.txt");
	LexAnalyzer scanner(tLData);
	scanner.scanFile(sCode, oFile);

	return 0;
}
