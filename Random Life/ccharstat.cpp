#include "ccharstat.h"

CharStat::CharStat(CharStat &model)
{
	StatId = model.GetStatId();
	score = model.GetScore();
	TempScore = model.GetTempScore();
	modifier = model.GetModifier();
	Description = model.GetDescription();
	Label = model.GetLabel();
}
