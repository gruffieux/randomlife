#ifndef CCHARINFO
#define CCHARINFO

#include <cpanel.h>
#include "ccharacter.h"
#include "cstatsinfo.h"

class CharInfo : public Panel
{
private:
	Character *Focus;
	Panel *PSkills, *PCombatSkills, *PMagicSkill, *PStats2;
	StatsInfo *PStats, *PMagicSchools;
public:
	CharInfo();
	CharInfo(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~CharInfo();
	void SetFocus(Character *CharInfo_Focus);
	void Init();
	void Initialize();
	void initAni();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	void AddForbidden2(Item *pItem);
	void RemoveForbidden2(Item *pItem);
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	Character * GetFocus() {return Focus;}
	Panel * GetPSkills() {return PSkills;}
	Panel * GetPCombatSkills() {return PCombatSkills;}
	Panel * GetPMagicSkill() {return PMagicSkill;}
	StatsInfo * GetPStats() {return PStats;}
	Panel * GetPStats2() {return PStats2;}
	StatsInfo * GetPMagicSchools() {return PMagicSchools;}
};

#endif