#include "cmob.h"

Mob::Mob()
{
	TargetNode = NULL;
	CurrentTarget = NULL;
}

Mob::Mob(Mob &model)
{
	int x, y;

	TargetNode = model.GetTargetNode();
	CurrentTarget = model.GetCurrentTarget();

	for (x = 0; x < TOTAL_ZONE_X; x++)
		for (y = 0; y < TOTAL_ZONE_Y; y++)
			ZoneStatus[y][x] = *model.GetZoneStatus(x, y);
}

void Mob::SelectNearestTarget()
{
	if (Dests.GetElementCount())
		CurrentTarget = Dests.GetTileActorElement(0);
	else
		CurrentTarget = NULL;
}

bool Mob::SelectAnotherTarget()
{
	int i, j;

	Dests.OrderBy(TileActorList::ORDER_BY_RANGE);
	i = Dests.SearchElement(CurrentTarget);

	for (j = 0; j < Dests.GetElementCount(); j++)
		if (j != i)
		{
			CurrentTarget = Dests.GetTileActorElement(j);
			return true;
		}

	return false;
}

bool Mob::CanAttack(TileActor *pTileActor)
{
	short SearchResult = -1;

	Compass.SetCheckObstacle(true);
	Compass.StartSearch(TileNodeID, pTileActor->GetTileNodeID());

	do SearchResult = Compass.Search();
	while (SearchResult == 0);

	if ((GetAdjacent(pTileActor) == true) && (SearchResult == 1)) return true;

	return false;
}
