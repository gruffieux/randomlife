#ifndef CASKFORSAVE
#define CASKFORSAVE

#include <cpanel.h>

class AskForSave : public Panel
{
private:
	int SkillPoints, MagicPoints;
	Panel *Choices;
public:
	AskForSave() {Choices = new Panel();}
	AskForSave(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~AskForSave() {delete Choices;}
	void SetRemainingPoints(int AFS_SkillPoints, int AFS_MagicPoints);
	void Init();
	void Initialize();
	void initAni();
	void restoreAni();
	void destroyAni();
	void DestroyText();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	bool CanMove(Item *pItem);
	bool CanClick(Item *pItem);
};

#endif