#ifndef BEASTRACE_H
#define BEASTRACE_H

#include <Citizen.h>


class BeastRace : protected Citizen
{
	public:
		BeastRace(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~BeastRace();

	protected:

	private:
};

#endif // BEASTRACE_H
