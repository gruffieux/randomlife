#include "cplayer.h"
#include "../RLRes/RLRes/resource.h"

extern List SoundPack;

Player::Player()
{
	int i;

	SkillPoints = GlobalHitCount = GlobalHittedCount = NameID = 0;
	AlwaysWalk = false;

	Crafting = NULL;
	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
		PotionElement[i] = NULL;
	MobTarget = NULL;
	PnjTarget = NULL;

	Behavior.progress = true;

	GenerateName();
}

Player::Player(Player &model)
{
	int i;

	SkillPoints = model.GetSkillPoints();
	GlobalHitCount = model.GetGlobalHitCount();
	GlobalHittedCount = model.GetGlobalHittedCount();
	NameID = model.GetNameID();
	AlwaysWalk = model.GetAlwaysWalk();

	Crafting = model.GetCrafting();
	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
		PotionElement[i] = model.getPotionElement(i);

	MobTarget = model.GetMobTarget();
	PnjTarget = model.GetPnjTarget();
}

Player::~Player()
{
	CombatSkillBarPack.RemoveAllElement(0, true);
	SkillBarPack.RemoveAllElement(0, true);
}

void Player::SetTarget(Character *pCharacter)
{
	Humanized::SetTarget(pCharacter);

	MobTarget = dynamic_cast<Mob*>(pCharacter);
	PnjTarget = dynamic_cast<Pnj*>(pCharacter);
}

bool Player::Init()
{
	int i;

	if (!Humanized::Init())
		return false;

	//Les barres d'expériences
	for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
		CombatSkillBarPack.AddElement(new EnergyBar(0, NULL, 50, 12, XP, NULL, NULL, 0, 16));
	for (i = 0; i < SkillPack.GetElementCount(); i++)
		SkillBarPack.AddElement(new EnergyBar(0, NULL, 50, 12, XP, NULL, NULL, 0, 16));
	MagicSkillBar = new EnergyBar(0, NULL, 50, 12, XP, NULL, NULL, 0, 16);

	return true;
}

void Player::initAni()
{
	int i;

	Humanized::initAni();

	// Les barres d'expériences
	for (i = 0; i < CombatSkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &CombatSkillBarPack)->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	for (i = 0; i < SkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &SkillBarPack)->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	MagicSkillBar->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
}

void Player::restoreAni()
{
	int i;

	Humanized::restoreAni();

	// Les potion craftées
	for (i = 0; i < CraftPack.GetElementCount(); i++)
		Pickup::getPickupElement(i, &CraftPack)->restoreAni();

	// Les barres d'expériences
	for (i = 0; i < CombatSkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &CombatSkillBarPack)->GetBar()->restoreAni();
	for (i = 0; i < SkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &SkillBarPack)->GetBar()->restoreAni();
	MagicSkillBar->GetBar()->restoreAni();
}

void Player::destroyAni()
{
	int i;

	Humanized::destroyAni();

	// Les potion craftées
	for (i = 0; i < CraftPack.GetElementCount(); i++)
		Pickup::getPickupElement(i, &CraftPack)->destroyAni();

	// Les barres d'expériences
	for (i = 0; i < CombatSkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &CombatSkillBarPack)->GetBar()->destroyAni();
	for (i = 0; i < SkillBarPack.GetElementCount(); i++)
		EnergyBar::getEnergyBarElement(i, &SkillBarPack)->GetBar()->destroyAni();
	MagicSkillBar->GetBar()->destroyAni();
}

void Player::AddPotionElement(int index, Ingredient *pIngredient)
{
	//Si c'est une fleur, lui ajoute un effet
	if (index == 0)
		Crafting->SetEffect((PICKUP_POTION_EFFECT)(pIngredient->GetChemicalID() / CHEMICAL_SCALE));

	PotionElement[index] = pIngredient;
}

EVENT_TYPE Player::GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i;

	Humanized::GenerateSkill(ORCData, pMsgReporter, pMiniEvents);

	for (i = 0; i < SkillPack.GetElementCount(); i++)
	{
		if (SkillPack.getCharSkillElement(i)->GetUsed())
			SkillPack.getCharSkillElement(i)->GenerateXP(NULL);
		if (SkillPack.getCharSkillElement(i)->ImproveLevel(MAX_CHAR_SKILL_LEVEL))
		{
			SkillPoints += BONUS_POINTS;
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "+1 niveau!", PROGRESS_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous atteignez le niveau %d dans la compétence %s", SkillPack.getCharSkillElement(i)->GetLevel(), SkillPack.getCharSkillElement(i)->GetDescription().Get()), PROGRESS_MSG_COLOR);
			return SKILL_UP;
		}
	}

	for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
		if (CombatSkillPack.getCharSkillElement(i)->ImproveLevel(MAX_CHAR_SKILL_LEVEL))
		{
			SkillPoints += BONUS_POINTS;
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "+1 niveau!", PROGRESS_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous atteignez le niveau %d dans la compétence de combat %s", CombatSkillPack.getCharSkillElement(i)->GetLevel(), CombatSkillPack.getCharSkillElement(i)->GetDescription().Get()), PROGRESS_MSG_COLOR);
			return COMBAT_SKILL_UP;
		}

	if (MagicSkill.ImproveLevel(MAX_CHAR_MAGIC_LEVEL))
	{
		MagicPoints += MAGIC_POINTS;
		MagicResistSkill = MagicSkill;
		if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "+1 niveau!", PROGRESS_MSG_COLOR);
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous atteignez le niveau %d dans la compétence de magie", MagicSkill.GetLevel()), PROGRESS_MSG_COLOR);
		return MAGIC_SKILL_UP;
	}

	return NONE_EVENT_TYPE;
}

void Player::GenerateName()
{
	switch (NameID)
	{
	case WARRIOR_QUEST:
		setName("Guerrier");
		break;
	case PRIEST_QUEST:
		setName("Prêtre");
		break;
	case FINAL_QUEST:
		setName("Héro");
		break;
	default:
		setName("Villageois");
		break;
	}

	DisplayedName = name;
}

EVENT_TYPE Player::LearnSpell(MagicSpell *pMagicSpell, List SpellPack, MsgReporter *pMsgReporter)
{
	int i;
	MagicSpell *pPreviousSpell = NULL;

	//Vérifie si le joueur n'est pas adjacent au Grand Mage
	if (!GetAdjacent(Target))
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 4);
		return ACTION_FAILED;
	}

	//Vérifie si le joueur est en train de combattre
	if (Status.fight)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 6);
		return ACTION_FAILED;
	}

	//Vérifie si le joueur a déjà ce sort
	if (MagicSpellPack[pMagicSpell->GetSchool()-1].SearchElement(pMagicSpell, true) != -1)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(LEARN_SPELL_DENIED, 0);
		return ACTION_FAILED;
	}

	//Vérifie si le joueur a le niveau requis pour ce sort
	if (MagicSkill.GetLevel() < pMagicSpell->GetLevel())
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(LEARN_SPELL_DENIED, 1);
		return ACTION_FAILED;
	}

	//Vérifie si le joueur a assez de points magiques
	if (MagicPoints < SPELL_PRICE)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(LEARN_SPELL_DENIED, 2);
		return ACTION_FAILED;
	}

	//Vérifie si le joueur a le sort précédent requis
	i = SpellPack.SearchElement(pMagicSpell);
	while (i)
	{
		i--;
		if (MagicSpell::getMagicSpellElement(i, &SpellPack)->GetEffect() == pMagicSpell->GetEffect())
		{
			pPreviousSpell = MagicSpell::getMagicSpellElement(i, &SpellPack);
			break;
		}
	}
	if (pPreviousSpell)
		if (MagicSpellPack[pMagicSpell->GetSchool()-1].SearchElement(pPreviousSpell, true) == -1)
		{
			PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
			if (pMsgReporter) pMsgReporter->AddMessage(LEARN_SPELL_DENIED, 3);
			return ACTION_FAILED;
		}

	//Si c'est tout bon, apprend le nouveau sort
	MagicSpell *LearnMagicSpell = new MagicSpell(*pMagicSpell);
	LearnMagicSpell->GetManaCost(StatPack.GetStat(WILLPOWER)->GetModifier(), true);
	MagicSpellPack[LearnMagicSpell->GetSchool()-1].AddElement(LearnMagicSpell);
	MagicPoints -= SPELL_PRICE;
	if (MagicPoints == 0)
		for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
			MagicSchoolPack.getCharStatElement(i)->SetTempScore(0);
	if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous apprenez le sort %s pour %d points magiques", LearnMagicSpell->GetDescription().Get(), SPELL_PRICE), LEARN_SPELL_MSG_COLOR);
	
	return ACTION_SUCCESS;
}

EVENT_TYPE Player::Drop(Pickup *pPickup, MsgReporter *pMsgReporter)
{
	int i, x, y;

	Start.SetNodeID(TileNodeID);
	Start.GenerateTileCoord(PATHNODE_ID_INTERVAL);
	x = Start.GetZone()->x;
	y = Start.GetZone()->y;

	if (pPickup->GetStatus()->equiped)
		if (UnEquip(pPickup, false, pMsgReporter) == ACTION_FAILED)
			return ACTION_FAILED;

	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
		if (pPickup == PotionElement[i])
		{
			PotionElement[i] = NULL;
			break;
		}

	pPickup->SetOwnerID(-1);
	pPickup->GetStatus()->picked = false;
	pPickup->setRemoved(false);
	pPickup->SetTileNodeID(Map->GetMapData()->TileNodeID[y][x]);
	pPickup->GetTileCoord()->x = x;
	pPickup->GetTileCoord()->y = y;
	Map->GetMapData()->TileID[y][x][ITEMS] = pPickup->GetTileID();
	pPickup->SetMapPosition(x, y);
	pPickup->CenterMapPosition();

	Pocket[pPickup->GetGroup()].RemoveElement(pPickup);
	GenerateEquipment();

	if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s jette %s", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get()), ACTION_MSG_COLOR);

	return ACTION_SUCCESS;
}

