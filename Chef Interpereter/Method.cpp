#include "Method.h"

using namespace std;

void LoadUtinsils(vector<vector<IngredInfo>>& bakingDishes, vector<vector<IngredInfo>>& mixingBowls)
{
	for (int i = 0; i < 100; i++)
	{
		bakingDishes.push_back(vector<IngredInfo>{});
		mixingBowls.push_back(vector<IngredInfo>{});
	}
}

bool CheckContainIngredient(int& i, string line, string& ingriStr, Ingredients ingri)
{
	string tempStr = "";
	int s = i;
	bool contains = false;

	//cout << "INGRI NUM " << i;

	while (s < line.length())
	{
		tempStr += line[s];

		if (ingri.count(tempStr))
		{
			i = s;
			ingriStr = tempStr;
			contains = true;
		}

		s++;
	}

	//cout << "INGRI " << ingriStr << endl;

	return contains;
}

bool CheckContainWord(int& i, string line, string check)
{
	string tempStr;

	//cout << "WORD NUM " << i;
	int s = i;

	while (s < line.length())
	{
		tempStr += line[s];

		if (tempStr == check)
		{
			i = s;
			//cout << "WORD " << tempStr << endl;
			return true;
		}

		s++;
	}

	//cout << "WORD " << tempStr << endl;
	return false;
}

bool CheckContainNumber(int& i, string line, string& numReturn)
{
	string tempStr = "";
	string tempStr2 = "";
	bool digit = false;
	int s = i;

	//cout << "NUM NUM " << i;

	while (s < (line.length() - 1))
	{
		if (!isdigit(line[s]))
		{
			break;
		}

		tempStr += line[s];

		s++;
	}

	while (s < (line.length() - 1))
	{
		tempStr2 += line[s];

		if (tempStr2.length() == 2)
		{
			if (tempStr2 == "st" || tempStr2 == "nd" || tempStr2 == "rd" || tempStr2 == "th")
			{
				digit = true;
				i = s;
			}
			else
			{
				digit = false;
			}

			break;
		}

		s++;
	}

	if (digit)
	{
		numReturn = tempStr;
	}

	//cout << "NUMBER " << tempStr << endl;

	return digit;
}

bool CheckContainPlainNumber(int& i, string line, string& numReturn)
{
	string tempStr = "";
	string tempStr2 = "";
	bool digit = true;
	int s = i;

	//cout << "NUM NUM " << i;
	//cout << line;

	while (s < (line.length() - 1) && line[s] != ' ')
	{
		if (!isdigit(line[s]))
		{
			digit = false;
			break;
		}

		tempStr += line[s];

		s++;
	}

	//cout << endl << tempStr << endl;

	if (digit)
	{
		i = s - 1;
		numReturn = tempStr;
	}

	//cout << "NUMBER " << tempStr << endl;

	return digit;
}

bool CheckContainAux(int& i, string line, string& auxReturn)
{
	string tempStr;
	int s = i;

	while (s < (line.length()))
	{
		tempStr += line[s];

		s++;
	}

	i = s;

	if (tempStr == "")
	{
		return false;
	}
	else
	{
		auxReturn = tempStr;
		return true;
	}
}

bool CheckIfMethod(string line, Ingredients ingri, vector<string>& IngriPosition, vector<int>& bowlPosition, vector<int>& dishPosition, int pos, int currentLinePos, int lineNum, vector<pair<bool,string>> checks, vector<string>& verbs)
{
	int i = pos + 2;
	string tempStr = "";

	string ingriStr = "";
	string bowlNumStr = "0";
	string dishNumStr = "0";
	string numberStr = "0";
	bool contains;

	string::iterator it = line.end();

	if (*(it - 1) != '.')
	{
		ThrowError(9, lineNum + currentLinePos);
	}

	for (auto check : checks)
	{
		std::cout << check.first << " " << check.second << "\n";
		if (check.second == "Ingri")
		{
			contains = CheckContainIngredient(i, line, ingriStr, ingri);
		}

		else if (check.second == "Bnum")
		{
			contains = CheckContainNumber(i, line, bowlNumStr);
		}

		else if (check.second == "Dnum")
		{
			contains = CheckContainNumber(i, line, dishNumStr);
		}

		else if (check.second == "num")
		{
			contains = CheckContainPlainNumber(i, line, numberStr);
		}

		else if (check.second == "aux")
		{
			contains = CheckContainAux(i, line, numberStr);
		}

		else
		{
			contains = CheckContainWord(i, line, check.second);
		}

		if (!contains && check.first)
		{
			std::cout << "DYING HERE\n";
			return false;
		}

		if (contains)
		{
			i += 2;
		}

		//cout << "LOOP NUM " << i;
	}

	IngriPosition.push_back(ingriStr);
	bowlPosition.push_back(stoi(bowlNumStr));
	dishPosition.push_back(stoi(dishNumStr));
	verbs.push_back(numberStr);
	return true;
}

string RemoveEDVerb(string verb)
{
	string returnVerb;
	int i = 0;

	while (i < verb.length() - 3)
	{
		returnVerb += verb[i];
		i++;
	}

	return returnVerb;
}

int CheckIfVerb(string line, Ingredients ingri, vector<string>& IngriPosition, int lineNum, int currentLinePos, vector<string>& verbs)
{
	string tempStr = "";
	string tempIngStr = "";
	string IngStr = "";
	string returnVerb;
	string verbWithED;
	int spaceCount = 0;
	int i = 0;
	vector<string> words;
	vector<string>::iterator it;

	for (char c : line)
	{
		tempStr += c;

		if (spaceCount >= 2)
		{
			tempIngStr += c;
		}

		if (c == ' ' || c == '.')
		{
			words.push_back(tempStr);
			tempStr = "";
			spaceCount++;
		}

		if (ingri.count(tempIngStr))
		{
			IngStr = tempIngStr;
		}
	}

	IngriPosition.push_back(IngStr);

	if (count(words.begin(), words.end(), "until "))
	{
		verbs.push_back(RemoveEDVerb(*(words.end() - 1)));

		return 1;
	}

	else if (tempIngStr == IngStr + ".")
	{
		words[0][0] = tolower(words[0][0]);

		verbs.push_back((words[0]));

		return 0;
	}

	else
	{
		ThrowError(9, lineNum + currentLinePos);
	}
}

int KeyWordType(string line, Ingredients ingri, vector<string>& IngriPosition, vector<int>& bowlPosition, vector<int>& dishPosition, int currentLinePos, int lineNum, vector<string>& verbs)
{
	string check;
	int pos = 0;

	for (char c : line)
	{
		if (c != ' ' && c != '\t')
		{
			check += c;
		}

		std::cout << check << " ";

		if (check == "Take")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {true, "from"}, {false, "the"}, {true, "refrigerator"}}, verbs))
				return 0;
		}
		else if (check == "Put")
		{
			std::cout << "BOO\n";
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {true, "into"},{false, "the"}, {false, "Bnum"}, {true, "mixing bowl"} }, verbs))
			{
				std::cout << "BOO2\n";
				return 1;
			}
		}
		else if (check == "Fold")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {true, "into"},{false, "the"}, {false, "Bnum"}, {true, "mixing bowl"} }, verbs))
				return 2;
		}
		else if (check == "Add")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {false, "to"}, {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"} }, verbs))
				return 3;

			else if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "dry ingredients"}, {false, "to"}, {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"} }, verbs))
				return 15;
		}
		else if (check == "Remove")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {false, "from"}, {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"} }, verbs))
				return 4;
		}
		else if (check == "Combine")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {false, "into"}, {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"} }, verbs))
				return 5;
		}
		else if (check == "Divide")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {false, "into"}, {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"} }, verbs))
				return 6;
		}
		else if (check == "Liquefy")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "ingri"} }, verbs))
				return 7;

			else if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "contents of the"}, {false, "Bnum"}, {true, "mixing bowl"} }, verbs))
				return 16;
		}
		else if (check == "Stir")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"}, {true, "for"}, {true, "num"}, {true, "minutes"} }, verbs))
				return 8;
			else if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "Ingri"}, {true, "into the"}, {false, "Bnum"}, {true, "mixing bowl"} }, verbs))
				return 17;
		}
		else if (check == "Mix")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {false, "Bnum"}, {false, "mixing bowl"}, {true, "well"} }, verbs))
				return 9;
		}
		else if (check == "Clean")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {false, "Bnum"}, {true, "mixing bowl"}}, verbs))
				return 10;
		}
		else if (check == "Pour")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "the"}, {true, "contents of the"}, {false, "Bnum"}, {true, "mixing bowl into the"}, {false, "Dnum"}, {true, "baking dish"} }, verbs))
				return 11;
		}
		else if (check == "Set")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {true, "aside"}}, verbs))
				return 12;
		}
		else if (check == "Serve")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {true, "with"},{true, "aux"} }, verbs))
				return 13;
		}
		else if (check == "Refrigerate")
		{
			if (CheckIfMethod(line, ingri, IngriPosition, bowlPosition, dishPosition, pos, currentLinePos, lineNum, { {false, "for"}, {false, "num"}, {false, "hours"}}, verbs))
				return 14;
		}
		pos++;
	}

	bowlPosition.push_back(stoi("0"));
	dishPosition.push_back(stoi("0"));

	if (CheckIfVerb(line, ingri, IngriPosition, lineNum, currentLinePos, verbs))
	{
		return 19;
	}
	else
	{
		return 18;
	}
}

