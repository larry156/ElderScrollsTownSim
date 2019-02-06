/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base Human class. */

#include "Human.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Human::Human(string myName, string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount) : Citizen(myName, mySpecies, startMoneyMin, startMoneyMax, speciesTaxAmount)
{
	// The 8 Divines are pretty standard among most humans
	deities.push_back("Akatosh");
	deities.push_back("Arkay");
	deities.push_back("Dibella");
	deities.push_back("Julianos");
	deities.push_back("Kynareth");
	deities.push_back("Mara");
	deities.push_back("Stendarr");
	deities.push_back("Zenithar");

	// Add some generic dialogue
	dialogue.push_back("The Empire isn't what it used to be, that's for sure.");
	dialogue.push_back("Hey, %targetfirst%, you aren't a daedric cultist, are you?");

	cout << "A human named " << myName << " has moved into town." << endl;
}

Human::~Human()
{
	// Deconstructor messages are left for the individual human races.
}
