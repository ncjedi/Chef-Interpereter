#include "Method.h"

using namespace std;

void InterpretMethod(vector<string> methodCode, int lineNum, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients)
{
	bakingDishes.push_back(vector<IngredInfo>{ingredients["chocolate sprinkles"]});
}