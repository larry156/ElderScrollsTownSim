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
		std::string getSpecies();
		int checkWealth();
		bool getDead();
		//Citizen* getTarget();
		//void setTarget(Citizen* target);
		void payPerson(int toPay, Citizen* recipient, bool requireSufficientMoney, bool isTheft);
		void payTaxes();
		void getPaid(int toPay, bool showMessage);
		virtual void speak() = 0;
		virtual void upkeep(Citizen* target) = 0;
		virtual void kill();
		bool replaceString(std::string &theString, std::string replaceThis, std::string replaceWith); // Used to replace things like %target% with actual words/names.

	protected:
		bool isDead;
		std::vector<std::string> dialogue;
		std::vector<std::string> deities;
		Citizen* curTarget;

	private:
		std::string name;
		std::string species;
		int money;
		int taxAmount;

};

#endif // CITIZEN_H
