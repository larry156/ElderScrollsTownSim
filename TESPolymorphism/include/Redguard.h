#ifndef REDGUARD_H
#define REDGUARD_H

#include <Human.h>
#include <string>

class Redguard : public Human
{
	public:
		Redguard(std::string myName);
		virtual ~Redguard();
		void upkeep(Citizen* target);

	protected:

	private:
		void adventure();
		void mercenary();
};

#endif // REDGUARD_H
