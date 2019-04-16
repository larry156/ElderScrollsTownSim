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
};

#endif // ALTMER_H
