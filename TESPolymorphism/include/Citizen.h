#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>
#include <string>

class Citizen
{
	public:
		Citizen(std::string myName, std::string mySpecies, int startMoneyMin, int startMoneyMax, int speciesTaxAmount);
		virtual ~Citizen();
		std::string getName();
		std::string shortName();
		std::string getSpecies();
		std::string getProfession();
		int checkWealth();
		bool getDead();
		int getCombatSkill();
		int combatRoll();
		void payPerson(int toPay, Citizen* recipient, bool requireSufficientMoney = false, bool isTheft = false);
		void payTaxes();
		void getPaid(int toPay, bool showMessage = true);
		virtual void speak();
		virtual void upkeep(Citizen* target) = 0;
		virtual void kill();
		void replaceString(std::string &theString, std::string replaceThis, std::string replaceWith); // Used to replace things like %target% with actual words/names.
		void listStats(); // Used for debug purposes.

	protected:
		bool isDead;
		std::vector<std::string> dialogue;
		std::vector<std::string> deities;
		std::string profession;
		Citizen* curTarget;
		int speakChance;
		int combatSkill;
		int profCombatSkillBonus; // Some professions get a bonus or malus to their combat skill.
		int jobSkill;

	private:
		std::string name;
		std::string species;
		int money;
		int taxAmount;

};

#endif // CITIZEN_H
