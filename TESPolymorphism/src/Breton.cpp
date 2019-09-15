/* Name: Larry Y.
 * Date: July 19, 2019
 * Desc: Breton society resembles a feudal realm, so here they can be peasants, traders, or in the mages guild. */

#include "Breton.h"
#include <iostream>
#include "Town.h"

using namespace std;

Breton::Breton(string myName) : Human(myName, "Breton", 15, 25, 3) // Bretons start with some more money
{
	//ctor
}

Breton::~Breton()
{
	//dtor
}
