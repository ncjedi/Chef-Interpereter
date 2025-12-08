#include "Auxillery.h"

void InterperetAux(vector<string> lines, vector<string> methodCode, vector<string> ingredientCode, int methodLine, int ingredientLine, vector<vector<IngredInfo>>& mixingBowls)
{
	Ingredients ingredients;
	vector<vector<IngredInfo>> bakingDishes;
	int currentILine = ingredientLine;

	for (string line : ingredientCode)
	{
		currentILine++;
		GetIngridient(ingredients, line, currentILine);
	}

	InterpretMethod(methodCode, methodLine, mixingBowls, bakingDishes, ingredients, lines);
}

vector<IngredInfo> ParseAux(vector<string> lines, string auxName, int errLineNum)
{
	vector<vector<IngredInfo>> mixingBowls;

	vector<string> IngredientCode;
	vector<string> MethodCode;

	bool nameFound = false;
	bool inIngredients = false;
	bool foundIngredients = false;
	bool inMethod = false;
	bool foundMethod = false;
	bool endMain = false;
	int lineNum = 1;
	int ingredientLine = 0;
	int methodLine = 0;

	for (string line : lines)
	{
		if (line == auxName)
		{
			nameFound = true;
		}
		if (line == "Ingredients." && !inIngredients && !foundIngredients && nameFound)
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

	if (!nameFound)
	{
		ThrowError(12, lineNum);
	}

	InterperetAux(lines, MethodCode, IngredientCode, methodLine, ingredientLine, mixingBowls);
	
	return mixingBowls[0];
}