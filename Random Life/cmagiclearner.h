#ifndef CMAGICLEARNER
#define CMAGICLEARNER

#include <cpanel.h>
#include "chumanized.h"
#include "cspelllist.h"

class MagicLearner : public Panel
{
private:
	Humanized *Focus;
	SpellList *Cure, *Destruction, *Protection;
public:
	MagicLearner();
	MagicLearner(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~MagicLearner();
	void SetFocus(Humanized *ML_Focus);
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
	SpellList * GetCure() {return Cure;}
	SpellList * GetDestruction() {return Destruction;}
	SpellList * GetProtection() {return Protection;}
};

#endif