/* Name: Larry Y.
 * Date: April 16, 2019
 * Desc: The Town class. */

#include "Town.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <Altmer.h>
#include <Nord.h>
#include <Imperial.h>
#include <Redguard.h>

using namespace std;

Town::Town(int townSize, bool doWelfare) : preferredSize(townSize), giveWelfare(doWelfare)
{
	consHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	curDay = 0;
	curMonth = 0;
	curYear = 1;
}

Town::~Town()
{
	SetConsoleTextAttribute(consHandle, C_GRAY);
	for (int i = 0; i < myCitizens.size(); i++)
    {
        delete myCitizens[i];
    }
}

// Get a random citizen from the town
Citizen* Town::getRandomCitizen()
{
	int citizenIndex = rand() % myCitizens.size();
	return myCitizens[citizenIndex];
}
// Get a random citizen that is not the one passed in as a parameter
int Town::getTarget(Citizen* curCitizen)
{
	int citizenIndex, targetIndex;
	// First find the position of curCitizen
	for (int i = 0; i < myCitizens.size(); i++)
	{
		if (curCitizen == myCitizens[i])
		{
			citizenIndex = i;
			targetIndex = citizenIndex;
		}
	}
	// Then randomly pick a citizen until the citizen picked isn't curCitizen.
	while (targetIndex == citizenIndex)
	{
		targetIndex = rand() % myCitizens.size();
	}
	return targetIndex;
}
// Return the whole town as a vector of citizens
/* vector<Citizen*> Town::getResidents()
{
	return myCitizens;
} */

// This function will store the contents of a file into a vector of strings. It returns true if there was an error.
bool Town::readNames(string fileName, vector<string> &theList)
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
// This function will return a random string from the vector passed in.
string Town::nameGen(vector<string> &nameList)
{
	int nameIndex = rand() % nameList.size();
	return nameList[nameIndex];
}

// This function will allow new citizens to move into the town. It will add the new citizen to the end of the residents list.
void Town::moveIn()
{
	//cout << "Hey" << endl;
	SetConsoleTextAttribute(consHandle, C_GRAY);
	int raceRoll = rand() % nameListsFirst.size();
	Citizen* newResident;
    //raceRoll = 1;
    //cout << raceRoll << endl;
    if (raceRoll == 0)
    {
        myCitizens.push_back(new Nord(nameGen(nameListsFirst["Nord"]) + " " + nameGen(nameListsLast["Nord"])));
    }
    else if (raceRoll == 1)
    {
        myCitizens.push_back(new Imperial(nameGen(nameListsFirst["Imperial"]) + " " + nameGen(nameListsLast["Imperial"])));
    }
    else if (raceRoll == 2)
	{
		const vector<string> PREFIXES = {"at-", "af-"};
		int prefixRoll = rand() % PREFIXES.size();
		myCitizens.push_back(new Redguard(nameGen(nameListsFirst["Redguard"]) + " " + PREFIXES[prefixRoll] + nameGen(nameListsFirst["Redguard"])));
	}
	else if (raceRoll == 3)
	{
		int isHoRMember = rand() % 3; // 33% chance of being House of Reveries. (Not enough unique names)
		if (isHoRMember == 0)
		{
			myCitizens.push_back(new Altmer(nameGen(nameListsLast["HouseOfReveries"]), true));
		}
		else
		{
			myCitizens.push_back(new Altmer(nameGen(nameListsFirst["Altmer"]) + " " + nameGen(nameListsLast["Altmer"])));
		}
	}
    cout << endl;
}
/* void moveOut(int indexPos)
{

} */
// Bury all the dead residents of the town. This function does not move in any citizens to replace deceased ones.
void Town::clearDeadResidents()
{
	SetConsoleTextAttribute(consHandle, C_GRAY);
	for (int i = 0; i < myCitizens.size(); i++)
	{
		if (myCitizens[i]->getDead())
		{
			delete myCitizens[i];
			myCitizens.erase(myCitizens.begin() + i);
			cout << endl;
		}
	}
}

// Initialize the town, moving in the preferred amount of residents to begin with.
void Town::initializeTown()
{
	cout << "Populating the town..." << endl << endl;
	for (int i = 0; i < preferredSize; i++)
	{
		moveIn();
	}
}

// The main upkeep function of the town.
void Town::runSimulation()
{
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

		// Get rid of all the dead people. Loop checks to see if any citizens are dead.
		for (int i = 0; i < myCitizens.size(); i++) // Probably very inefficient but whatever
		{
			if (myCitizens[i]->getDead())
			{
				clearDeadResidents();
				break;
			}
		}
		// New people move in at the start of each day, if there is space in the town.
		if (myCitizens.size() < preferredSize)
		{
			moveIn();
		}

    	// Do upkeep actions for each person.
    	SetConsoleTextAttribute(consHandle, C_DEFAULT);
		for (int i = 0; i < myCitizens.size(); i++)
		{
			// If a citizen was dead at the start of their upkeep, then two empty lines would be output to the console. This prevents that.
			bool skipFormatting = myCitizens[i]->getDead();

            // Giving citizens money.
            if (giveWelfare)
            {
                int payRoll = rand() % 5; // 20% chance
				if (payRoll == 0 && !myCitizens[i]->getDead())
                {
                    int toPay = rand() % 5 + 1; // Citizen gets paid $1 - $5
                    myCitizens[i]->getPaid(toPay);
                }
            }

			// Citizen chooses a random target that isn't themselves and performs an action
			int rngTarget = getTarget(myCitizens[i]);
			//cout << theTown[i]->getName() << " targeted a " << theTown[i]->getSpecies() << endl;
			if (!myCitizens[i]->getDead())
			{
				myCitizens[i]->upkeep(myCitizens[rngTarget]);
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
}
