#include "Ingredients.h"

bool CheckIfAmmount(string ammount)
{
	for (char c : ammount)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}

	return true;
}

int CheckType(string type)
{
	if (type == "heaped cup" || type == "heaped cups" || type == "heaped teaspoon" || type == "heaped teaspoons" || type == "heaped tablespoon" || type == "heaped tablespoons" || type == "level cup" || type == "level cups" || type == "level teaspoon" || type == "level teaspoons" || type == "level tablespoon" || type == "level tablespoons")
	{
		return 1;
	}
	else if (type == "heaped" || type == "level")
	{
		return 2;
	}
	else if (type == "g" || type == "kg" || type == "pinch" || type == "pinches" || type == "ml" || type == "l" || type == "dash" || type == "dashes" || type == "cup" || type == "cups" || type == "teaspoon" || type == "teaspoons" || type == "tablespoon" || type == "tablespoons")
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

int GetTypeDOrW(string line)
{
	if (line == "cup" || line == "cups" || line == "teaspoon" || line == "teaspoons" || line == "tablespoon" || line == "tablespoons")
	{
		return 0;
	}
	else if (line == "ml" || line == "l" || line == "dash" || line == "dashes")
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

vector<string> ParseLine(string line, int lineNum)
{
	string ammount;
	string type;
	string varName;
	string tempString;
	int part = 0;
	int spaceNum = 0;
	int theType;
	int i = 0;
	int afterAmmount = 0;
	string::iterator it = line.begin();

	while (line[i] == ' ' || line[i] == '\t')
	{
		i++;
		it++;
	}

	spaceNum = count(it, line.end(), ' ');

	if (spaceNum == 0)
	{
		while (i < line.length())
		{
			varName += line[i];
			i++;
		}

		if (varName.empty())
		{
			ThrowError(7, lineNum);
		}

		return {"0", "cup", varName};
	}

	while (line[i] != ' ')
	{
		tempString += line[i];
		i++;
	}
	i++;

	if (CheckIfAmmount(tempString))
	{
		ammount = tempString;
		afterAmmount = i;
	}
	else
	{
		ammount = "0";
		i = 0;
	}

	tempString = "";

	while (line[i] != ' ' && i < line.length())
	{
		tempString += line[i];
		i++;
	}

	theType = CheckType(tempString);

	if (theType == 3)
	{
		type = tempString;
		i++;
	}
	else if (theType == 2)
	{
		type = tempString;

		do
		{
			type += line[i];
			i++;
		} 
		while (line[i] != ' ' && i < line.length());

		if (!CheckType(type))
		{
			ThrowError(7, lineNum);
		}
		i++;
	}
	else
	{
		type = "cup";
		i = afterAmmount;
	}

	tempString = "";

	while (i < line.length())
	{
		varName += line[i];
		i++;
	}

	if (varName.length() == 0)
	{
		ThrowError(7, lineNum);
	}

	return { ammount,type,varName };
}

void GetIngridient(Ingredients& ingredients, string line, int lineNum)
{
	vector<string> parsedLine;
	string ammount;
	string type;
	string varName;

	parsedLine = ParseLine(line, lineNum);

	ammount = parsedLine[0];
	type = parsedLine[1];
	varName = parsedLine[2];

	ingredients[varName].ammount = stoi(ammount);
	ingredients[varName].type = GetTypeDOrW(type);
}