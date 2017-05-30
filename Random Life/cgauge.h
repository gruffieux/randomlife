#ifndef CGAUGE
#define CGAUGE

#include <cpanel.h>
#include "ccharacter.h"

class Gauge : virtual public Panel
{
private:
protected:
	Panel *PActions;
	Character *Focus;
public:
	Gauge();
	Gauge(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~Gauge() {;}
	void SetFocus(Character *Gauge_Focus);
	void Init();
	void Initialize();
	void initAni();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif