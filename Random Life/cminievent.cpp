#include "cminievent.h"

MiniEvent::MiniEvent(COLORREF Text_FGColor, int Item_xpos, int Item_ypos) :
Text(RGB(255, 255, 255), Text_FGColor),
Item(Item_xpos, Item_ypos, NULL, NULL, Str())
{
}

bool MiniEvent::GenerateCollision(MiniEvent *pMiniEvent)
{
	if (pMiniEvent == this) return false;

	if (Item::collision(pMiniEvent))
	{
		if (position.x == pMiniEvent->getPosition()->x)
		{
			if (position.y <= pMiniEvent->getPosition()->y)
				pMiniEvent->getPosition()->y = position.y + dimension.y + 5;
		}
		else
			if (position.x <= pMiniEvent->getPosition()->x)
				pMiniEvent->getPosition()->x = position.x + dimension.x + 5;
	}
	
	return true;
}
