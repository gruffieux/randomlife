#ifndef CQUESTCOMMENT
#define CQUESTCOMMENT

#include <cmsg.h>

class QuestComment : public Msg
{
private:
	int quest, day;
	Str Comment;
public:
	QuestComment() {quest = day = 0;}
	QuestComment(int QC_quest, int QC_day, Str QC_Comment);
	int GetQuest() {return quest;}
	int GetDay() {return day;}
	Str GetComment() {return Comment;}
	static QuestComment* getQuestCommentElement(int index, List *pList) {return dynamic_cast<QuestComment*>(pList->GetElement(index));}
};

#endif