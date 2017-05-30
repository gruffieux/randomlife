#include "ccombatsystem.h"
#include "chumanized.h"

CombatSystem::CombatSystem()
{
	turn = phase = SpellCasting = 0;
	StartNodeID = -1;
	running = false;
	PTurn = NULL;
}

CombatSystem::CombatSystem(Panel *CS_ITurn)
{
	turn = phase = SpellCasting = 0;
	StartNodeID = -1;
	running = false;
	PTurn = CS_ITurn;
}

CombatSystem::CombatSystem(CombatSystem &model)
{
	turn = model.GetTurn();
	phase = model.GetPhase();
	SpellCasting = model.GetSpellCasting();
	StartNodeID = model.GetStartNodeID();
	running = model.GetRunning();
	CombatantPack = *model.GetCombatantPack();
	PTurn = model.GetPTurn();
}

bool CombatSystem::AddCombatant(Character *pCharacter, MsgReporter *pMsgReporter)
{
	if (CombatantPack.AddElement(pCharacter, true))
	{
		pCharacter->EngageCombat(pMsgReporter);
		return true;
	}

	return false;
}

bool CombatSystem::RemoveCombatant(Character *pCharacter, MsgReporter *pMsgReporter)
{
	int PosInList = CombatantPack.SearchElement(pCharacter);

	if (PosInList == -1) return false;

	pCharacter->ExitCombat(pMsgReporter);

	if (!CombatantPack.RemoveElement(pCharacter)) return false;

	// Si le combatant était en cours de tour ...
	if (PosInList == turn)
	{
		// ... Si l'indice du tour en cours dépasse le nombre de combatants ...
		// ... On passe au tour suivant pour corriger
		if (turn > CombatantPack.GetElementCount() - 1) FindNextTurn();
		// Dans un autre cas, l'indice du tour doit être correct
	}
	else
		// Si le combatant était avant l'indice du tour en cours ...
		// ... On passe au tour précédent pour garder le pointeur sur le combatant en cours de tour
		if (PosInList < turn) FindPreviousTurn();
		// Dans un autre cas, l'indice du tour doit être correct

	return true;
}

bool CombatSystem::Incomming(Character *Attacker, MsgReporter *pMsgReporter)
{
	if (AddCombatant(Attacker->GetTarget(), pMsgReporter))
		Attacker->GetTarget()->SetTarget(Attacker);

	Attacker->GetTarget()->GetAttackedBy()->AddElement(Attacker, true);
	AddCombatant(Attacker, pMsgReporter);
	Attacker->GetAttackedBy()->AddElement(Attacker->GetTarget(), true);

	// Premier tour, l'attaquant joue en dernier
	if (!running)
	{
		turn = 1; // C'est le tour de l'attaquant!
		phase = 1;
		StartNodeID = Attacker->GetTileNodeID();
		running = true;
		return true;
	}

	return false;
}

void CombatSystem::RunRound(MsgReporter *pMsgReporter)
{
	if ((Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->stunned == true) | (Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->WpStunned == true))
	{
		Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->WpStunned = false;
		NextTurn();
	}

	if (!Character::getCharacterElement(turn, &CombatantPack)->GetSwatch(SPELL_CASTING)->isGoing())
		Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->access = true;
	Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->turn = true;

	switch (Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->Action)
	{
	//Mouvements
	//Ref: "The Freestyle RPG Basic Rules", page 17-18.
	case MOVEMENT:
		if ((TileActor::getTileActorElement(turn, &CombatantPack)->GetRange(StartNodeID) >= Character::getCharacterElement(turn, &CombatantPack)->GetCombat()->MaxDistance))
			if (Character::getCharacterElement(turn, &CombatantPack)->StopMove()) NextTurn();
		break;
	//Attaque!
	//Ref: "The Freestyle RPG Basic Rules", page 18.
	case ATTACK:
		if (!Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->attack) NextTurn();
		break;
	case CAST_SPELL:
		if (!Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->concentrate)
		{
			SpellCasting = 0;
			NextTurn();
		}
		else
			//On accorde 3 secondes max de cast pour le sort en cours, passé ce délai le cast continue au prochain tour
			if (Character::getCharacterElement(turn, &CombatantPack)->GetSwatch(SPELL_CASTING)->getCounter() >= SpellCasting + 3)
			{
				SpellCasting = Character::getCharacterElement(turn, &CombatantPack)->GetSwatch(SPELL_CASTING)->getCounter();
				NextTurn();
			}
		break;
	default:
		break;
	}

	if (!CheckCombatant(Character::getCharacterElement(turn, &CombatantPack)))
		RemoveCombatant(Character::getCharacterElement(turn, &CombatantPack), pMsgReporter);
}

