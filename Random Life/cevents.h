#ifndef CEVENTS
#define CEVENTS

#include <cpanel.h>
#include <celevator.h>

class Events : public Panel
{
private:
	Elevator *Selector;
public:
	Events() {Selector = new Elevator();}
	Events(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height);
	~Events() {delete Selector;}
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	Elevator * GetSelector() {return Selector;}
};

#endif