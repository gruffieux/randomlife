#ifndef CDIALOGUE
#define CDIALOGUE

#include <cpanel.h>
#include "cpnj.h"
#include "cplayer.h"
#include "main.h"

class Dialogue : public Panel
{
private:
	int step[TOTAL_CHAR_PNJ_QUEST];
	List QuestMsg, AnswerYesMsg, AnswerNoMsg;
	Pnj *Speaker;
	Player *Listener;
public:
	Dialogue();
	Dialogue(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~Dialogue() {DestroyMsg();}
	void SetStep(int index, int value) {step[index] = value;}
	void SetSpeaker(Pnj *Dialogue_Speaker) {Speaker = Dialogue_Speaker;}
	void SetListener(Player *Dialogue_Listener) {Listener = Dialogue_Listener;}
	bool show();
	bool hide();
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	void InitMsg();
	void DestroyMsg();
	void NextStep();
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	int GetStep(int index) {return step[index];}
	Pnj * GetSpeaker() {return Speaker;}
};

#endif