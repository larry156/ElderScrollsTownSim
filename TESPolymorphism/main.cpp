/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: Polymorphism using Elder Scrolls races. */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <time.h>
#include <windows.h>

#include "Town.h"
#include "Citizen.h"
#include "Human.h"
#include "Mer.h"
#include "BeastRace.h"
#include "Nord.h"
#include "Imperial.h"
#include "Redguard.h"
#include "Altmer.h"

using namespace std;

// This function will allow new citizens to move into the town.
// It requires the town array, a map storing name vectors, and the citizen's index in the array.
/* void moveIn(Citizen* townArray[], map<string, vector<string>> &nameListsFirst, map<string, vector<string>> &nameListsLast, int indexPos)
{
    int raceRoll = rand() % nameListsFirst.size();
    //raceRoll = 1;
    if (raceRoll == 0)
    {
        townArray[indexPos] = new Nord(nameGen(nameListsFirst["Nord"]) + " " + nameGen(nameListsLast["Nord"]));
    }
    else if (raceRoll == 1)
    {
        townArray[indexPos] = new Imperial(nameGen(nameListsFirst["Imperial"]) + " " + nameGen(nameListsLast["Imperial"]));
    }
    else if (raceRoll == 2)
	{
		const vector<string> PREFIXES = {"at-", "af-"};
		int prefixRoll = rand() % PREFIXES.size();
		townArray[indexPos] = new Redguard(nameGen(nameListsFirst["Redguard"]) + " " + PREFIXES[prefixRoll] + nameGen(nameListsFirst["Redguard"]));
	}
	else if (raceRoll == 3)
	{
		int isHoRMember = rand() % 3; // 33% chance of being House of Reveries. (Not enough unique names)
		if (isHoRMember == 0)
		{
			townArray[indexPos] = new Altmer(nameGen(nameListsLast["HouseOfReveries"]), true);
		}
		else
		{
			townArray[indexPos] = new Altmer(nameGen(nameListsFirst["Altmer"]) + " " + nameGen(nameListsLast["Altmer"]));
		}
	}
    cout << endl;
} */

int main()
{
	srand(time(NULL));

	// Get console handle & set up colours.
    // Colours may look different on some computers.
    HANDLE consHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const int C_GRAY = 8, C_TURQUOISE = 3, C_YELLOW = 14, C_DEFAULT = 15, C_ERROR = 244, C_WARNING = 12;
    SetConsoleTextAttribute(consHandle, C_DEFAULT); // For some reason the default console colour is actually a light shade of gray.

	// Should be at least 4, must be greater than 1.
	int townSize = 7;
	// Make sure the number of citizens is reasonable.
	if (townSize < 1)
	{
		SetConsoleTextAttribute(consHandle, C_ERROR);
		cout << "Error: Number of citizens must be greater than 1. Exiting..." << endl;
		return 1;
	}
	else if (townSize < 4)
	{
		SetConsoleTextAttribute(consHandle, C_WARNING);
		cout << "Warning: Town size should be at least 4." << endl;
	}
	// The largest town size tested was 200 people, and while it DOES work, at first glance, there's no reason for the town size to be so high.
	else if (townSize > 50)
	{
		SetConsoleTextAttribute(consHandle, C_WARNING);
		cout << "Warning: Very large town sizes are not supported. Enter \"IGNORE\" to ignore this and continue (Not recommended)." << endl;
		SetConsoleTextAttribute(consHandle, C_DEFAULT);

		string ignoreThis;
		cout << "Ignore? ";
		getline(cin, ignoreThis);
		if (ignoreThis != "IGNORE")
		{
			cout << "Exiting..." << endl;
			return 1;
		}
		else
		{
			cout << "Ignoring and continuing..." << endl;
		}
	}
	else if (townSize > 10)
	{
		SetConsoleTextAttribute(consHandle, C_WARNING);
		cout << "Warning: It may be hard to keep track of the town with sizes over 10." << endl;
	}
	Town* theTown = new Town(townSize, true);

	// Read in names from text files and store them in 2 vectors; one for first names, one for last names.
    // Names were generated using fantasynamegenerators.com
    //ifstream namesNordFirst("Names/nordNamesFirst.txt"), namesNordLast("Names/nordNamesFirst.txt");
    //map<string, vector<string>> nameListsFirst, nameListsLast;
    cout << "Compiling list of names..." << endl;
    // Read in names
    if (theTown->readNames("Names/nordNamesFirst.txt", theTown->nameListsFirst["Nord"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/nordNamesLast.txt", theTown->nameListsLast["Nord"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/imperialNamesFirst.txt", theTown->nameListsFirst["Imperial"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/imperialNamesLast.txt", theTown->nameListsLast["Imperial"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/redguardNames.txt", theTown->nameListsFirst["Redguard"])) // Redguards are named a bit differently than the other races.
	{
		return 1;
	}
	if (theTown->readNames("Names/altmerNamesFirst.txt", theTown->nameListsFirst["Altmer"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/altmerNamesLast.txt", theTown->nameListsLast["Altmer"]))
	{
		return 1;
	}
	if (theTown->readNames("Names/HoRNames.txt", theTown->nameListsLast["HouseOfReveries"])) // Only used by Altmer for now, might be used by some other races later.
	// Uses nameListsLast because nameListsFirst is used to determine number of races.
	{
		return 1;
	}
	//cout << "There are " << nameListsFirst.size() << " races." << endl;
	SetConsoleTextAttribute(consHandle, C_DEFAULT);

	theTown->initializeTown();
	theTown->runSimulation();

	delete theTown;
    SetConsoleTextAttribute(consHandle, C_DEFAULT);

    return 0;
}
