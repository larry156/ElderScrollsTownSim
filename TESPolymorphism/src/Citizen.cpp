/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base Citizen class. */

#include "Citizen.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "Town.h"

using namespace std;

Citizen::Citizen(string myName, string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount) : name(myName), species(mySpecies), taxAmount(speciesTaxAmount)
{
	// If min and max are equal, or max is less than min for some reason, set money to startMoneyMin.
	if (startMoneyMin == startMoneyMax || startMoneyMax < startMoneyMin)
	{
		money = startMoneyMin;
	}
	// Otherwise randomize money to a value between min and max (inclusive)
	else
	{
		money = rand() % (startMoneyMax - startMoneyMin + 1) + startMoneyMin;
	}

	isDead = false;
	hasLeft = false;
	// This constructor has no console messages.
}
Citizen::~Citizen()
{
	// Each type of citizen will have its own unique deconstructor message.
}

// Get various properties
string Citizen::getName()
{
	return name;
}
string Citizen::shortName() // Return their first name.
{
	// Find the index of the first space character
	size_t firstSpacePos = name.find(" ");
	return name.substr(0, firstSpacePos);
}
string Citizen::getSpecies()
{
	return species;
}
string Citizen::getProfession()
{
	return profession;
}
int Citizen::checkWealth()
{
	return money;
}
bool Citizen::getDead()
{
	return isDead;
}
bool Citizen::isLeaving()
{
	return hasLeft;
}
int Citizen::getCombatSkill()
{
	return combatSkill;
}
int Citizen::getJobSkill()
{
	return jobSkill;
}

// As the name implies, used for combat. Returns a random number between 0 and 100 + this Citizen's combatSkill.
int Citizen::combatRoll()
{
	int roll = rand() % 101 + combatSkill;
	return roll;
}

// Paying money to various entities, as well as getting paid.
void Citizen::payPerson(int toPay, Citizen* recipient, bool requireSufficientMoney, bool isTheft)
{
	// Theft and non-theft have different messages, if requireSuffiecientMoney is false, this person will just give the recipient
	// all of their money if they don't have enough to pay the recipient's requested amount.
	if (isDead)
	{
		cout << getName() << " is dead. Dead people can't give people money." << endl;
	}
	else if (money < toPay && requireSufficientMoney)
	{
		cout << getName() << " doesn't have enough Gold to pay " << recipient->getName() << "." << endl;
	}
	else if (money >= toPay)
	{
		money -= toPay;
		recipient->getPaid(toPay, false);
		cout << getName() << " has paid " << toPay << " Gold to " << recipient->getName() << "." << endl;
	}
	else
	{
		int actualPaid = money;
		money = 0;
		recipient->getPaid(actualPaid, false);
		cout << getName() << " has paid " << actualPaid << " Gold to " << recipient->getName() << "." << endl;
	}
}
void Citizen::payTaxes()
{
	if (isDead)
	{
		cout << getName() << " is dead. Dead people don't pay taxes." << endl;
	}
	else
	{
		int taxToPay = taxAmount + (checkWealth() * 3 / 100);

		if (money >= taxToPay)
		{
			money -= taxToPay;
			cout << getName() << " has paid " << taxToPay << " Gold in taxes." << endl;
		}
		else
		{
			cout << getName() << " has been executed for not paying taxes!" << endl;
			kill();
		}
	}
}
void Citizen::getPaid(int toPay, bool showMessage)
{
	money += toPay;
	if (showMessage) // Sometimes we don't want to show this message.
	{
		cout << getName() << " has received " << toPay << " Gold." << endl;
	}
}

// Speech. Randomly choose a line of dialogue and say it, replacing %these% when necessary.
void Citizen::speak()
{
	if (!isDead)
	{
		// Roll for dialogue
		int dialogueRoll = rand() % dialogue.size();
		int deityRoll = rand() % deities.size();
		string chosenDialogue = dialogue[dialogueRoll];

		// Replace %these%
		replaceString(chosenDialogue, "%target%", curTarget->getName());
		replaceString(chosenDialogue, "%targetfirst%", curTarget->shortName());
		replaceString(chosenDialogue, "%deity%", deities[deityRoll]);

		// Say the line
		cout << getName() << ": " << chosenDialogue << endl;
	}
}


// This function kills the citizen and displays a message. Different races may do different things when they die.
void Citizen::kill()
{
	isDead = true;
	cout << getName() << " has met their demise." << endl;
}
void Citizen::leave()
{
	hasLeft = true;
	cout << getName() << " has left the town." << endl;
}

// Used to create targets within the town for Dark Brotherhood assassins
void Citizen::blackSacrament()
{
	cout << getName() << " creates an effigy of " << curTarget->getName() << "..." << endl;
	cout << getName() << ": Sweet Mother, sweet Mother, send your child unto me, for the sins of the unworthy must be baptized in blood and fear..." << endl;
	bool alreadyATarget = false;
	for (int i = 0; i < myTown->blackSacramentTargets.size(); i++)
	{
		if (myTown->blackSacramentTargets[i] == curTarget)
		{
			alreadyATarget = true;
			break;
		}
	}
	if (alreadyATarget || curTarget->getProfession() == "Assassin")
	{
		cout << "An unknown force has observed this event and ignored it." << endl;
	}
	else if (!alreadyATarget)
	{
		myTown->blackSacramentTargets.push_back(curTarget);
		cout << "An unknown force has observed this event." << endl;
	}
}

// Find something in a string and replace it. Used heavily for dialogue.
void Citizen::replaceString(string &theString, string replaceThis, string replaceWith)
{
	if (replaceThis == "")
	{
		return;
	}
	size_t pos = 0;
    while ((pos = theString.find(replaceThis)) != string::npos)
	{
		theString.replace(pos, replaceThis.length(), replaceWith);
		pos += replaceWith.length();
	}
}

// List this Citizen's stats.
void Citizen::listStats()
{
	cout << getName() << ": " << getSpecies() << " Profession: " << profession << endl;
	cout << "Wealth: " << checkWealth() << " combatSkill: " << combatSkill << " jobSkill: " << jobSkill << endl;
}

// Assign this Citizen to a Town
void Citizen::setResidence(Town* newTown)
{
	myTown = newTown;
}
