#include "ccharstatlist.h"

CharStat * CharStatList::GetStat(int StatId)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (getCharStatElement(i)->GetStatId() == StatId)
			return getCharStatElement(i);

	return NULL;
}

CharStat * CharStatList::GetBestStat()
{
	int i;
	CharStat *BestStat = NULL;

	for (i = 0; i < ElementCount; i++)
		if (!BestStat || getCharStatElement(i)->GetScore() > BestStat->GetScore())
			BestStat = getCharStatElement(i);

	return BestStat;
}
