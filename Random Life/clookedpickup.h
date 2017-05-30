#ifndef CLOOKEDPICKUP
#define CLOOKEDPICKUP

#include <cpanel.h>
#include "cpickup.h"
#include "cplayer.h"

class LookedPickup : public Panel
{
private:
	Pickup *Focus;
	Player *Observer;
public:
	LookedPickup();
	LookedPickup(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~LookedPickup() {;}
	void SetFocus(Pickup *LP_Focus) {Focus = LP_Focus;}
	void SetObserver(Player *LP_Observer) {Observer = LP_Observer;}
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	Pickup * GetFocus() {return Focus;}
};

#endif