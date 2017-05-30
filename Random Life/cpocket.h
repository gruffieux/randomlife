#ifndef CPOCKET
#define CPOCKET

#include "ciconlist.h"
#include "cpickuplist.h"

class Pocket : public IconList
{
private:
	PickupList *Focus;
public:
	Pocket() {Focus = NULL;}
	Pocket(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, Str Object_Name);
	Pocket(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height, Str Object_Name);
	~Pocket() {;}
	void SetFocus(PickupList *Pocket_Focus) {Focus = Pocket_Focus;}
	void InitIcons(PICKUP_GROUP group);
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif