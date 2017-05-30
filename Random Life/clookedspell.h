#ifndef CLOOKEDSPELL
#define CLOOKEDSPELL

#include <cpanel.h>
#include "cmagicspell.h"
#include "cplayer.h"

class LookedSpell : public Panel
{
private:
	bool learn;
	MagicSpell *Focus;
	Player *Observer;
public:
	LookedSpell();
	LookedSpell(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~LookedSpell() {;}
	void SetLearn(bool LS_learn) {learn = LS_learn;}
	void SetFocus(MagicSpell *LS_Focus) {Focus = LS_Focus;}
	void SetObserver(Player *LS_Observer) {Observer = LS_Observer;}
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	bool GetLearn() {return learn;}
	MagicSpell * GetFocus() {return Focus;}
};

#endif