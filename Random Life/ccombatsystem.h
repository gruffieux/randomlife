#ifndef CCOMBATSYSTEM
#define CCOMBATSYSTEM

#include <cobject.h>
#include <clist.h>
#include <cpanel.h>
#include <cmsgreporter.h>
#include "ccharacter.h"

class CombatSystem : public Object
{
private:
	int turn, phase, SpellCasting, StartNodeID;
	bool running;
	Panel *PTurn;
	List CombatantPack;
	void FindNextTurn();
	void FindPreviousTurn();
	void NextTurn();
public:
	CombatSystem();
	CombatSystem(Panel *CS_ITurn);
	CombatSystem(CombatSystem &model);
	~CombatSystem() {;}
	void SetPTurn(Panel *pPanel) {PTurn = pPanel;}
	bool AddCombatant(Character *pCharacter, MsgReporter *pMsgReporter=NULL);
	bool RemoveCombatant(Character *pCharacter, MsgReporter *pMsgReporter=NULL);
	bool Incomming(Character *Attacker, MsgReporter *pMsgReporter=NULL);
	void RunRound(MsgReporter *pMsgReporter=NULL);
	bool CheckCombatant(Character *pCharacter);
	bool WaitForNextTurn(Character *pCharacter);
	void End(MsgReporter *pMsgReporter=NULL);
	int GetTurn() {return turn;}
	int GetPhase() {return phase;}
	int GetSpellCasting() {return SpellCasting;}
	int GetStartNodeID() {return StartNodeID;}
	bool GetRunning() {return running;}
	Panel * GetPTurn() {return PTurn;}
	List * GetCombatantPack() {return &CombatantPack;}
};

#endif