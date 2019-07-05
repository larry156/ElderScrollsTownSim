#ifndef ALTMER_H
#define ALTMER_H

#include <Mer.h>
#include <string>

class Altmer : public Mer
{
	public:
		Altmer(std::string myName, bool isHoRMember = false);
		virtual ~Altmer();
		void upkeep(Citizen* target);

	protected:

	private:
		// These professions don't really properly represent *all* Altmer. More professions might be added for all races later to add some variety.
		void perform();
		void bard();
		void mage();
		int researchQuality, researchTopic;
		// Members of the House of Reveries leave the town after a certain number of days to go somewhere else.
		// Members of the Mages Guild leave after having less than 10 gold for 10 days in a row.
		const int MAGE_LEAVE_GOLD_THRESHOLD = 10;
		int daysInTown, daysUntilLeave;
};

#endif // ALTMER_H
