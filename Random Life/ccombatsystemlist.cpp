#include "ccombatsystemlist.h"

bool CombatSystemList::DispatchCombatant(Character *pCharacter, Panel *pPanel, MsgReporter *pMsgReporter)
{
	int i, j;
	CombatSystem *Element;

	if ((pCharacter->GetStatus()->attacking == false) | (pCharacter->HasNoTarget() == true) | (pCharacter->GetTarget()->GetStatus()->dead == true))
		return false;

	if ((pCharacter->GetAdjacent(pCharacter->GetTarget()) == false) && (pCharacter->GetTarget()->GetStatus()->hitted == false) && (pCharacter->GetTarget()->GetStatus()->ResistSpell == false))
		return false;
	
	if ((pCharacter->StopMove() == false) | (pCharacter->GetTarget()->StopMove() == false))
		return false;

	i = SearchElementWithCombatant(pCharacter);
	j = SearchElementWithCombatant(pCharacter->GetTarget());

	// L'attaquant et sa cible ne sont ni l'un ni l'autre dans un combat
	if ((i == -1) && (j == -1))
	{
		Element = new CombatSystem();
		AddElement(Element);
	}
	else
		// L'attaquant est pas dans un combat mais sa cible oui
		if ((i == -1) && (j != -1))
			Element = GetCombatSystemElement(j);
		else
			// L'attaquant est déjà dans un combat mais pas sa cible
			if ((i != -1) && (j == -1))
				Element = GetCombatSystemElement(i);
			else
				Element = NULL;

	pCharacter->GetStatus()->attacking = pCharacter->GetTarget()->GetStatus()->spy = false;

	if (Element)
	{
		if ((pCharacter->GetBehavior()->progress == true) | (pCharacter->GetTarget()->GetBehavior()->progress == true))
		{
			Element->SetPTurn(pPanel);
			if (Element->GetPTurn())
			{
				Element->GetPTurn()->show();
				Element->GetPTurn()->Refresh();
			}
		}
		return ((Element->Incomming(pCharacter, pMsgReporter) == true) && (Element->GetPTurn() != NULL));
	}

	return false;
}

int CombatSystemList::SearchElementWithCombatant(Character *pCharacter)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (GetCombatSystemElement(i)->GetCombatantPack()->SearchElement(pCharacter) != -1) return i;

	return -1;
}

int CombatSystemList::SearchElementWithPanel()
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (GetCombatSystemElement(i)->GetPTurn()) return i;

	return -1;
}
