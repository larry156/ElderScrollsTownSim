/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The Nords of Skyrim. They have some unique deities, unique dialogue, and may hunt an animal or brawl a citizen. */

#include "Nord.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>

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

	speakChance = rand() % 21 + 50; // Nords have a 50-70% chance of speaking.
	brawlChance = rand() % 101; // Nords have a 0-100% chance of brawling someone.
	combatSkill = rand() % 21 + 10; // Nords have a combatSkill between 10-30.

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
	// Set curTarget to the current target and roll for actions
	curTarget = target;
	int speakRoll = rand() % 100;
	int brawlRoll = rand() % 100;
	int huntRoll = rand() % 2;

	// Now do upkeep
	payTaxes();
	if (speakRoll < speakChance)
	{
		speak();
	}
	if (brawlRoll < brawlChance)
	{

	}
	else if (huntRoll == 0)
	{
		hunt();
	}

	cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
}

// Nords are able to hunt animals for money. They have a 50% chance of doing this if they don't brawl someone.
void Nord::hunt()
{
	// Fails if combatRoll < 30, critically fails (i.e. dies) if < 10
	const int FAIL_THRESHOLD = 30, CRITICAL_FAIL = 10;
	const vector<string> ANIMALS = {"Deer", "Elk", "Troll", "Bear"}; // Vector is used here because it knows its size.
	int animalToHunt = rand() % ANIMALS.size();

	cout << getName() << " is hunting a " << ANIMALS[animalToHunt] << "." << endl;
	if (combatRoll() < CRITICAL_FAIL)
	{
		cout << "The " << ANIMALS[animalToHunt] << " has gravely wounded " << getName() << "!" << endl;
		kill();
	}
	else if (combatRoll() < FAIL_THRESHOLD)
	{
		int deityToInvoke = rand() % deities.size();
		cout << getName() << ": By " << deities[deityToInvoke] << ", I haven't seen a single " << ANIMALS[animalToHunt] << " today!" << endl;
	}
	else
	{
		cout << getName() << ": This " << ANIMALS[animalToHunt] << " pelt is bound to be worth some gold at market." << endl;
		int salePrice = rand() % 7 + 3; // Should be 3-9 gold.
		getPaid(salePrice);
	}
}
