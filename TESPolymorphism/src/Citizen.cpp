/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base Citizen class. */

#include "Citizen.h"
#include <iostream>
#include <cstdlib>

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
string Citizen::getSpecies()
{
	return species;
}
int Citizen::checkWealth()
{
	return money;
}
bool Citizen::getDead()
{
	return isDead;
}

// Paying money to various entities, as well as getting paid.
void Citizen::payPerson(int toPay, Citizen* recipient, bool requireSufficientMoney = false, bool isTheft = false)
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
	else if (money >= taxAmount)
	{
		money -= taxAmount;
		cout << getName() << " has paid " << taxAmount << " Gold in taxes." << endl;
	}
	else
	{
		cout << getName() << " has been executed for not paying taxes!" << endl;
		kill();
	}
}
void Citizen::getPaid(int toPay, bool showMessage = true)
{
	money += toPay;
	if (showMessage) // Sometimes we don't want to show this message.
	{
		cout << getName() << " has received " << toPay << " Gold." << endl;
	}
}

// This function kills the citizen and displays a message. Different races may do different things when they die.
void Citizen::kill()
{
	isDead = true;
	cout << getName() << " has met their demise." << endl;
}
