#ifndef CSTATSINFO
#define CSTATSINFO

#include <cpanel.h>
#include "cplayer.h"

class StatsInfo : public Panel
{
private:
	bool magic;
	Panel *Mother;
	Player *Focus;
public:
	StatsInfo();
	StatsInfo(bool SI_magic, int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height, Str Object_Name);
	~StatsInfo() {;}
	void SetMother(Panel *SI_Mother) {Mother = SI_Mother;}
	void SetFocus(Player *SI_Focus) {Focus = SI_Focus;}
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif