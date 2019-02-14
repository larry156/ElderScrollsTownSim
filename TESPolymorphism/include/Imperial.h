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
		int getProfession(); // TODO: MOVE PROFESSIONS TO CITIZEN, TURN IT INTO A STRING.

	protected:

	private:
		const int PROF_BARD = 0, PROF_ASSASSIN = 1, PROF_MERCHANT = 2;
		int profession, attemptsOnTarget;
		void bard();
		void assassinate();
		void trade();
};

#endif // IMPERIAL_H
