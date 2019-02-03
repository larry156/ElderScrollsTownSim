#ifndef MER_H
#define MER_H

#include <Citizen.h>


class Mer : protected Citizen
{
	public:
		Mer(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~Mer();
		virtual void speak() = 0;
		virtual void upkeep(Citizen* target) = 0;

	protected:

	private:
};

#endif // MER_H
