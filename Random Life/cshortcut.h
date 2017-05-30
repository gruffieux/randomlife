#ifndef CSHORTCUT
#define CSHORTCUT

#include "ciconlist.h"

class ShortCut : public IconList
{
private:
public:
	ShortCut() {;}
	ShortCut(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~ShortCut() {;}
	void Init();
	void initAni();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif