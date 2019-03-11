/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The Nords of Skyrim. They have some unique deities, unique dialogue, and may hunt an animal or brawl a citizen. */
 // As of February 22, 2019, Nords can now be adventurers as well.

#include "Nord.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

Nord::Nord(string myName) : Human(myName, "Nord", 10, 20, 3)
{
	// Remove deities from the pantheon. These are probably going to be replaced with something else.
	deities.erase(find(deities.begin(), deities.end(), "Kynareth"));

	// Add some Nordic deities
	deities.push_back("Shor");
	deities.push_back("Ysmir");
	deities.push_back("Kyne"); // Kyne is a different version of Kynareth.

	jobSkill = rand() % 51 + 25; // Nords have between 25 and 75 jobSkill.
	const int JOB_THRESHOLD = 50; // A jobSkill below this is considered "bad".
	int professionRoll = rand() % 2; // 0 for Hunter, 1 for Adventurer.
	if (professionRoll == 1)
	{
		profession = "Hunter";
		profCombatSkillBonus = rand() % 5 - 2; // Should be -2 to 2;

		dialogue.push_back("I used to be an adventurer like you. Then I took an arrow in the knee.");
	}
	else
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
	}

	// Nord-specific dialogue
	dialogue.push_back("Someone steal your sweetroll, %targetfirst%?");

	dialogue.push_back("%target% seems like a milk-drinker to me.");
	dialogue.push_back("Have you seen those warriors from Hammerfell? They have curved swords.");
	dialogue.push_back("%deity%'s blessings be upon you, %target%.");

	speakChance = rand() % 21 + 50; // Nords have a 50-70% chance of speaking.
	brawlChance = rand() % 101; // Nords have a 0-100% chance of brawling someone.
	combatSkill = rand() % 21 + 10 + profCombatSkillBonus; // Nords have a combatSkill between 10-30, plus whatever their profession gives them.

	cout << getName() << " is a Nord of Skyrim." << endl;
	//listStats();
}
Nord::~Nord()
{
	if (!getDead())
	{
		cout << getName() << " is still living in the town." << endl;
	}
	else
	{
		cout << getName() << " was entombed in the Hall of the Dead." << endl;
	}
}

// Each upkeep, Nords will speak, pay taxes, and perform an action.
void Nord::upkeep(Citizen* target)
{
	// Set curTarget to the current target and roll for actions
	curTarget = target;
	int speakRoll = rand() % 100;
	int brawlRoll = rand() % 100;
	int huntRoll = rand() % 2;

	// Now do upkeep
	payTaxes();
	if (speakRoll < speakChance)
	{
		speak();
	}
	if (brawlRoll < brawlChance && !curTarget->getDead())
	{
		brawl();
	}
	else if (huntRoll == 0 && profession == "Hunter")
	{
		hunt();
	}
	else if (profession == "Adventurer")
	{
		adventure();
	}

	if (!getDead())
	{
		cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
	}
}

// Nords are able to hunt animals for money. They have a 50% chance of doing this if they don't brawl someone.
void Nord::hunt()
{
	if (!getDead())
	{
		// Fails if combatRoll < 40, critically fails (i.e. dies) if < 20
		const int FAIL_THRESHOLD = 40, CRITICAL_FAIL = 20;
		const vector<string> ANIMALS = {"Deer", "Elk", "Troll", "Bear"}; // Vector is used here because it knows its size.
		int animalToHunt = rand() % ANIMALS.size();

		cout << getName() << " is hunting a " << ANIMALS[animalToHunt] << "." << endl;
		if (combatRoll() < CRITICAL_FAIL)
		{
			cout << "The " << ANIMALS[animalToHunt] << " has gravely wounded " << getName() << "!" << endl;
			kill();
		}
		else if (combatRoll() < FAIL_THRESHOLD)
		{
			int deityToInvoke = rand() % deities.size();
			cout << getName() << ": By " << deities[deityToInvoke] << ", I haven't seen a single " << ANIMALS[animalToHunt] << " today!" << endl;
		}
		else
		{
			cout << getName() << ": This " << ANIMALS[animalToHunt] << " pelt is bound to be worth some gold at market." << endl;
			int salePrice = rand() % 7 + 3; // Should be 3-9 gold.
			getPaid(salePrice);
		}
	}
}

