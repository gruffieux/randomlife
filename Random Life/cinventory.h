#ifndef CINVENTORY
#define CINVENTORY

#include <cpanel.h>
#include "chumanized.h"
#include "cpocket.h"

class Inventory : public Panel
{
private:
	Humanized *Focus;
	Pocket *Others, *Weapons, *Armors, *Ingredients;
public:
	Inventory();
	Inventory(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name);
	~Inventory();
	void SetFocus(Humanized *Inventory_Focus);
	void Init();
	void Initialize();
	void initAni();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	void AddForbidden2(Item *pItem);
	void RemoveForbidden2(Item *pItem);
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	bool CanMove(Item *pItem);
	bool CanClick(Item *pItem);
	Humanized * GetFocus() {return Focus;}
	Pocket * GetOthers() {return Others;}
	Pocket * GetWeapons() {return Weapons;}
	Pocket * GetArmors() {return Armors;}
	Pocket * GetIngredients() {return Ingredients;}
};

#endif