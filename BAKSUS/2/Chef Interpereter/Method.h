#pragma once
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Ingredients.h"

using namespace std;

void InterpretMethod(vector<string> methodCode, int lineNum, vector<vector<IngredInfo>>& mixingBowls, vector<vector<IngredInfo>>& bakingDishes, Ingredients& ingredients, vector<string> lines);