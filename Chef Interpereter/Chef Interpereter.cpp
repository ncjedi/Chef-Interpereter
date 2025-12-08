// Chef Interpereter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iomanip>
#include <windows.h>
#include <cstring>
#include <codecvt>
#include <locale>
#include "Ingredients.h"
#include "ErrorHandling.h"
#include "IngredInfo.h"
#include "Method.h"

using namespace std;

bool CheckIfTitle(string line)
{
	char LastCharacter;

	string::iterator it = line.end();

	if (line.empty())
	{
		return false;
	}

	if (*(it - 1) == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckIfServes(string line)
{
	string isServes = "";
	string isInt = "";
	int i = 0;
	string::iterator it;

	if (line.empty())
	{
		return false;
	}

	while (i < 7)
	{
		isServes += line[i];
		i++;
	}

	if (isServes != "Serves ")
	{
		return false;
	}

	it = line.end();

	if (*(it - 1) != '.')
	{
		ThrowError(5);
	}

	while (line[i] != '.')
	{
		isInt += line[i];
		i++;
	}

	if (isInt.empty())
	{
		return false;
	}

	for (char c : isInt)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}

	return true;
}

int servesNumber(string serves)
{
	string returnString;
	for (char c : serves)
	{
		if (isdigit(c))
		{
			returnString += c;
		}
	}

	return stoi(returnString);
}

void Interpret(vector<string> lines, vector<string> methodCode, vector<string> ingredientCode, string serves, int methodLine, int ingredientLine, int servesLine)
{
	Ingredients ingredients;
	vector<vector<IngredInfo>> mixingBowls;
	vector<vector<IngredInfo>> bakingDishes;
	int currentILine = ingredientLine;
	int i = 0;
	int j = 0;
	wstring outputString;

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

	for (string line : ingredientCode)
	{
		currentILine++;
		GetIngridient(ingredients, line, currentILine);
	}

	InterpretMethod(methodCode, methodLine, mixingBowls, bakingDishes, ingredients, lines);

	while (i < servesNumber(serves))
	{
		j = bakingDishes[i].size() - 1;
		while (j >= 0)
		{
			if (bakingDishes[i][j].type == 1)
			{
				outputString += to_wstring(bakingDishes[i][j].ammount);
			}
			else if (bakingDishes[i][j].type == 2)
			{
				outputString += wchar_t(bakingDishes[i][j].ammount);
			}
			else if (bakingDishes[i][j].type == 0)
			{
				ThrowError(8);
			}
			j--;
		}
		i++;
	}

	wofstream out("output.txt");
	out.imbue(utf8_locale);
	out << outputString;
	out.close();

	system("notepad.exe output.txt");
}

void ParseMain(istream &in)
{
	vector<string> lines;

	vector<string> IngredientCode;
	vector<string> MethodCode;
	string title;
	string serves;

	string pushLine;
	bool inIngredients = false;
	bool foundIngredients = false;
	bool inMethod = false;
	bool foundMethod = false;
	bool endMain = false;
	int lineNum = 1;
	int ingredientLine = 0;
	int methodLine = 0;

	while (getline(in,pushLine))
	{
		lines.push_back(pushLine);
	}

	if (lines.empty())
	{
		ThrowError(2);
	}

	if (CheckIfTitle(lines[0]))
	{
		title = lines[0];
	}
	else
	{
		ThrowError(1, 0);
	}

	for (string line : lines)
	{
		if (line == "Ingredients." && !inIngredients && !foundIngredients)
		{
			ingredientLine = lineNum;
			inIngredients = true;
		}
		else if (inIngredients)
		{
			if (line == "")
			{
				inIngredients = false;
				foundIngredients = true;
			}
			else
			{
				IngredientCode.push_back(line);
			}

			if (!ingredientLine)
			{
				ingredientLine = lineNum;
			}
		}

		else if (line == "Method." && foundIngredients && !inMethod && !foundMethod)
		{
			inMethod = true;
			methodLine = lineNum;
		}
		else if (inMethod)
		{
			if (line == "")
			{
				inMethod = false;
				foundMethod = true;
			}
			else
			{
				MethodCode.push_back(line);
			}
		}

		else if (foundMethod)
		{
			if (CheckIfServes(line))
			{
				serves = line;
			}
			break;
		}

		lineNum++;
	}

	if (IngredientCode.empty())
	{
		ThrowError(3, ingredientLine);
	}
	if (MethodCode.empty())
	{
		ThrowError(4, methodLine);
	}

	Interpret(lines, MethodCode, IngredientCode, serves, methodLine, ingredientLine, lineNum);
}

int main(int argc, char *argv[])
{
	//_setmode(_fileno(stdout), _O_U16TEXT);

	if (argc > 1)
	{
		ifstream in(argv[1]);
		ParseMain(in);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