// Nords might brawl people each upkeep. The challenged may or may not accept (Threshold is listed below, roll checks combatRoll - half their combatSkill).
// If they do, a combatRoll is made for both participants, and the person with the higher roll wins. Combat is non-lethal (since it's a fistfight), and if both
// combatRolls are within a certain range of each other, it's a draw. If there is a winner, the loser pays the winner 5 Gold.
void Nord::brawl()
{
	if (!getDead())
	{
		const int ACCEPT_THRESHOLD = 70, DRAW_RANGE = 10, WINNINGS = 5;

		// Challenge the target
		cout << getName() << ": " << curTarget->getName() << ", you milk-drinker! I challenge you to a brawl!" << endl;
		// They are more likely to accept with a higher combatSkill. Since combatRoll adds their combatSkill to the roll, it has to be subtracted once here.
		int acceptRoll = curTarget->combatRoll() - curTarget->getCombatSkill() - curTarget->getCombatSkill() / 2;
		if (acceptRoll < ACCEPT_THRESHOLD) // They accept
		{
			cout << curTarget->getName() << ": Alright, I accept. Do your worst!" << endl;
		}
		else // They refuse
		{
			cout << curTarget->getName() << ": I don't think so." << endl;
			cout << getName() << ": I knew it! You're a milk-drinker!" << endl;
			return;
		}

		// Since the target has accepted, begin the duel!
		int myRoll = combatRoll(), theirRoll = curTarget->combatRoll();
		cout << getName() << " and " << curTarget->getName() << " are brawling!" << endl;
		//cout << getName() << " rolled a " << myRoll << "! " << curTarget->getName() << " rolled a " << theirRoll << "!" << endl;
		// Check for draws first
		if (abs(myRoll - theirRoll) < DRAW_RANGE)
		{
			cout << getName() << ": Shor's bones, what a fight that was! You fought well, " << curTarget->shortName() << "." << endl;
			cout << curTarget->getName() << ": You fought well too. A shame it ended up being a draw." << endl;
		}
		else
		{
			// Determine the winner and loser.
			Citizen* winner = NULL;
			Citizen* loser = NULL;
			if (myRoll > theirRoll)
			{
				winner = this;
				loser = curTarget;
			}
			else
			{
				winner = curTarget;
				loser = this;
			}

			// Loser concedes and pays the winner.
			cout << loser->getName() << ": Alright alright I concede! No more! Take your winnings and leave, " << winner->shortName() << "!" << endl;
			loser->payPerson(WINNINGS, winner, true);
		}
	}
}

// Adventuring functionality copied over from the Redguard class.
void Nord::adventure()
{
	if (!getDead())
	{
		const vector<string> EXPLORABLES = {"an Ancient Crypt", "a Nordic Ruin", "a Dwemer Ruin", "a Ruined Fort"};
		int toExplore = rand() % EXPLORABLES.size();
		vector<tuple<string, int>> enemyList; // Key = Name of Enemy, Value = combatSkill of enemy. Enemies are only fought if the adventurer fails a jobSkill roll.
		// Some generic enemies that are found in all ruins
		enemyList.push_back(make_tuple("Bandit", 3));
		enemyList.push_back(make_tuple("Bandit Chief", 15));
		enemyList.push_back(make_tuple("Warlock", 10));
		enemyList.push_back(make_tuple("Necromancer", 15));
		// Ruin-specific enemies
		if (EXPLORABLES[toExplore] == "an Ancient Crypt") // Ancient Crypts can have various pests and undead within.
		{
			enemyList.push_back(make_tuple("Spider", 5));
			enemyList.push_back(make_tuple("Vampire", 15));
			enemyList.push_back(make_tuple("Vampire's Thrall", 7));
			enemyList.push_back(make_tuple("Skeever", -2)); // These are essentially large rats.
			enemyList.push_back(make_tuple("Draugr", 0));
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
			const int DEATH_RANGE = 35; // If theirRoll - myRoll is greater than this number, the adventurer dies.
			if (theirRoll - myRoll > DEATH_RANGE)
			{
				cout << "The " << enemyName << " has mortally wounded " << getName() << "!" << endl;
				kill();
				return;
			}
			else if (theirRoll > myRoll)
			{
				cout << getName() << ": Shor's bones, I'm going to need to see a healer after this..." << endl;
				return;
			}
			else
			{
				cout << getName() << ": Now that was a good fight! Let's see what that " << enemyName << " was hiding..." << endl;
				int bonusGold = rand() % 6; // Some extra gold for defeating an enemy successfully.
				treasure += bonusGold;
			}
		}
		else
		{
			cout << getName() << ": Hmm, this place is empty. Maybe there's still some treasure left..." << endl;
		}
		cout << getName() << " has found treasure worth " << treasure << " Gold within the ruins." << endl;
		getPaid(treasure, false);
	}
}
