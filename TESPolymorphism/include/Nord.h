#ifndef NORD_H
#define NORD_H

#include <Human.h>
#include <string>

class Nord : public Human
{
	public:
		Nord(std::string myName);
		virtual ~Nord();
		//void speak();
		void upkeep(Citizen* target);

	protected:

	private:
		void hunt();
		void brawl();
		int brawlChance;
};

#endif // NORD_H