EVENT_TYPE Player::CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i, ChemicalRare, xp, PotionGiftBefore;
	EVENT_TYPE result = NONE_EVENT_TYPE;

	ChemicalRare = 0;

	if (Status.Action == CRAFT_POTION)
	{
		if (SkillPack.GetSkill(CHEMISTRY)->GetUsed())
		{
			SkillPack.GetSkill(CHEMISTRY)->SetUsed(false);
			if (SkillPack.GetSkill(CHEMISTRY)->GetResult() < FAILURE)
			{
				PlaySound(Sound::getSoundElement(CRAFT_SUCCESS, &SoundPack), 0, false);
				if (SkillPack.GetSkill(CHEMISTRY)->GetResult() == CRITICAL_SUCCESS)
					Crafting->SetPower(Crafting->GetPower() * 2);
				PotionGiftBefore = PotionGift.GetElementCount();
				if (AddToEquipment(Crafting, pMsgReporter, pMiniEvents) == ACTION_FAILED)
				{
					pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s a été détruite", Crafting->GetDisplayedName()->Get()), ACTION_MSG_COLOR);
					delete Crafting;
				}
				else
				{
					if (PotionGiftBefore < PotionGift.GetElementCount())
						result = POTIONGIFT_ADDED;
					CraftPack.AddElement(Crafting);
				}
			}
			else
				PlaySound(Sound::getSoundElement(CRAFT_FAILURE, &SoundPack), 0, false);
			if (SkillPack.GetSkill(CHEMISTRY)->GetResult() == CRITICAL_FAILURE)
			{
				PlaySound(Sound::getSoundElement(POTION_BREAK, &SoundPack), 0, false);
				Crafting->SetEffect(NONE_PICKUP_POTION_EFFECT);
			}
			if (SkillPack.GetSkill(CHEMISTRY)->GetResult() != FAILURE)
			{
				for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
				{
					ChemicalRare += PotionElement[i]->GetChemicalID();
					PotionElement[i]->SetOwnerID(-1);
					PotionElement[i]->GetStatus()->picked = false;
					Pocket[INGREDIENTS].RemoveElement(PotionElement[i]);
					PotionElement[i] = NULL;
				}
				if ((ChemicalRare == 30) && (SkillPack.GetSkill(CHEMISTRY)->GetResult() == CRITICAL_SUCCESS))
				{
					PlaySound(Sound::getSoundElement(CRAFT_SPECIAL, &SoundPack), 0, false);
					Crafting->SetEffect(INVISIBILITY);
				}
				Crafting = NULL;
			}
			GenerateEquipment();
			if (Behavior.progress)
			{
				xp = SkillPack.GetSkill(CHEMISTRY)->GainXP(NULL, NULL, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
				if ((xp > 0) && (pMiniEvents != NULL))
					pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
			}
		}
	}
	else
	{
		SkillPack.GetSkill(CHEMISTRY)->SetUsed(false);
		if (Status.hit)
			CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->GenerateXP(&GlobalHitCount);
		result = Humanized::CheckActionResult(ORCData, pMsgReporter, pMiniEvents);
	}

	return result;
}

void Player::CraftPotion(MsgReporter *pMsgReporter)
{
	if (Crafting)
		if ((PotionElement[0] != NULL) && (PotionElement[1] != NULL) && (PotionElement[2] != NULL))
		{
			Status.Action = CRAFT_POTION;
			CurrentSkill = SkillPack.GetSkill(CHEMISTRY)->Use();
		}
		else
		{
			PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
			pMsgReporter->AddMessage(ACTION_DENIED, 1);
		}
}

EVENT_TYPE Player::Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i;

	if (Humanized::Give(pPickup, pHumanized, pMsgReporter, pMiniEvents) == ACTION_SUCCESS)
	{
		i = CraftPack.SearchElement(pPickup);
		if (i != -1)
			CraftPack.RemoveElement(i, 1);
		return ACTION_SUCCESS;
	}

	return ACTION_FAILED;
}

bool Player::Consume(Potion *pPotion, List *PickupPack, MsgReporter *pMsgReporter)
{
	int i;

	if (Humanized::Consume(pPotion, PickupPack, pMsgReporter))
	{
		i = CraftPack.SearchElement(pPotion);
		if (i != -1)
		{
			CraftPack.RemoveElement(i, 1);
			PickupPack->RemoveElement(pPotion);
			pPotion->destroyAni();
			delete pPotion;
		}
		return true;
	}

	return false;
}

