/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The Nords of Skyrim. They have some unique deities, unique dialogue, and  */

#include "Nord.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

Nord::Nord(string myName) : Human(myName, "Nord", 10, 20, 2)
{
	// Remove deities from the pantheon. These are probably going to be replaced with something else.
	deities.erase(find(deities.begin(), deities.end(), "Kynareth"));

	// Add some Nordic deities
	deities.push_back("Shor");
	deities.push_back("Ysmir");
	deities.push_back("Kyne"); // Kyne is a different version of Kynareth.

	// Nord-specific dialogue
	dialogue.push_back("Someone steal your sweetroll, %target%?");
	dialogue.push_back("I used to be an adventurer like you. Then I took an arrow in the knee.");
	dialogue.push_back("%target% seems like a milk-drinker to me.");
	dialogue.push_back("Have you seen those warriors from Hammerfell? They have curved swords.");

	cout << getName() << " is a Nord of Skyrim." << endl;
}
Nord::~Nord()
{
	cout << getName() << " was entombed in the Hall of the Dead." << endl;
}

// Speech. Randomly choose a line of dialogue and say it, replacing %these% when necessary.
void Nord::speak()
{
	if (!isDead)
	{
		// Roll for dialogue
		int dialogueRoll = rand() % dialogue.size();
		string chosenDialogue = dialogue[dialogueRoll];

		// Replace %these%
		replaceString(chosenDialogue, "%target%", curTarget->getName());

		// Say the line
		cout << getName() << ": " << chosenDialogue << endl;
	}
}

// Each upkeep, Nords will speak, pay taxes, and perform an action.
void Nord::upkeep(Citizen* target)
{
	// Set curTarget to the current target
	curTarget = target;

	// Now do upkeep
	payTaxes();
	speak();
}
