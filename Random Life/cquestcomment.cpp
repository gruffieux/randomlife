#include <windows.h>
#include "cquestcomment.h"
#include "main.h"

QuestComment::QuestComment(int QC_quest, int QC_day, Str QC_Comment)
{
	quest = QC_quest;
	day = QC_day;
	Comment = QC_Comment;

	switch (quest)
	{
	case WARRIOR_QUEST:
		color = RGB(128, 128, 128);
		break;
	case PRIEST_QUEST:
		color = RGB(0, 128, 255);
		break;
	case FINAL_QUEST:
		color = RGB(0, 0, 0);
		break;
	}

	name = BuildString("jour %d: %s", day, Comment.Get());
}