void CheckVerbPairs(vector<string> methodCode, int lineNum, vector<int> keyWord, int endLine = 0)
{
	int errorLine = 0;
	int outsideVerbs = 0;

	if (!endLine)
	{
		endLine = methodCode.size();
	}

	while (lineNum < endLine)
	{
		if (keyWord[lineNum] == 18)
		{
			outsideVerbs++;
			if (outsideVerbs != 0)
			{
				errorLine = lineNum + lineNum;
			}
		}
		else if (keyWord[lineNum] == 19)
		{
			outsideVerbs--;
			if (outsideVerbs != 0)
			{
				errorLine = lineNum + lineNum;
			}
		}

		lineNum++;
	}

	if (outsideVerbs != 0)
	{
		ThrowError(12, errorLine);
	}
}

void DoTake(Ingredients &ingri, string toTake)
{
	string in;

	cout << "How much " << toTake << " to take from the refrigerator? ";
	cin >> in;

	ingri[toTake].ammount = stoi(in);
}

void DoPut(Ingredients &ingri, string toPut, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toPut))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	mixingBowls[currentBowl].push_back(ingri[toPut]);
}

void DoFold(Ingredients& ingri, string toDo, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	ingri[toDo].ammount = mixingBowls[currentBowl].back().ammount;
	mixingBowls[currentBowl].pop_back();
}

void DoAdd(Ingredients& ingri, string toAdd, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toAdd))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	mixingBowls[currentBowl].front().ammount += ingri[toAdd].ammount;
}

void DoRemove(Ingredients& ingri, string toDo, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	mixingBowls[currentBowl].front().ammount -= ingri[toDo].ammount;
}

void DoCombine(Ingredients& ingri, string toDo, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	mixingBowls[currentBowl].front().ammount *= ingri[toDo].ammount;
}

void DoDivide(Ingredients& ingri, string toDo, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	mixingBowls[currentBowl].front().ammount /= ingri[toDo].ammount;
}

void DoAddDry(Ingredients& ingri, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;
	int addNum = 0;
	IngredInfo newIng;

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	for (auto ing : ingri)
	{
		if (ing.second.type == 1)
		{
			addNum += ing.second.ammount;
		}
	}

	newIng.ammount = addNum;
	newIng.type = 1;

	mixingBowls[currentBowl].push_back(newIng);
}

void DoLiquify(Ingredients& ingri, string toDo, int lineNum)
{
	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	ingri[toDo].type = 2;
}

void DoStir(string strNum, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int num = stoi(strNum);
	int currentBowl;
	IngredInfo ing;

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	ing = mixingBowls[currentBowl].back();

	if (num > mixingBowls[currentBowl].size())
	{
		mixingBowls[currentBowl].emplace(mixingBowls[currentBowl].begin(), ing);
	}
	else
	{
		mixingBowls[currentBowl].emplace(mixingBowls[currentBowl].end() - (num + 1), ing);
	}

	mixingBowls[currentBowl].pop_back();
}

void DoMix(int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;
	int randVal;
	vector<IngredInfo> tempBowl;
	vector<int> alreadyChosen;
	srand(time(NULL));

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	while (tempBowl.size() != mixingBowls[currentBowl].size())
	{
		randVal = rand() % mixingBowls[currentBowl].size();

		if (!count(alreadyChosen.begin(), alreadyChosen.end(), randVal))
		{
			tempBowl.push_back(mixingBowls[currentBowl][randVal]);
			alreadyChosen.push_back(randVal);
		}
	}

	mixingBowls[currentBowl] = tempBowl;
}

