#ifndef CQUESTBOOK
#define CQUESTBOOK

#include <cpanel.h>
#include <celevator.h>
#include <cmsgreporter.h>
#include "main.h"

class QuestBook : public Panel, public List
{
private:
	int status[TOTAL_CHAR_PNJ_QUEST], AcolytKill, PotionGift, CurseStatus;
	short comment[TOTAL_BOOK_COMMENT];
	bool MaxHeal, MinPotionGift;
	Elevator *Selector;
	List CommentPack;
public:
	static const int ORDER_BY_DAY = 10;
	static const int ORDER_BY_QUEST = 11;
	static const int ORDER_BY_COMMENT = 12;
	QuestBook();
	QuestBook(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height);
	~QuestBook();
	void SetStatus(int index, int value) {status[index] = value;}
	void SetAcolytKill(int QB_AcolytKill) {AcolytKill = QB_AcolytKill;}
	void SetPotionGift(int QB_PotionGift) {PotionGift = QB_PotionGift;}
	void SetCurseStatus(int QB_CurseStatus) {CurseStatus = QB_CurseStatus;}
	void SetComment(int index, int value) {comment[index] = value;}
	void SetMaxHeal(bool QB_MaxHeal) {MaxHeal = QB_MaxHeal;}
	void SetMinPotionGift(bool QB_MinPotionGift) {MinPotionGift = QB_MinPotionGift;}
	void IncreaseAcolytKill();
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	void Start();
	void AddComment(int day, bool ShowIt, BOOK_COMMENT MsgID, MsgReporter *pMsgReporter=NULL);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	int OrderBy(int NewOrder, Container *pContainer);
	bool GetFinalQuest() {return ((status[WARRIOR_QUEST] == 3) && (status[PRIEST_QUEST] == 3));}
	int GetStatus(int index) {return status[index];}
	int GetAcolytKill() {return AcolytKill;}
	int GetPotionGift() {return PotionGift;}
	int GetCurseStatus() {return CurseStatus;}
	short GetComment(int index) {return comment[index];}
	bool GetMaxHeal() {return MaxHeal;}
	bool GetMinPotionGift() {return MinPotionGift;}
	Elevator * GetSelector() {return Selector;}
	List * GetCommentPack() {return &CommentPack;}
};

#endif