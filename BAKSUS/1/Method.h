#pragma once
#include <vector>
#include <string>
#include "Ingredients.h"

using namespace std;

void InterpretMethod(vector<string> methodCode, int lineNum, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients);