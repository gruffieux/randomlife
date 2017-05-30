#ifndef CCOMBATSYSTEMLIST
#define CCOMBATSYSTEMLIST

#include "ccombatsystem.h"

class CombatSystemList : public List
{
private:
public:
	CombatSystemList() {}
	~CombatSystemList() {RemoveAllElement(0, true);}
	bool DispatchCombatant(Character *pCharacter, Panel *pPanel, MsgReporter *pMsgReporter=NULL);
	int SearchElementWithCombatant(Character *pCharacter);
	int SearchElementWithPanel();
	CombatSystem * GetCombatSystemElement(int index) {return dynamic_cast<CombatSystem*>(GetElement(index));}
};

#endif