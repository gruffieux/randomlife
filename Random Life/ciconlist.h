#ifndef CICONLIST
#define CICONLIST

#include <cpanel.h>
#include <celevator.h>

class IconList : public Panel
{
protected:
	Elevator *Selector;
public:
	IconList() {Selector = NULL;}
	IconList(bool UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name);
	IconList(bool UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, int Item_width, int Item_height, Str Object_Name);
	~IconList() {if (Selector) delete Selector;}
	void Init();
	void initAni();
	void InitIcons(int BmpId, Str SkinId, int nFrame, int width, int height);
	void Initialize();
	void restoreAni();
	void destroyAni();
	bool Refresh();
	Elevator * GetSelector() {return Selector;}
};

#endif