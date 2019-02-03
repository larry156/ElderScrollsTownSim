/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base Human class. */

#include "Human.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Human::Human(string myName, string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount) : Citizen(myName, mySpecies, startMoneyMin, startMoneyMax, speciesTaxAmount)
{
	deities.push_back("Akatosh");
	deities.push_back("Arkay");
	deities.push_back("Dibella");
	deities.push_back("Julianos");
	deities.push_back("Kynareth");
	deities.push_back("Mara");
	deities.push_back("Stendarr");
	deities.push_back("Zenithar");

	cout << "A human named " << myName << " has moved into town." << endl;
}

Human::~Human()
{
	// Deconstructor messages are left for the individual human races.
}
