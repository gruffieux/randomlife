#ifndef CBATTLEINFO
#define CBATTLEINFO

#include <cpanel.h>

class BattleInfo : public Panel
{
private:
public:
	BattleInfo() {}
	BattleInfo(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~BattleInfo() {}
	void Init();
	bool Refresh();
};

#endif