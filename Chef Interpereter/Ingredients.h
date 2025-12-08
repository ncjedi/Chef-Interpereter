#pragma once
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include "IngredInfo.h"
#include "ErrorHandling.h"

using namespace std;

typedef::unordered_map<string, IngredInfo> Ingredients;

void GetIngridient(Ingredients& ingredients, string line, int lineNum);