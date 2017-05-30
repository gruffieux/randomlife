#ifndef CMOB
#define CMOB

#include <ctileia.h>
#include <cmsgreporter.h>
#include "cminievents.h"
#include "main.h"

class Mob : public TileIA
{
private:
	ActorNode *TargetNode;
	TileActor *CurrentTarget;
protected:
	CharZoneStatus ZoneStatus[TOTAL_ZONE_Y][TOTAL_ZONE_X];
public:
	Mob();
	Mob(Mob &model);
	~Mob() {}
	void SetTargetNode(ActorNode *pActorNode) {TargetNode = pActorNode;}
	void SelectNearestTarget();
	bool SelectAnotherTarget();
	bool CanAttack(TileActor *pTileActor);
	void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL) {Dests.RemoveElement(Corpse);}
	virtual void Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL) {ZoneStatus[ZoneCoordY][ZoneCoordX].dead = 1;}
	ActorNode* GetTargetNode() {return TargetNode;}
	CharZoneStatus* GetZoneStatus(int x, int y) {return &ZoneStatus[y][x];}
	TileActor* GetCurrentTarget() {return CurrentTarget;}
	static Mob* getMobElement(int index, List *pList) {return dynamic_cast<Mob*>(pList->GetElement(index));}
};

#endif