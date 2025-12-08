#include "ErrorHandling.h"

using namespace std;

void ThrowError(int error, int lineNum)
{
	string printError = "";

	switch (error)
	{
	case 1:
	{
		printError = "Recipe Name Not Found";
		break;
	}
	case 2:
	{
		printError = "No Recipe Found";
		break;
	}
	case 3:
	{
		printError = "No Ingredients Found";
		break;
	}
	case 4:
	{
		printError = "No Method Found";
		break;
	}
	case 5:
	{
		printError = "Serves Method Incorrect";
		break;
	}
	case 6:
	{
		printError = "Expected Integer";
		break;
	}
	case 7:
	{
		printError = "Invalid Ingredient";
		break;
	}
	default:
		printError = "Unknown Error";
		break;
	}

	cout << "ERROR: " << printError << " at line number " << lineNum;
	getchar();
	exit(0);
}