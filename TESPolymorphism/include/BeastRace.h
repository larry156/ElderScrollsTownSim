#ifndef BEASTRACE_H
#define BEASTRACE_H

#include <Citizen.h>


class BeastRace : protected Citizen
{
	public:
		BeastRace(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~BeastRace();
		virtual void speak() = 0;
		virtual void upkeep(Citizen* target) = 0;

	protected:

	private:
};

#endif // BEASTRACE_H