void Player::StopSpellEffect(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int xp;
	MagicSkill.GenerateXP(&GlobalHitCount);

	if ((pMagicSpell->GetEffect() == SELF_HEAL) && (Status.fight == false) && (pMiniEvents != NULL))
	{
		xp = MagicSkill.GainXP(pMagicSpell->GetLevel(), NULL, MAX_CHAR_MAGIC_LEVEL, pMsgReporter);
		if (xp) pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
	}

	Humanized::StopSpellEffect(pMagicSpell, pMsgReporter, pMiniEvents);
}

void Player::EngageCombat(MsgReporter *pMsgReporter)
{
	int i;

	for (i = 0; i < SkillPack.GetElementCount(); i++) SkillPack.getCharSkillElement(i)->InitXP();
	for (i = 0; i < CombatSkillPack.GetElementCount(); i++) CombatSkillPack.getCharSkillElement(i)->InitXP();
	MagicSkill.InitXP();
	Character::EngageCombat(pMsgReporter);
}

void Player::ExitCombat(MsgReporter *pMsgReporter)
{
	int i;

	for (i = 0; i < SkillPack.GetElementCount(); i++)
		SkillPack.getCharSkillElement(i)->InitXP();

	for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
		CombatSkillPack.getCharSkillElement(i)->InitXP();
	
	MagicSkill.InitXP();
	Character::ExitCombat(pMsgReporter);
	
	if (!Status.dead) setPlan(1);
}

void Player::WinCombat(Object *Corpse, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i, xp;
	Character *pCharacter = dynamic_cast<Character*>(Corpse);

	Character::WinCombat(Corpse, pMsgReporter, pMiniEvents);

	if (pCharacter)
	{
		xp = SkillPack.GetSkill(LIGHT_ARMOR)->GainXP(pCharacter->GetCurrentSkill()->GetLevel(), GlobalHittedCount, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
		if ((xp > 0) && (pMiniEvents != NULL))
			pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
		xp = SkillPack.GetSkill(HEAVY_ARMOR)->GainXP(pCharacter->GetCurrentSkill()->GetLevel(), GlobalHittedCount, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
		if ((xp > 0) && (pMiniEvents != NULL))
			pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
		for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
		{
			xp = CombatSkillPack.getCharSkillElement(i)->GainXP(pCharacter->GetCurrentSkill()->GetLevel(), GlobalHittedCount, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
			if ((xp > 0) && (pMiniEvents != NULL))
				pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
		}
		xp = MagicSkill.GainXP(pCharacter->GetCurrentSkill()->GetLevel(), GlobalHittedCount, MAX_CHAR_MAGIC_LEVEL, pMsgReporter);
		if ((xp > 0) && (pMiniEvents != NULL))
			pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
	}
	
	GlobalHitCount = GlobalHittedCount = 0;
}

void Player::Reborn(bool start, MsgReporter *pMsgReporter)
{
	int i;

	Humanized::Reborn(start, pMsgReporter);

	for (i = 0; i < SkillPack.GetElementCount(); i++)
	{
		SkillPack.getCharSkillElement(i)->SetXP(0);
		SkillPack.getCharSkillElement(i)->SetSuccessCount(0);
		if (Status.dead)
			SkillPack.getCharSkillElement(i)->GeneratePenality(pMsgReporter);
	}

	for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
	{
		CombatSkillPack.getCharSkillElement(i)->SetXP(0);
		CombatSkillPack.getCharSkillElement(i)->SetSuccessCount(0);
		if (Status.dead)
			CombatSkillPack.getCharSkillElement(i)->GeneratePenality(pMsgReporter);
	}

	MagicSkill.SetXP(0);
	MagicSkill.SetSuccessCount(0);

	if (Status.dead)
	{
		MagicSkill.GeneratePenality(pMsgReporter);
		HitPoints->Increase(1, 1, false);
		HitPoints->GenerateBar();
	}

	for (i = 0; i < TOTAL_CHAR_SWATCH; i++)
		Swatch[i] = Clock();

	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
		PotionElement[i] = NULL;

	MobTarget = NULL;
	PnjTarget = NULL;

	Target = this;
	SpellTarget = this;
	HumanizedTarget = NULL;

	GenerateEquipment();

	Status = CharStatus();

	setPlan(1);
}

Mob * Player::GetMobTarget()
{
	if (MobTarget)
		return MobTarget;

	return dynamic_cast<Mob*>(Target);
}

Pnj * Player::GetPnjTarget()
{
	if (PnjTarget)
		return PnjTarget;

	return dynamic_cast<Pnj*>(Target);
}
