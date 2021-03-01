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
	vector<string> lexemes;  		// source code file lexemes
	vector<string> tokens;   		// source code file tokens
	map<string, string> tokenmap;   // valid lexeme/token pairs

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

	void pushString(vector<string>& sourceCode, string &line, int &i, int& j, bool& error)
	{
		string stringCandidate = "";
		bool isString = true;

		while (isString == true && error == false)
		{
			stringCandidate.push_back(line[j]);
			j++;

				if (line[j] == 34)
				{
					isString = false;
					stringCandidate.push_back(line[j]);
					j++;
				}
				else if (i == sourceCode.size())
				{
					error = true;
				}
				else if (j == line.size())
				{
					i++;
					line = sourceCode[i];
					j = 0;
					stringCandidate.push_back(' ');
				}
			}
		cout << stringCandidate << endl;
	}

	void pushNum()
	{

	}

	void pushAlpha(string &line, int &j)
	{
		string alphaCandidate = "";
		while (testIfAlpha(line[j]))
		{
			alphaCandidate.push_back(line[j]);
			j++;
		}
		cout << alphaCandidate << endl;
	}

	void pushSymbol(string &line, int &j)
	{
		string symbolCandidate = "";
		while (testIfSymbol(line[j]))
		{
			symbolCandidate.push_back(line[j]);
			j++;
		}
		cout << symbolCandidate << endl;
	}

public:
	LexAnalyzer(istream& infile)
	// pre: parameter refers to open data file consisting of token and
	// lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair    // appears on its own input line.
	// post: tokenmap has been populated
	{
		string lexemInput;
		string tokenInput;
		string fullInput;	//npt needed
		if (!infile)
		{
			cout << "ERROR: Failed opening file! Exiting program." << endl;
			exit(-1);
		}

		infile >> lexemInput >> tokenInput;
		while (!infile.eof())
		{
			tokenmap.insert(make_pair(lexemInput, tokenInput));
			infile >> lexemInput >> tokenInput;

		}

		//map<string, string>::iterator it;
		//for(it = tokenmap.begin(); it != tokenmap.end(); ++it)
		//{
		//  cout << it->first << " : " << it->second << '\n';
		//}
		//cout << endl;
	}

	void scanFile(istream& infile, ostream& outfile)
	// pre: 1st parameter refers to an open text file that contains source
	// code in the language, 2nd parameter refers to an open empty output
	// file
	// post: If no error, the token and lexeme pairs for the given input
	// file have been written to the output file.  If there is an error,
	// the incomplete token/lexeme pairs, as well as an error message have // written to the output file.  A success or fail message has printed // to the console.
	{
		vector<string> sourceCode;
		string inputLine;
		bool error = false;

		if (!infile)
		{
			cout << "ERROR: Failed opening file! Exiting program." << endl;
			exit(-1);
		}
		infile >> inputLine;
		while (!infile.eof())
		{
			sourceCode.push_back(inputLine);
			infile >> inputLine;
		}

		//for (int i = 0; i < sourceCode.size(); i++)
		//{
		//	cout << sourceCode[i] << endl;
		//}

		int i = 0;
		int j = 0;
		while (i < sourceCode.size() && error != true)
		{
			string lineCandidate = sourceCode[i];

			// ALPHA
			if (testIfAlpha(lineCandidate[j]))
			{
				pushAlpha(lineCandidate, j);
			}

			// NUMBER
			else if (lineCandidate[j] >= '0' && lineCandidate[j] <= '9')
			{
				cout << "num" << endl;
			}

			// STRING
			else if (lineCandidate[j] == 34)
			{
				pushString(sourceCode, lineCandidate, i, j, error);
			}

			// SYMBOL
			else if(testIfSymbol(lineCandidate[j]))
			{
				pushSymbol(lineCandidate, j);
			}
			else
			{
				error = true;
			}

			if (j == lineCandidate.size())
			{
				i++;
				j = 0;
			}
		}
	}
};

int main()
{
	ifstream tLData("tokenlexemedata.txt");
	ifstream sCode("sourceCode.txt");
	ofstream oFile("outputfile.txt");
	LexAnalyzer scanner(tLData);
	scanner.scanFile(sCode, oFile);


	return 0;
}
