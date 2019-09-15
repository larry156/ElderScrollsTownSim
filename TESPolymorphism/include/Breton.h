#ifndef BRETON_H
#define BRETON_H

#include <Human.h>
#include <string>

class Breton : public Human
{
	public:
		Breton(std::string myName);
		virtual ~Breton();
		void upkeep(Citizen* target);

	protected:

	private:
		void peasant();
		void trade();
		void mage();
};

#endif // BRETON_H
