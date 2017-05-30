#ifndef CPICKUPLIST
#define CPICKUPLIST

#include <clist.h>
#include "cpickup.h"

class PickupList : public List
{
private:
public:
	static const int ORDER_BY_PTYPE = 10;
	PickupList() {}
	PickupList(int List_Order) : List(List_Order) {}
	~PickupList() {}
	int OrderBy(int NewOrder, Container *pContainer);
	Pickup* getPickupElement(int index) {return dynamic_cast<Pickup*>(GetElement(index));}
};

#endif