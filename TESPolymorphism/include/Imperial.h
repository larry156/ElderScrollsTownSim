#ifndef IMPERIAL_H
#define IMPERIAL_H

#include <Human.h>
#include <string>

class Imperial : public Human
{
	public:
		Imperial(std::string myName);
		virtual ~Imperial();
		void upkeep(Citizen* target);

	protected:

	private:
		int attemptsOnTarget;
		Citizen* assassinationTarget;
		void bard();
		void assassinate();
		void trade();
};

#endif // IMPERIAL_H
