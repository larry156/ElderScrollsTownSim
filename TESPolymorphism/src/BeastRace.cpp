/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The base BeastRace class. Unlike the human and elven races, beast races don't really have much in common with each other. */

#include "BeastRace.h"
#include <iostream>
#include <cstdlib>

using namespace std;

BeastRace::BeastRace(string myName, string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount) : Citizen(myName, mySpecies, startMoneyMin, startMoneyMax, speciesTaxAmount)
{
	// Argonians and Khajiit have their own constructor messages and deities.
}

BeastRace::~BeastRace()
{
	// Argonians and Khajiit have their own deconstructor messages.
}
