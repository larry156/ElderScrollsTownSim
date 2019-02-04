/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: Polymorphism using Elder Scrolls races. */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

#include "Citizen.h"
#include "Human.h"
#include "Mer.h"
#include "BeastRace.h"
#include "Nord.h"

using namespace std;

int main()
{
	srand(time(NULL));

	Nord* Jorunn = new Nord("Jorunn Flame-Hair");
	Nord* Mera = new Nord("Mera Stormcloak");
	cout << endl;

	Jorunn->upkeep(Mera);
	cout << endl;
	Mera->upkeep(Jorunn);

	cout << endl;
    delete Jorunn;
    delete Mera;

    return 0;
}
