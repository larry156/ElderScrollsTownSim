/* Name: Larry Y.
 * Date: February 18, 2019
 * Desc: The Redguards of Hammerfell. They have some unique dialogue, deities, and can be either adventurers or mercenaries. */

#include "Redguard.h"
#include <iostream>
#include <cstdlib>
#include <tuple>

using namespace std;

Redguard::Redguard(string myName) : Human(myName, "Redguard", 10, 20, 3)
{
	// According to the uesp.net page for Redguards, they have their own pantheon of gods, but sometimes use the regular human names when referring to them.
	// Therefore, no deities should need to be removed.
	deities.push_back("Diagna");
	deities.push_back("HoonDing");
	deities.push_back("Leki");
	deities.push_back("Morwha");
	deities.push_back("Ruptga");
	deities.push_back("Satakal");
	deities.push_back("Sep");
	deities.push_back("Tava");
	deities.push_back("Tu'whacca");
	deities.push_back("Zeht");

	jobSkill = rand() % 51 + 30; // 30-80 jobSkill.
	const int JOB_THRESHOLD = 50; // A jobSkill below this is considered "bad".
	int professionRoll = rand() % 2; // 0 for Adventurer, 1 for Mercenary.
	if (professionRoll == 0)
	{
		profession = "Adventurer";
		if (jobSkill < JOB_THRESHOLD) // If an adventurer is bad at their job, they're probably not great at fighting...
		{
			profCombatSkillBonus = rand() % 11 * -1;
		}
		else
		{
			profCombatSkillBonus = rand() % 11 + 5; // Otherwise, they get a bonus to combatSkill ranging from 5 to 15 points.
		}

		dialogue.push_back("Be careful on the roads, %targetfirst%. There's been reports of bandits recently.");
		dialogue.push_back("These ancient ruins are surprisingly well-maintained.");
	}
	else
	{
		profession = "Mercenary";
		if (jobSkill < JOB_THRESHOLD) // If an mercenary is bad at their job, they're probably not great at fighting...
		{
			profCombatSkillBonus = rand() % 6 * -1;
		}
		else
		{
			profCombatSkillBonus = rand() % 6 + 2; // Bonus of 2-7 points.
		}

		dialogue.push_back("Let's see if I have a new contract today...");
		dialogue.push_back("You know, %targetfirst%, you should consider joining the Fighter's Guild.");
	}

	// Add some dialogue
	dialogue.push_back("You know, my ancestor was one of the famous Sword-Singers of Yokuda.");
	dialogue.push_back("Have you ever been to Hew's Bane, %targetfirst%? It's a lovely place.");
	dialogue.push_back("Blasted necromancers and their foul magic...%deity% curse them all!");

	speakChance = rand() % 21 + 50; // Redguards have a 50-70% chance of speaking.
	combatSkill = rand() % 16 + 15 + profCombatSkillBonus; // Redguards have a combatSkill between 15-30, plus whatever bonus their profession gives them.

	cout << getName() << " is a Redguard from Hammerfell." << endl;
	//listStats();
}

Redguard::~Redguard()
{
	if (!getDead())
	{
		cout << getName() << " is still living in the town." << endl;
	}
	else
	{
		cout << getName() << " was consecrated and entombed underground." << endl;
	}
}

// Each upkeep, Redguards may speak, will pay taxes, and may perform an action based on their profession.
void Redguard::upkeep(Citizen* target)
{
	curTarget = target;

	// Roll for actions
	int speakRoll = rand() % 100;
	int actionRoll = rand() % 100;
	// actionRoll needs to be below these thresholds for the Redguard to actually do something.
	// If this Redguard has less than MIN_GOLD left, they will always do their job. (Adventurers will always do something so they don't have a threshold)
	const int MERCENARY_CHANCE = 60, MIN_GOLD = 5;

	// Now do upkeep
	payTaxes();
	if (speakRoll < speakChance)
	{
		speak();
	}
	if (profession == "Adventurer")
	{
		adventure();
	}
	else if (profession == "Mercenary" && (actionRoll < MERCENARY_CHANCE || checkWealth() < MIN_GOLD))
	{
		mercenary();
	}

	if (!getDead())
	{
		cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
	}
}

// Adventurers will explore ruins and look for treasure. They may however be attacked by stuff inside.
// This function is almost definitely going to be used for other species as well, since a lot of work went into it and it would be kind of a waste not to.
void Redguard::adventure()
{
	if (!getDead())
	{
		const vector<string> EXPLORABLES = {"an Ayleid Ruin", "a Nordic Ruin", "a Dwemer Ruin", "a Ruined Fort"};
		int toExplore = rand() % EXPLORABLES.size();
		vector<tuple<string, int>> enemyList; // Key = Name of Enemy, Value = combatSkill of enemy. Enemies are only fought if the adventurer fails a jobSkill roll.
		// Some generic enemies that are found in all ruins
		enemyList.push_back(make_tuple("Bandit", 3));
		enemyList.push_back(make_tuple("Bandit Chief", 15));
		enemyList.push_back(make_tuple("Warlock", 10));
		enemyList.push_back(make_tuple("Necromancer", 15));
		// Ruin-specific enemies
		if (EXPLORABLES[toExplore] == "an Ayleid Ruin") // Ayleid ruins can have spiders, various daedra, as well as ghosts and other undead.
		{
			enemyList.push_back(make_tuple("Spider", 5));
			enemyList.push_back(make_tuple("Auroran", 15));
			enemyList.push_back(make_tuple("Winged Twilight", 7));
			enemyList.push_back(make_tuple("Dremora", 15));
			enemyList.push_back(make_tuple("Mazken", 15));
			enemyList.push_back(make_tuple("Ghost", 10));
			enemyList.push_back(make_tuple("Skeleton", -5)); // Skeletons are very fragile.
		}
		else if (EXPLORABLES[toExplore] == "a Nordic Ruin") // Nordic ruins can have spiders, draugr, and skeletons.
		{
			enemyList.push_back(make_tuple("Spider", 5)); // It might seem kind of weird that giant spiders are stronger than draugr.
			enemyList.push_back(make_tuple("Draugr", 0)); // This is because spiders might be venomous, while draugr are usually kind of weak.
			enemyList.push_back(make_tuple("Skeleton", -5));
		}
		else if (EXPLORABLES[toExplore] == "a Dwemer Ruin") // Dwarven ruins can have various automotons inside, as well as falmer and chaurus.
		{
			enemyList.push_back(make_tuple("Dwarven Spider", 7)); // Not actually a real spider.
			enemyList.push_back(make_tuple("Dwarven Sphere", 15)); // Is sometimes a sphere.
			enemyList.push_back(make_tuple("Dwarven Centurion", 40)); // These are very big.
			enemyList.push_back(make_tuple("Falmer", 5));
			enemyList.push_back(make_tuple("Chaurus", 10));
		}
		else if (EXPLORABLES[toExplore] == "a Ruined Fort")
		{
			enemyList.push_back(make_tuple("Ghost", 10));
			enemyList.push_back(make_tuple("Spriggan", 7));
			enemyList.push_back(make_tuple("Skeleton", -5));
		}

		cout << getName() << " is exploring " << EXPLORABLES[toExplore] << "." << endl;
		int doCombatRoll = rand() % 100, jobRoll = rand() % 100;
		int treasure = rand() % 11 + 3 + ((jobSkill - jobRoll) / 10); // How much treasure the adventurer finds is dependant on their skill.
		// Treasure should always be worth at least 1 gold.
		if (treasure < 1)
		{
			treasure = 1;
		}
		if (doCombatRoll > jobSkill) // If the roll fails, there's a monster lurking inside.
		{
			int enemyRoll = rand() % enemyList.size();
			string enemyName = get<0>(enemyList[enemyRoll]);
			int enemyCombatSkill = get<1>(enemyList[enemyRoll]);
			cout << getName() << " has encountered a " << enemyName << " within the ruins!" << endl;

			int myRoll = combatRoll(), theirRoll = rand() % 101 + enemyCombatSkill;
			const int DEATH_RANGE = 30; // If theirRoll - myRoll is greater than this number, the adventurer dies.
			if (theirRoll - myRoll > DEATH_RANGE)
			{
				cout << "The " << enemyName << " has mortally wounded " << getName() << "!" << endl;
				kill();
				return;
			}
			else if (theirRoll > myRoll)
			{
				cout << getName() << ": Blast, that was too close! I should get out of here before more show up..." << endl;
				return;
			}
			else
			{
				cout << getName() << ": Ugh, finally. Now let's see what that " << enemyName << " was hiding..." << endl;
				int bonusGold = rand() % 6; // Some extra gold for defeating an enemy successfully.
				treasure += bonusGold;
			}
		}
		else
		{
			cout << getName() << ": Hmm, there's nobody in here. Curious." << endl;
		}
		cout << getName() << " has found treasure worth " << treasure << " Gold within the ruins." << endl;
		getPaid(treasure, false);
	}
}

// Mercenaries get hired to go fight monsters, bandits, etc. Similar to adventuring, but more combat-based.
void Redguard::mercenary()
{
	// Create enemies using the same method as above.
	// Enemies are generally tougher than the ones faced by adventurers, but mercenaries will always be paid 12 gold for a successful contract.
	vector<tuple<string, int>> enemyList;
	enemyList.push_back(make_tuple("Bandit Chief", 15));
	enemyList.push_back(make_tuple("Daedroth", 25));
	enemyList.push_back(make_tuple("Frost Troll", 25));
	enemyList.push_back(make_tuple("Giant", 30));

	// Pick an enemy to fight
	int enemyRoll = rand() % enemyList.size();
	string enemyName = get<0>(enemyList[enemyRoll]);
	int enemyCombatSkill = get<1>(enemyList[enemyRoll]);
	cout << getName() << ": Looks like I have a contract to take out a " << enemyName << " today." << endl;

	// Fight it
	int myRoll = combatRoll(), theirRoll = rand() % 101 + enemyCombatSkill;
	// A mercenary with a jobSkill of 50 will die if theirRoll - myRoll > 30.
	// With a jobSkill of 55, they die if theirRoll - myRoll > 31.
	// With a jobSkill of 45, they die if theirRoll - myRoll > 29.
	const int DEATH_RANGE = 30 + (jobSkill - 50) / 5;
	if (theirRoll - myRoll > DEATH_RANGE)
	{
		cout << "The " << enemyName << " has mortally wounded " << getName() << "!" << endl;
		kill();
		return;
	}
	else if (theirRoll > myRoll)
	{
		cout << getName() << ": Blast it! Looks like I'll be limping back to the Guild hall." << endl;
		return;
	}
	else if (myRoll - theirRoll > DEATH_RANGE)
	{
		cout << getName() << ": A flawless victory. I should return to the Fighter's Guild." << endl;
	}
	else
	{
		cout << getName() << ": Ugh, that " << enemyName << " hits hard. Glad that's over with." << endl;
	}

	// Get paid, if the contract was not a failure.
	cout << getName() << " has received 12 Gold for successfully completing a Fighter's Guild contract." << endl;
	getPaid(12, false);
}
