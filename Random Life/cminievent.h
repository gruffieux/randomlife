#ifndef CMINIEVENT
#define CMINIEVENT

#include <ctext.h>
#include <cmover.h>

class MiniEvent : public Text, public Mover
{
private:
public:
	MiniEvent() {}
	MiniEvent(COLORREF Text_FGColor, int Item_xpos, int Item_ypos);
	~MiniEvent() {}
	bool GenerateCollision(MiniEvent *pMiniEvent);
};

#endif