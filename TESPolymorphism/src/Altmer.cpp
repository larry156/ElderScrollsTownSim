/* Name: Larry Y.
 * Date: March 17, 2019
 * Desc: The High Elves of Summerset. They have some unique dialogue, and can be members of either the House of Reveries (a troupe of performers) or the Mages Guild. */

#include "Altmer.h"
#include <iostream>
#include "Town.h"

using namespace std;

// Professions will be determined outside of the class, when a new citizen moves in, since members of the House of Reveries have special names.
Altmer::Altmer(string myName, bool isHoRMember) : Mer(myName, "Altmer", 10, 20, 3)
{
	if (isHoRMember)
	{
		int specialization = 1;//rand() % 2; // Members can be either regular performers or bards
		profCombatSkillBonus = rand() % 6 + 2; // Range between 2 to 7.
		jobSkill = rand() % 51 + 50; // The House of Reveries doesn't just accept any random stranger with a lute... Range between 50-100.
		daysUntilLeave = rand() % 365 + 30; // Will stay for at least 1 month, and at most 13 months.
		daysInTown = 0;
		blackSacramentChance = 0;

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
		researchQuality = 0;
		researchTopic = -1; // researchTopic of -1 indicates that they are not currently working on something.
		profCombatSkillBonus = rand() % 11 + 2; // Bonus ranging from 2 to 12.
		jobSkill = rand() % 61 + 30; // For mages, jobSkill determines how likely things are to go wrong when practicing magic.
		blackSacramentChance = 3;
		daysUntilLeave = 10;
		daysInTown = 0;

		dialogue.push_back("Sorry, %targetfirst%, I'm busy with research right now and can't talk.");
		dialogue.push_back("I could have sworn I left those potions around here somewhere...");
		dialogue.push_back("Blast it, the scamps have gotten loose again...");
		dialogue.push_back("I caught someone snooping around the other day. They were about to incinerate themselves with a dangerous artifact.");
		dialogue.push_back("I should have stayed in Eyevea...");
	}
	dialogue.push_back("This town is too cold. If only I were back in Summerset...");
	dialogue.push_back("Apparently something happened at the Crystal Tower recently. I wonder what?");

	speakChance = rand() % 21 + 50; // Altmer have a 50-70% chance of speaking.
	combatSkill = rand() % 31 - 5 + profCombatSkillBonus; // Altmer have a combatSkill between -5 to 25, plus whatever their profession gives them.

	cout << getName() << " is an Altmer from the Summerset Isles." << endl;
}

Altmer::~Altmer()
{
	if (isLeaving() && (profession == "Bard" || profession == "Performer"))
	{
		cout << getName() << " has left for the Summerset Isles." << endl;
	}
	else if (isLeaving() && profession == "Mage")
	{
		cout << getName() << " summons a portal and walks through. The portal disappears." << endl;
	}
	else if (!getDead())
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
	int sacramentRoll = rand() % 100;
	int actionRoll = rand() % 100;
	// actionRoll needs to be below these thresholds for the Altmer to actually do something.
	// If this Altmer has less than MIN_GOLD left, they will always do their job.
	const int BARD_CHANCE = 75, PERFORM_CHANCE = 65, MAGE_CHANCE = 100, MIN_GOLD = 5;

	if ((profession == "Bard" || profession == "Performer") && daysInTown > daysUntilLeave)
	{
		cout << getName() << ": My time in this town is unfortunately nearing it's end. I wish you all the best, and if you have the time, come see the House of Reveries in Rellenthil!" << endl;
		leave();
		return;
	}
	else if (profession == "Mage" && checkWealth() < MAGE_LEAVE_GOLD_THRESHOLD)
	{
		daysInTown++;
	}
	else if (profession == "Mage")
	{
		daysInTown = 0;
	}

	if (profession == "Mage" && checkWealth() < MAGE_LEAVE_GOLD_THRESHOLD && daysInTown >= daysUntilLeave)
	{
		cout << getName() << ": Hmm, this town doesn't really provide me with many research opportunities. I think it's time to return to Eyevea." << endl;
		leave();
		return;
	}

	// Now do upkeep
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
		mage();
	}
	else if (sacramentRoll < blackSacramentChance)
	{
		blackSacrament();
	}
	payTaxes();

	if (!getDead())
	{
		cout << getName() << " has " << checkWealth() << " Gold remaining." << endl;
	}

	if (profession == "Bard" || profession == "Performer")
	{
		daysInTown++;
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
	int revenue = 6;

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
				revenue += rand() % 4; // Up to 3 additional gold will be earned in tips.
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
				revenue += rand() % 4; // Up to 3 additional gold will be earned in tips.
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
			revenue += rand() % 4; // Up to 3 additional gold will be earned in tips.
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

// Mages will do research and publish their findings. If it does well, they get paid a large sum of money.
// However, research takes time, so mages won't get paid every day (unless, of course, they are really good at their job (unlikely)).
void Altmer::mage()
{
	if (!getDead())
	{
		const int PUBLISHING_QUALITY = 100; // Quality should be above this number in order to publish.
		const int CRIT_FAIL_CHANCE = 10; // Chance of a failure causing a death.
		const int DEFAULT_PAY = 40, MIN_GOLD = 4; // If the mage has less than MIN_GOLD remaining, they will publish their work regardless of its quality.
		const vector<string> RESEARCH_TOPICS = {"Daedra", "Restoration Magic", "Alteration Magic", "Destruction Magic", "Conjuration Magic", "Illusion Magic", "Alchemy", "Enchanting"};

		if (researchTopic == -1)
		{
			researchTopic = rand() % RESEARCH_TOPICS.size();
			cout << getName() << " begins researching the topic of " << RESEARCH_TOPICS[researchTopic] << "." << endl;
		}
		else if (researchQuality < 100 && checkWealth() >= MIN_GOLD)
		{
			cout << getName() << " continues their research into " << RESEARCH_TOPICS[researchTopic] << "." << endl;
		}
		else if (researchQuality >= 100 || checkWealth() < MIN_GOLD)
		{
			int qualityDiff = researchQuality - PUBLISHING_QUALITY;
			int additionalPay = qualityDiff / 2; // Each additional point of quality is worth about 0.5 gold, with the total amount rounded down.
			int randomPayAmount = rand() % 21 - 10; // -10 to +10 additional gold in addition to that given by quality.
			int totalPay = DEFAULT_PAY + additionalPay + randomPayAmount;
			// The Guild shouldn't be taking away any money...
			if (totalPay < 5)
			{
				totalPay = 5; // Minimum amount of Gold that can be received is 5.
			}

			//cout << "researchQuality: " << researchQuality << endl;
			cout << getName() << " has published their research on " << RESEARCH_TOPICS[researchTopic] << ", and received " << totalPay << " Gold from the Mages Guild." << endl;
			getPaid(totalPay, false);
			// Reset topic and quality
			researchQuality = 0;
			researchTopic = -1;
			return;
		}

		int failRoll = rand() % 100;
		if (failRoll > jobSkill) // The higher the jobSkill, the less likely things are to go wrong.
		{
			int critFailRoll = rand() % 100;
			if (critFailRoll < CRIT_FAIL_CHANCE) // On a critical fail, a random citizen in the town dies.
			{
				Citizen* unfortunateCitizen = myTown->getRandomCitizen();
				cout << getName() << ": Wait a minute, that's not right-Oh no, this is very bad..." << endl;
				cout << "One of " << getName() << "'s experiments has failed spectacularly, injuring several citizens and killing " << unfortunateCitizen->getName() << "!" << endl;
				unfortunateCitizen->kill();
				return;
			}

			// A regular fail results in a setback for the mage's research.
			cout << getName() << ": ...Did I do something wrong? These results aren't right at all." << endl;
			cout << getName() << "'s research progress has been set back due to a failed experiment." << endl;
			int setbackSeverity = rand() % 11 + 10; // 10-20
			researchQuality -= setbackSeverity;
			return;
		}

		// Increasing or decreasing quality.
		const int CONST_QUALITY_INCREASE = 25; // On average it should take 4 days to finish something.
		int skillModifier = rand() % (jobSkill + 1) - 40; // Increase in quality is dependent on jobSkill. The mage can also lower the quality of their work.
		researchQuality += CONST_QUALITY_INCREASE + skillModifier;
		//cout << "researchQuality: " << researchQuality << endl;
	}
}
