#ifndef CTARGETGAUGE
#define CTARGETGAUGE

#include "cgauge.h"
#include "cplayer.h"

class TargetGauge : public Gauge
{
private:
	Panel *PRange;
	Player *Observer;
public:
	TargetGauge();
	TargetGauge(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~TargetGauge() {delete PRange;}
	void SetObserver(Player *TargetGauge_Observer) {Observer = TargetGauge_Observer;}
	void Init();
	void Initialize();
	void initAni();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	bool Run(Item *Cursor);
	Panel * GetPRange() {return PRange;}
};

#endif