#ifndef HUMAN_H
#define HUMAN_H

#include <Citizen.h>
#include <string>

class Human : public Citizen
{
	public:
		Human(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~Human();

	protected:

	private:
};

#endif // HUMAN_H