void DoStirIng(Ingredients& ingri, string toDo, string strNum, int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int num = stoi(strNum);
	int currentBowl;
	IngredInfo ing;
	vector<IngredInfo> tempBowl;
	vector<IngredInfo> returnBowl;

	if (!ingri.count(toDo))
	{
		ThrowError(7, lineNum);
	}

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (num > mixingBowls[currentBowl].size())
	{
		mixingBowls[currentBowl].emplace(mixingBowls[currentBowl].begin(), ingri[toDo]);
	}
	else
	{
		mixingBowls[currentBowl].emplace(mixingBowls[currentBowl].end() - (num + 1), ingri[toDo]);
	}
}

void DoLiquifyContents(int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	if (mixingBowls[currentBowl].empty())
	{
		ThrowError(10, lineNum);
	}

	for (auto &ing : mixingBowls[currentBowl])
	{
		ing.type = 2;
	}
}

void DoClean(int bowlNum, int lineNum, vector<vector<IngredInfo>>& mixingBowls)
{
	int currentBowl;

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = 0;
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = bowlNum - 1;
	}

	mixingBowls[currentBowl].clear();
}

void DoPour(int bowlNum, int dishNum, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, int lineNum)
{
	vector<IngredInfo>* currentBowl = nullptr;
	vector<IngredInfo>* currentDish = nullptr;

	if (!bowlNum)
	{
		if (mixingBowls[1].empty())
		{
			currentBowl = &mixingBowls[0];
		}
		else
		{
			ThrowError(10, lineNum);
		}
	}
	else
	{
		currentBowl = &mixingBowls[bowlNum - 1];
	}

	if (!dishNum)
	{
		if (bakingDishes[1].empty())
		{
			currentDish = &bakingDishes[0];
		}
		else
		{
			ThrowError(11, lineNum);
		}
	}
	else
	{
		currentDish = &bakingDishes[dishNum - 1];
	}

	for (auto ing : *currentBowl)
	{
		currentDish->push_back(ing);
	}
}

void DoAux(vector<vector<IngredInfo>>& mixingBowls, vector<string> lines, string auxName, int lineNum)
{
	vector<IngredInfo> addBowl;

	addBowl = ParseAux(lines, auxName, lineNum);

	for (auto ing : mixingBowls[0])
	{
		cout << ing.ammount;
	}

	for (auto ing : addBowl)
	{
		mixingBowls[0].push_back(ing);
	}
}

bool DoMethod(vector<string> methodCode, vector<string> lines, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients, int lineNum, vector<int> keyWord, vector<int> bowlPosition, vector<int> dishPosition, vector<string> ingriPosition, vector<string> verbs, bool& ref, int currentLinePos = 0, int endLine = 0);

void DoVerb(vector<string> methodCode, vector<string> lines, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingri, int lineNum, int& currentLinePos, vector<int> keyWord, vector<int> bowlPosition, vector<int> dishPosition, vector<string> ingriPosition, vector<string> verbs, string verbToDo, string ingriToCheck, bool& ref)
{
	int i = currentLinePos + 1;
	int currentVerb = 0;
	int endLine = 0;
	bool endLoop = false;
	string tempCheck;

	if (!ingri.count(ingriToCheck))
	{
		ThrowError(7, lineNum);
	}

	while (i < methodCode.size())
	{
		if (verbToDo == verbs[i] + " " || verbToDo == verbs[i] + "e ")
		{
			endLine = i;
			break;
		}
		i++;
	}

	if (!endLine)
	{
		ThrowError(12, lineNum);
	}

	CheckVerbPairs(methodCode, currentLinePos + 1, keyWord, endLine);

	i = currentLinePos + 1;

	while (ingri[ingriToCheck].ammount)
	{
		if (!DoMethod(methodCode, lines, mixingBowls, bakingDishes, ingri, lineNum, keyWord, bowlPosition, dishPosition, ingriPosition, verbs, ref, i, endLine))
		{
			break;
		}

		if (ref)
		{
			break;
		}

		if (ingriPosition[endLine] != "")
		{
			ingri[ingriPosition[endLine]].ammount--;
		}
	}

	currentLinePos = endLine;
}

