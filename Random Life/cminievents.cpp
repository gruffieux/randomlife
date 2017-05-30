#include "cminievents.h"

extern Display *Screen;

void MiniEvents::ReleaseEvent(int x, int y, Str Message, COLORREF Color)
{
	MiniEvent *Event = new MiniEvent(Color, x, y);
	Event->SetMagnitude(1.9);
	Event->SetDeceleration(0.01);
	Event->SetAngle(3 * PI / 2);

	AddElement((Text*)Event);

	Screen->LoadText(Event->GetSprite(), NULL, Message, RGB(255, 255, 255), Color);
	Event->GetSprite()->SetColorKey(RGB(255, 255, 255));

	Event->getDimension()->x = Event->GetSprite()->GetWidth();
	Event->getDimension()->y = Event->GetSprite()->GetHeight();
}

void MiniEvents::ScrollEvents()
{
	int i = 0;

	while (ElementExist(i))
	{
		GetMiniEventElement(i)->Move();
		GetMiniEventElement(i)->DecreaseMagnitude(0);
		if (GetMiniEventElement(i)->GetMagnitude() >= 1)
		{
			Screen->Blit(GetMiniEventElement(i)->getPosition()->x, GetMiniEventElement(i)->getPosition()->y, GetMiniEventElement(i)->GetSprite(), NULL, GetMiniEventElement(i)->GetRect(), DDBLT_WAIT | DDBLT_KEYSRC);
			i++;
		}
		else
			RemoveElement(i, 1, true);
	}
}

void MiniEvents::GenerateCollisions()
{
	int i, j;

	for (i = 0; i < ElementCount; i++)
		for (j = 0; j < ElementCount; j++)
			GetMiniEventElement(i)->GenerateCollision(GetMiniEventElement(j));
}
