#ifndef CCHARSTATLIST
#define CCHARSTATLIST

#include "ccharstat.h"

class CharStatList : public List
{
private:
public:
	CharStatList() {}
	CharStatList(int List_Order) : List(List_Order) {}
	~CharStatList() {}
	CharStat* GetStat(int StatId);
	CharStat* GetBestStat();
	CharStat* getCharStatElement(int index) {return dynamic_cast<CharStat*>(GetElement(index));}
};

#endif