bool CombatSystem::CheckCombatant(Character *pCharacter)
{
	int i, j;

	for (i = 0; i < CombatantPack.GetElementCount(); i++)
		if (Character::getCharacterElement(i, &CombatantPack) != pCharacter)
		{
			j = pCharacter->GetAttackedBy()->SearchElement(CombatantPack.GetElement(i));
			// Le personnage est toujours attaqué par un combatant
			if (j != -1)
			{
				// Si il n'a plus de cible on prend une nouvelle
				if (pCharacter->HasNoTarget())
					pCharacter->SetTarget(Character::getCharacterElement(j, pCharacter->GetAttackedBy()));
				return true;
			}
			// Le combatant est attaqué par ce personnage
			if (Character::getCharacterElement(i, &CombatantPack)->GetAttackedBy()->SearchElement(pCharacter) != -1)
				return true;
			// Le personnage a toujours une cible dans le combat
			if (pCharacter->GetTarget() == Character::getCharacterElement(i, &CombatantPack))
				return true;
		}

	return false;
}

bool CombatSystem::WaitForNextTurn(Character *pCharacter)
{
	int CombatantID = CombatantPack.SearchElement(pCharacter);

	if (turn == CombatantID)
	{
		NextTurn();
		return true;
	}

	return false;
}

void CombatSystem::End(MsgReporter *pMsgReporter)
{
	int i;

	running = false;
	if (PTurn) PTurn->hide();

	for (i = 0; i < CombatantPack.GetElementCount(); i++)
		Character::getCharacterElement(i, &CombatantPack)->ExitCombat(pMsgReporter);

	CombatantPack.RemoveAllElement();
}

void CombatSystem::FindNextTurn()
{
	if (turn < CombatantPack.GetElementCount() - 1)
		turn++; //Combatant suivant
	else
	{
		turn = 0;
		phase++; //Phase suivante
	}

	if ((turn == 0) && (phase >= TOTAL_COMBAT_PHASE + 1))
		phase = 1; //Nouveau round
}

void CombatSystem::FindPreviousTurn()
{
	if (turn > 0)
		turn--;
	else
		turn = CombatantPack.GetElementCount() - 1;
}

void CombatSystem::NextTurn()
{
	Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->access = false;
	Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->turn = false;
	Character::getCharacterElement(turn, &CombatantPack)->GetStatus()->Action = NONE_CHAR_ACTION; // Attention il faut que le personnage soit arrêté!
	Visual::getVisualElement(turn, &CombatantPack)->setPlan(2);

	FindNextTurn();

	//Si le combatant n'a pas d'action a ce tour, il passe son tour.
	if (!Character::getCharacterElement(turn, &CombatantPack)->GetCombat()->action[phase-1])
		NextTurn();
	else
	{
		StartNodeID = TileActor::getTileActorElement(turn, &CombatantPack)->GetTileNodeID();
		Visual::getVisualElement(turn, &CombatantPack)->setPlan(1);
		//Si il est sous l'effet d'un sort durable par tour, on incrémente la durée pour ce tour
		if (Humanized::getHumanizedElement(turn, &CombatantPack))
			if ((Humanized::getHumanizedElement(turn, &CombatantPack)->GetCurrentSpell()->GetHit() == true) && (Humanized::getHumanizedElement(turn, &CombatantPack)->GetCurrentSpell()->GetCheckTurn() == 1))
				Humanized::getHumanizedElement(turn, &CombatantPack)->GetCurrentSpell()->GetEffectDuration()->increaseCounter(1);
	}

	if (PTurn) PTurn->Refresh();
}
