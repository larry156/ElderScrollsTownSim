#ifndef MER_H
#define MER_H

#include <Citizen.h>


class Mer : protected Citizen
{
	public:
		Mer(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~Mer();

	protected:

	private:
};

#endif // MER_H
