/* Name: Larry Y.
 * Date: March 17, 2019
 * Desc: The High Elves of Summerset. They have some unique dialogue, and can be members of either the House of Reveries (a troupe of performers) or the Mages Guild. */

#include "Altmer.h"
#include <iostream>

using namespace std;

// Professions will be determined outside of the class, when a new citizen moves in, since members of the House of Reveries have special names.
Altmer::Altmer(string myName, bool isHoRMember) : Mer(myName, "Altmer", 10, 20, 3)
{
	if (isHoRMember)
	{
		int specialization = 1;//rand() % 2; // Members can be either regular performers or bards
		profCombatSkillBonus = rand() % 6 + 2; // Range between 2 to 7.
		jobSkill = rand() % 51 + 50; // The House of Reveries doesn't just accept any random stranger with a lute... Range between 50-100.

		if (specialization == 0)
		{
			profession = "Bard";
		}
		else
		{
			profession = "Performer";
		}
		dialogue.push_back("I will be hosting a performance soon. You should come, %targetfirst%!");
		dialogue.push_back("This town doesn't have anything as grand as the Manor of Masques back in Rellenthil, but it does have a certain charm to it.");
	}
	else
	{
		profession = "Mage";
		profCombatSkillBonus = rand() % 11 + 2; // Bonus ranging from 2 to 12.
		jobSkill = rand() % 61 + 30; // For mages, jobSkill determines how likely things are to go wrong when practicing magic.

		dialogue.push_back("");
	}
	//dialogue.push_back("");

	speakChance = rand() % 21 + 50; // Altmer have a 50-70% chance of speaking.
	combatSkill = rand() % 31 - 5 + profCombatSkillBonus; // Altmer have a combatSkill between -5 to 25, plus whatever their profession gives them.

	cout << getName() << " is an Altmer from the Summerset Isles." << endl;
}

Altmer::~Altmer()
{
	if (!getDead())
	{
		cout << getName() << " is still living in the town." << endl;
	}
	else
	{
		cout << getName() << "'s remains were cremated." << endl;
	}
}

// Each upkeep, High Elves may speak, will pay taxes, and may perform an action based on their profession.
void Altmer::upkeep(Citizen* target)
{
	// Set curTarget and roll for actions
	curTarget = target;
	int speakRoll = rand() % 100;
	int actionRoll = rand() % 100;
	// actionRoll needs to be below these thresholds for the Altmer to actually do something.
	// If this Altmer has less than MIN_GOLD left, they will always do their job.
	const int BARD_CHANCE = 75, PERFORM_CHANCE = 65, MAGE_CHANCE = 70, MIN_GOLD = 5;

	// Now do upkeep
	payTaxes();
	if (speakRoll < speakChance)
	{
		speak();
	}
	if (profession == "Bard" && (actionRoll < BARD_CHANCE || checkWealth() < MIN_GOLD))
	{
		bard();
	}
	else if (profession == "Performer" && (actionRoll < PERFORM_CHANCE || checkWealth() < MIN_GOLD))
	{
		perform();
	}
	else if (profession == "Mage" && (actionRoll < MAGE_CHANCE || checkWealth() < MIN_GOLD))
	{
		//mage();
	}

	if (!getDead())
	{
		cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
	}
}

// Bard function copied from the Imperial class, though some different songs are used.
void Altmer::bard()
{
	if (!getDead())
	{
		const vector<string> SONG_LIST = {"Over the Seas", "Charge of the Welkynar", "Folly of Man", "Praise Be", "The Star-Eyed Bride of Alinor", "The Tale of Syrabane", "The Beauty of Dawn"};
		int songRoll = rand() % SONG_LIST.size();
		int bardRoll = rand() % 100;

		cout << getName() << " sings a rendition of \"" << SONG_LIST[songRoll] << "\" for the townsfolk." << endl;
		if (bardRoll < jobSkill)
		{
			// If a bard succeeds in getting tipped, they will always receive gold equal to jobSkill / 10, plus up to 5 additional gold.
			int additionalGold = rand() % 6;
			int tips = jobSkill / 10 + additionalGold;
			cout << "Audience: *Cheers and applause*" << endl;
			cout << getName() << ": Thank you everyone, it's truly been an honour." << endl;
			cout << getName() << " has received " << tips << " Gold in for their performance." << endl;
			getPaid(tips, false);
		}
		else
		{
			int pay = jobSkill / 10; // Unlike regular bards, the troupe will always pay the performer some wages.
			cout << "Audience: *Polite applause*" << endl;
			cout << getName() << " has received " << pay << " Gold in for their performance." << endl;
			getPaid(pay, false);
		}
	}
}

// Performers will "do" various things (e.g. juggling knives, acrobatics, theatrical performances).
// If curTarget is another performer, the two will collaborate, increasing the chance of success.
void Altmer::perform()
{
	const vector<string> ACTIONS = {" juggles knives", " dazzles the audience with their fire-eating skills", " swallows a sword", " does an interpretive dance", " recounts a captivating tale", " plays a tune on their fiddle"};
	const vector<string> COLLAB_ACTIONS = {" perform an acrobatic routine", " put on a play", " do a comedy skit", " play a duet on their instruments"};

	// As mentioned in the Bard function, performers will always receive at some Gold from the troupe.
	int tipChance = jobSkill; // Chance of getting tips from the audience.
	int revenue = 7;

	//bool collaborate = false;
	if (curTarget->getProfession() == "Performer")
	{
		//collaborate = true;
		cout << getName() << " is staging a performance with " << curTarget->getName() << "." << endl;
		int collabActionType = rand() % 2; // A collaboration may consist of either two single "ACTIONS", or one "COLLAB_ACTION".
		tipChance = (jobSkill + curTarget->getJobSkill()) / 2 + 20; // Average of the two performers' jobSkills, plus a bonus of 20.
		if (collabActionType == 0) // Two single actions
		{
			int actionOne = rand() % ACTIONS.size();
			int actionTwo = actionOne;
			while (actionOne == actionTwo)
			{
				actionTwo = rand() % ACTIONS.size();
			}

			int tipRoll = rand() % 100;
			if (tipRoll < tipChance)
			{
				revenue += rand() % 6; // Up to 5 additional gold will be earned in tips.
				cout << getName() << " skillfully" << ACTIONS[actionOne] << " while " << curTarget->getName() << ACTIONS[actionTwo] << "." << endl;
				cout << "The audience responds with cheers and roaring applause." << endl;
			}
			else
			{
				cout << getName() << ACTIONS[actionOne] << " while " << curTarget->getName() << ACTIONS[actionTwo] << "." << endl;
				cout << "The audience applauds the duo." << endl;
			}
		}
		else // One COLLAB_ACTION
		{
			int actionRoll = rand() % COLLAB_ACTIONS.size();
			int tipRoll = rand() % 100;
			if (tipRoll < tipChance)
			{
				revenue += rand() % 6; // Up to 5 additional gold will be earned in tips.
				cout << getName() << " and " << curTarget->getName() << COLLAB_ACTIONS[actionRoll] << ", earning cheers and vigorous applause from the audience." << endl;
			}
			else
			{
				cout << getName() << " and " << curTarget->getName() << COLLAB_ACTIONS[actionRoll] << ". The audience applauds." << endl;
			}
		}
		cout << getName() << " and " << curTarget->getName() << " have earned " << revenue << " Gold for their performance." << endl;
		getPaid(revenue, false);
		curTarget->getPaid(revenue, false);
	}
	else
	{
		cout << getName() << " is staging a solo performance." << endl;
		int actionRoll = rand() % ACTIONS.size();
		int tipRoll = rand() % 100;
		if (tipRoll < tipChance)
		{
			revenue += rand() % 6; // Up to 5 additional gold will be earned in tips.
			cout << getName() << " skillfully" << ACTIONS[actionRoll] << ", earning cheers and vigorous applause from the audience." << endl;
		}
		else
		{
			cout << getName() << ACTIONS[actionRoll] << ". The audience applauds." << endl;
		}
		cout << getName() << " has earned " << revenue << " Gold for their performance." << endl;
		getPaid(revenue, false);
	}
}
