#ifndef CMENU
#define CMENU

#include <cpanel.h>

class Menu : public Panel
{
private:
	Str IntroText;
	Panel *ButtonsContainer, *Button[3];
public:
	Menu();
	Menu(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~Menu();
	void Init();
	void initAni();
	void Initialize();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif