#include "cpickuplist.h"

int PickupList::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_PTYPE:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = getPickupElement(i)->GetType();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}