bool DoMethod(vector<string> methodCode, vector<string> lines, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients, int lineNum, vector<int> keyWord, vector<int> bowlPosition, vector<int> dishPosition, vector<string> ingriPosition, vector<string> verbs, bool& ref, int currentLinePos, int endLine)
{
	if (!endLine)
	{
		endLine = keyWord.size();
	}

	while(currentLinePos < endLine)
	{
		//cout << keyWord[currentLinePos] << " " << ingriPosition[currentLinePos] << ingredients[ingriPosition[currentLinePos]].ammount << " " << bowlPosition[currentLinePos] << " " << dishPosition[currentLinePos] << " " << verbs[currentLinePos] << endl;

		switch (keyWord[currentLinePos])
		{
		case 0:
		{
			DoTake(ingredients, ingriPosition[currentLinePos]);
			break;
		}
		case 1:
		{
			DoPut(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], (currentLinePos + lineNum), mixingBowls);
			break;
		}
		case 2:
		{
			DoFold(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 3:
		{
			DoAdd(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 4:
		{
			DoRemove(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 5:
		{
			DoCombine(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 6:
		{
			DoDivide(ingredients, ingriPosition[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 7:
		{
			DoLiquify(ingredients, ingriPosition[currentLinePos], lineNum);
			break;
		}
		case 8:
		{
			DoStir(verbs[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 9:
		{
			DoMix(bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 10:
		{
			DoClean(bowlPosition[currentLinePos],lineNum, mixingBowls);
			break;
		}
		case 11:
		{
			DoPour(bowlPosition[currentLinePos], dishPosition[currentLinePos], mixingBowls, bakingDishes, (currentLinePos + lineNum));
			break;
		}
		case 12:
		{
			return false;
		}
		case 13:
		{
			DoAux(mixingBowls, lines, verbs[currentLinePos], lineNum);
			break;
		}
		case 14:
		{
			ref = true;
			break;
		}
		case 15:
		{
			DoAddDry(ingredients, bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 16:
		{
			DoLiquifyContents(bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 17:
		{
			DoStirIng(ingredients, ingriPosition[currentLinePos], verbs[currentLinePos], bowlPosition[currentLinePos], lineNum, mixingBowls);
			break;
		}
		case 18:
		{
			DoVerb(methodCode, lines, mixingBowls, bakingDishes, ingredients, (lineNum + currentLinePos), currentLinePos, keyWord, bowlPosition, dishPosition, ingriPosition, verbs, verbs[currentLinePos], ingriPosition[currentLinePos], ref);
			break;
		}
		}

		currentLinePos++;

		if (ref)
		{
			break;
		}
	}
	return true;
}

void InterpretMethod(vector<string> methodCode, int lineNum, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients, vector<string> lines)
{
	vector<int> keyWord;	//0: Take, 1: Put, 2: Fold, 3: Add, 4: Remove, 5: Combine, 6: Devide, 7: Liquefy, 8: Stir, 9: Mix, 10: Clean, 11: Pour, 12: Set, 13: Serve, 14: Refrigerate, 15: Add (dry), 16: Liquefy (contents), 17: Stir ({Ingridient}), 18: {verb}, 19: {verb} (until)
	vector<string> ingriPosition;
	vector<string> verbs;
	vector<int> bowlPosition;
	vector<int> dishPosition;
	int currentLinePos = 0;
	int tempCOUNT = 0;
	bool ref = false;

	LoadUtinsils(bakingDishes, mixingBowls);

	for (string line : methodCode)
	{
		currentLinePos++;
		keyWord.push_back(KeyWordType(line, ingredients, ingriPosition, bowlPosition, dishPosition,currentLinePos,lineNum, verbs));
	}

	/*for (int key : keyWord)
	{
		wcout << key << " ";
		cout << ingriPosition[tempCOUNT] << " ";
		cout << bowlPosition[tempCOUNT] << " ";
		cout << dishPosition[tempCOUNT] << " ";
		cout << verbs[tempCOUNT] << endl;
		tempCOUNT++;
	}*/

	CheckVerbPairs(methodCode, 0, keyWord);

	if (!DoMethod(methodCode, lines, mixingBowls, bakingDishes, ingredients, lineNum, keyWord, bowlPosition, dishPosition, ingriPosition, verbs, ref))
	{
		ThrowError(13,lineNum);
	}

	//bakingDishes[0].push_back(ingredients["sugar"]);
}