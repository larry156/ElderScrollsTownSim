/* Name: Larry Y.
 * Date: February 3, 2019
 * Desc: The Imperials of Cyrodiil. They have some unique dialogue, and can be either bards, assassins, or merchants. */

#include "Imperial.h"
#include <iostream>

using namespace std;

Imperial::Imperial(string myName) : Human(myName, "Imperial", 15, 25, 3) // Imperials start with some more money
{
	// The 8 Divines have already been set up in the Human constructor
	// Determine profession
	profession = rand() % 2; // 0 for Bard, 1 for Assassin, 2 for Merchant (Not yet implemented).

	// Add some dialogue for Imperials. First add profession-related dialogue, then shared dialogue. Assassins also get two deities.
	if (profession == PROF_BARD)
	{
		dialogue.push_back("Care for a song, %targetfirst%?");
		dialogue.push_back("I think my lute might be broken.");
	}
	else if (profession == PROF_ASSASSIN)
	{
		deities.push_back("Sithis");
		deities.push_back("The Night Mother");

		dialogue.push_back("*Remains silent*");
		dialogue.push_back("I wonder if I have a new contract today...");

		attemptsOnTarget = -1; // A -1 means a new assassination target should be assigned.
	}
	else if (profession == PROF_MERCHANT)
	{

	}
	dialogue.push_back("Have you ever been to the Imperial City, %targetfirst%?");
	dialogue.push_back("Apparently some \"Pirate Queen\" invaded the city of Anvil recently. I hope the people living there are okay.");

	speakChance = rand() % 21 + 50; // Imperials have a 50-70% chance of speaking.
	combatSkill = rand() % 21; // Imperials have a combatSkill between 0-20.
	jobSkill = rand() % 61 + 20; // Imperials have a jobSkill between 20 and 80. It works pretty much as you'd expect for bards and merchants.
	// For assassins, jobSkill determines how stealthy they are, while combat is still handled using combatSkill.

	cout << getName() << " is an Imperial from Cyrodiil." << endl;
}

Imperial::~Imperial()
{
	if (!getDead())
	{
		cout << getName() << " is still living in the town." << endl;
	}
	else if (profession != PROF_ASSASSIN)
	{
		cout << getName() << " was buried in the town cemetery." << endl;
	}
	else
	{
		cout << getName() << "'s soul has entered the Void." << endl;
	}
}

// Return an int representing this Imperial's profession.
int Imperial::getProfession()
{
	return profession;
}

// Each upkeep, Imperials may speak, will pay taxes, and may perform an action based on their profession.
void Imperial::upkeep(Citizen* target)
{
	// Set curTarget to the current target if this Imperial isn't an assassin or if they need a new target.
	if (profession != PROF_ASSASSIN || attemptsOnTarget == -1)
	{
		curTarget = target;

		if (profession == PROF_ASSASSIN)
		{
			attemptsOnTarget = 0;
		}

		if (curTarget->getSpecies() == "Imperial") // This is almost definitely going to be replaced later.
		{
			Imperial* profTest = static_cast<Imperial*>(curTarget);
			if (profTest->getProfession() == PROF_ASSASSIN)
			{
				attemptsOnTarget = -1; // Assassins shouldn't get contracts for other assassins.
			}
		}
	}

	// Roll for actions
	int speakRoll = rand() % 100;
	int actionRoll = rand() % 100;
	// actionRoll needs to be below these thresholds for the Imperial to actually do something.
	// If this Imperial has less than MIN_GOLD left, they will always do their job.
	const int BARD_CHANCE = 60, ASSASSIN_CHANCE = 90, TRADE_CHANCE = 60, MIN_GOLD = 5;

	// Now do upkeep
	payTaxes();
	if (speakRoll < speakChance)
	{
		speak();
	}
	if (profession == PROF_BARD && (actionRoll < BARD_CHANCE || checkWealth() < MIN_GOLD))
	{
		bard();
	}
	else if (profession == PROF_ASSASSIN && (actionRoll < ASSASSIN_CHANCE || checkWealth() < MIN_GOLD) && attemptsOnTarget != -1)
	{
		assassinate();
	}
	else if (profession == PROF_MERCHANT && (actionRoll < TRADE_CHANCE || checkWealth() < MIN_GOLD))
	{
		//trade();
	}

	if (!getDead())
	{
		cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
	}
}

// A bard will "sing" (I am not going to post an entire song here, that would be ridiculous) a song chosen from a list, and may receive some tips from the audience.
void Imperial::bard()
{
	if (!getDead())
	{
		const vector<string> SONG_LIST = {"The Dragonborn Comes", "Ragnar the Red", "Chim-El Adabal", "The Plane Meld", "The Tale of Syrabane", "Ithguleoir"};
		int songRoll = rand() % SONG_LIST.size();
		int bardRoll = rand() % 100;

		cout << getName() << " serenades the townsfolk with a rendition of \"" << SONG_LIST[songRoll] << "\"." << endl;
		if (bardRoll < jobSkill)
		{
			// If a bard succeeds in getting tipped, they will always receive gold equal to jobSkill / 10, plus up to 3 additional gold.
			int additionalGold = rand() % 4;
			int tips = jobSkill / 10 + additionalGold;
			cout << "Audience: *Cheers and applause*" << endl;
			cout << getName() << " has received " << tips << " Gold in tips from the audience." << endl;
			getPaid(tips, false);
		}
		else
		{
			cout << "Audience: *Polite applause*" << endl;
		}
	}
}

// Assassins try to kill their target. Success is based on a simple combatRoll(), with modifiers added/subtracted based on stealth.
// Whether or not the assassin successfully surprises their target depends on the assassin's jobSkill.
// If the attempt is successful, they complete their contract, get paid, and a new target may be received on the next upkeep.
// If it fails, the assassin might die (chance decreases if the target was surprised). They might also give up on the contract and choose a new target.
void Imperial::assassinate()
{
	if (!getDead() && !curTarget->getDead())
	{
		// Death thresholds and how much gold a successful contract will pay.
		const int CRIT_FAIL_NOSTEALTH = 30, CRIT_FAIL_STEALTH = 40, CONTRACT_WORTH = rand() % 11 + 5; // 5-15 gold

		// Roll needs to be below this number for a successful surprise attack
		const int STEALTH_THRESHOLD = jobSkill - (curTarget->getCombatSkill() / 2); // The higher the combatSkill, the more perceptive the target is?
		int stealthRoll = rand() % 100;
		bool targetSurprised = false;
		if (stealthRoll < STEALTH_THRESHOLD)
		{
			targetSurprised = true;
			cout << getName() << " sneaks up behind " << curTarget->getName() << "..." << endl;
		}
		else
		{
			cout << getName() << " tries to sneak up to " << curTarget->getName() << " but is spotted." << endl;
		}
		// Surprising the target gives a bonus, not doing so confers a penalty.
		int combatModifier = 0;
		if (targetSurprised)
		{
			combatModifier = 15;
		}
		else
		{
			combatModifier = -10;
		}

		// Assassinate
		int myRoll = combatRoll() + combatModifier, theirRoll = curTarget->combatRoll();
		// Success results in death of the target
		if (myRoll > theirRoll)
		{
			cout << getName() << ": Hail Sithis!" << endl;
			curTarget->kill();
			cout << getName() << ": I should return to the sanctuary and turn in my contract." << endl;
			getPaid(CONTRACT_WORTH);
			attemptsOnTarget = -1;
		}
		// The assassin dies if the difference between theirRoll and myRoll is greater than the death thresholds.
		else if (theirRoll - myRoll > CRIT_FAIL_STEALTH || (theirRoll - myRoll > CRIT_FAIL_NOSTEALTH && !targetSurprised))
		{
			cout << curTarget->getName() << ": Hah! This is what the Dark Brotherhood sends? Pathetic." << endl;
			kill();
		}
		// Just a regular failure where the assassin gets away.
		else
		{
			cout << curTarget->getName() << ": Blasted assassin, just die already!" << endl;


			int giveUpChance = attemptsOnTarget * 10; // Each failed attempt makes the assassin more likely to give up.
			int giveUpRoll = rand() % 100;
			if (giveUpRoll < giveUpChance)
			{
				cout << getName() << ": Gah, I've had enough of this! *turns invisible and runs off*" << endl;
				attemptsOnTarget = -1;
			}
			else
			{
				cout << getName() << ": Not today... *turns invisible and runs off*" << endl;
				attemptsOnTarget++;
			}
		}
	}
}
