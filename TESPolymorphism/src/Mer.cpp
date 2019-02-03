/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base Mer class. */

#include "Mer.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Mer::Mer(string myName, string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount) : Citizen(myName, mySpecies, startMoneyMin, startMoneyMax, speciesTaxAmount)
{
	deities.push_back("Auri-El");
	deities.push_back("Jephre");
	deities.push_back("Magnus");
	deities.push_back("Mara");
	deities.push_back("Phynaster");
	deities.push_back("Stendarr");
	deities.push_back("Syrabane");
	deities.push_back("Trinimac");
	deities.push_back("Xarxes");
	deities.push_back("Xen");

	cout << "An elf named " << myName << " has moved into town." << endl;
}

Mer::~Mer()
{
	// Deconstructor messages are left for the individual elven races.
}
