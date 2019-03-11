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

#include "Citizen.h"
#include "Human.h"
#include "Mer.h"
#include "BeastRace.h"
#include "Nord.h"
#include "Imperial.h"
#include "Redguard.h"

using namespace std;

// This function will return a random string from the vector passed in.
string nameGen(vector<string> &nameList)
{
	int nameIndex = rand() % nameList.size();
	return nameList[nameIndex];
}

// This function will allow new citizens to move into the town.
// It requires the town array, a map storing name vectors, and the citizen's index in the array.
void moveIn(Citizen* townArray[], map<string, vector<string>> &nameListsFirst, map<string, vector<string>> &nameListsLast, int indexPos)
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
    cout << endl;
}

// This function will store the contents of a file into a vector of strings. It returns true if there was an error.
bool readNames(string fileName, vector<string> &theList)
{
	ifstream readFrom(fileName);
	string curName = "this is wrong";
	while (curName != "") // Please ensure there is an empty line at the end of the text file.
	{
		getline(readFrom, curName);
		if (curName != "")
		{
			theList.push_back(curName);
		}
		// If curName is still "this is wrong", then the name file must not exist.
		if (curName == "this is wrong")
		{
			cout << "Error: " << fileName << " was not found. Exiting..." << endl;
			return true;
		}
	}
	return false;
}

int main()
{
	srand(time(NULL));

	// Get console handle & set up colours.
    // Colours may look different on some computers.
    HANDLE consHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const int C_GRAY = 8, C_TURQUOISE = 3, C_YELLOW = 14, C_DEFAULT = 15, C_ERROR = 244, C_WARNING = 12;
    SetConsoleTextAttribute(consHandle, C_DEFAULT); // For some reason the default console colour is actually a light shade of gray.

    // Read in names from text files and store them in 2 vectors; one for first names, one for last names.
    // Names were generated using fantasynamegenerators.com
    //ifstream namesNordFirst("Names/nordNamesFirst.txt"), namesNordLast("Names/nordNamesFirst.txt");
    map<string, vector<string>> nameListsFirst, nameListsLast;
    cout << "Compiling list of names..." << endl;
    // Read in names
    if (readNames("Names/nordNamesFirst.txt", nameListsFirst["Nord"]))
	{
		return 1;
	}
	if (readNames("Names/nordNamesLast.txt", nameListsLast["Nord"]))
	{
		return 1;
	}
	if (readNames("Names/imperialNamesFirst.txt", nameListsFirst["Imperial"]))
	{
		return 1;
	}
	if (readNames("Names/imperialNamesLast.txt", nameListsLast["Imperial"]))
	{
		return 1;
	}
	if (readNames("Names/redguardNames.txt", nameListsFirst["Redguard"])) // Redguards are named a bit differently than the other races.
	{
		return 1;
	}

	//cout << "There are " << nameListsFirst.size() << " races." << endl;

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

	SetConsoleTextAttribute(consHandle, C_DEFAULT);

	Citizen* theTown[townSize];

	// Whether or not citizens might get some money each day.
    const bool GIVE_WELFARE = true;

	// Set up the town.
	cout << "Populating the town..." << endl << endl;
	SetConsoleTextAttribute(consHandle, C_GRAY);
	for (int i = 0; i < townSize; i++)
	{
		moveIn(theTown, nameListsFirst, nameListsLast, i);
	}

	// For keeping track of days elapsed & whether or not to keep simulating.
	const int DAYS_IN_A_MONTH = 30;
	int curDay = 0, curMonth = 0, curYear = 1; // It takes a really long time to actually get past a year, but whatever.
	const string MONTHS[] = {"Morning Star", "Sun's Dawn", "First Seed", "Rain's Hand", "Second Seed", "Mid Year", "Sun's Height", "Last Seed", "Hearthfire", "Frost Fall", "Sun's Dusk", "Evening Star"};
    string keepGoing = "";
	// The main loop. Each run through is a day.
    while (keepGoing != "no" && keepGoing != "No")
    {
    	// Increment day/month/year (not in that order)
    	if (curDay >= DAYS_IN_A_MONTH)
		{
			curDay = 0;
			curMonth++;
		}
		if (curMonth >= 12) // There will always be 12 months in a year, which is why this isn't a variable.
		{
			curMonth = 0;
			curYear++;
		}
        curDay++;

        // What's the date today?
    	SetConsoleTextAttribute(consHandle, C_YELLOW);
    	if (curDay % 10 == 1 && curDay != 11)
		{
			cout << "It is the " << curDay << "st of " << MONTHS[curMonth] << ", Year " << curYear << "." << endl << endl;
		}
		else if (curDay % 10 == 2 && curDay != 12)
		{
			cout << "It is the " << curDay << "nd of " << MONTHS[curMonth] << ", Year " << curYear << "." << endl << endl;
		}
		else if (curDay % 10 == 3 && curDay != 13)
		{
			cout << "It is the " << curDay << "rd of " << MONTHS[curMonth] << ", Year " << curYear << "." << endl << endl;
		}
		else
		{
			cout << "It is the " << curDay << "th of " << MONTHS[curMonth] << ", Year " << curYear << "." << endl << endl;
		}

		// New people move in at the start of each day, if someone's died.
    	SetConsoleTextAttribute(consHandle, C_GRAY);
		for (int i = 0; i < townSize; i++)
		{
			if (theTown[i]->getDead())
			{
				delete theTown[i];
				moveIn(theTown, nameListsFirst, nameListsLast, i);
			}
		}

    	// Do upkeep actions for each person.
    	SetConsoleTextAttribute(consHandle, C_DEFAULT);
		for (int i = 0; i < townSize; i++)
		{
			// If a citizen was dead at the start of their upkeep, then two empty lines would be output to the console. This prevents that.
			bool skipFormatting = theTown[i]->getDead();

            // Giving citizens money.
            if (GIVE_WELFARE)
            {
                int payRoll = rand() % 5; // 20% chance
				if (payRoll == 0 && !theTown[i]->getDead())
                {
                    int toPay = rand() % 5 + 1; // Citizen gets paid $1 - $5
                    theTown[i]->getPaid(toPay);
                }
            }

			// Citizen chooses a random target that isn't themselves and performs an action
			int rngTarget = i;
			while (rngTarget == i)
			{
				rngTarget = rand() % townSize;
			}
			//cout << theTown[i]->getName() << " targeted a " << theTown[i]->getSpecies() << endl;
			if (!theTown[i]->getDead())
			{
				theTown[i]->upkeep(theTown[rngTarget]);
			}

			if (!skipFormatting)
			{
				cout << endl;
			}
		}
    	// The end of the day. Prompt user whether or not to continue.
    	SetConsoleTextAttribute(consHandle, C_TURQUOISE);
        cout << "The day has ended. Observe the next? (Yes/No): ";
        getline(cin, keepGoing);
        cout << endl;
    }

	// Clean up the town and reset console colours.
    SetConsoleTextAttribute(consHandle, C_GRAY);
	for (int i = 0; i < townSize; i++)
    {
        delete theTown[i];
    }
    SetConsoleTextAttribute(consHandle, C_DEFAULT);

    return 0;
}
