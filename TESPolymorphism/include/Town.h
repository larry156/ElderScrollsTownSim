#ifndef TOWN_H
#define TOWN_H
#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include <Citizen.h>

class Town
{
	public:
		Town(int townSize, bool doWelfare);
		virtual ~Town();
		std::vector<Citizen*> myCitizens;
		std::map<std::string, std::vector<std::string>> nameListsFirst, nameListsLast;
		Citizen* getRandomCitizen();
		int getTarget(Citizen* curCitizen);
		//std::vector<Citizen*> getResidents();
		bool readNames(std::string fileName, std::vector<std::string> &theList);
		std::string nameGen(std::vector<std::string> &nameList);
		void moveIn();
		//void moveOut(int indexPos);
		void clearDeadResidents();
		void initializeTown(); // For reasons, it's better to not have this in the constructor.
		void runSimulation();


	protected:

	private:
		int preferredSize;
		bool giveWelfare;
		HANDLE consHandle;
		const int C_GRAY = 8, C_TURQUOISE = 3, C_YELLOW = 14, C_DEFAULT = 15, C_ERROR = 244, C_WARNING = 12; // Colours for output text
		const int DAYS_IN_A_MONTH = 30;
		int curDay, curMonth, curYear; // It takes a really long time to actually get past a year, but whatever.
		const std::string MONTHS[12] = {"Morning Star", "Sun's Dawn", "First Seed", "Rain's Hand", "Second Seed", "Mid Year", "Sun's Height", "Last Seed", "Hearthfire", "Frost Fall", "Sun's Dusk", "Evening Star"};
};

#endif // TOWN_H
