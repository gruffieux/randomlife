#ifndef CMINIEVENTS
#define CMINIEVENTS

#include <clist.h>
#include "cminievent.h"

class MiniEvents : public List
{
private:
public:
	MiniEvents() {}
	~MiniEvents() {RemoveAllElement(0, true);}
	void ReleaseEvent(int x, int y, Str Message, COLORREF Color);
	void ScrollEvents();
	void GenerateCollisions();
	MiniEvent * GetMiniEventElement(int index) {return dynamic_cast<MiniEvent*>(GetElement(index));}
};

#endif