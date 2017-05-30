#include <canimation.h>
#include "ccharacter.h"
#include "../RLRes/RLRes/resource.h"

extern DataFileList Rules;

Character::Character()
{
	HPBonus = 10;

	Race = NONE_CHAR_RACE;
	Class = NONE_CHAR_CLASS;

	CurrentSkill = NULL;

	EquipedWp = NULL;

	HitPoints = new EnergyBar(0, NULL, 60, 12, LIFE, NULL, NULL, 0, 16);

	Title = "Aucun";

	Target = AttackTarget = this;

	GoSound = AttackSound = HitSound = SleepSound = NULL;

	DisplayedName = ReplaceCharByString(name, "0123456789", "#");
	DisplayedName = ReplaceStringByAnother(DisplayedName, " #", "");
}

Character::Character(Character &model)
{
	int i;

	HPBonus = model.GetHPBonus();

	Race = model.GetRace();
	Class = model.GetClass();

	Status = *model.GetStatus();

	MagicResistSkill = *model.GetMagicResistSkill();
	CurrentSkill = model.GetCurrentSkill();

	Combat = *model.GetCombat();

	EquipedWp = model.GetEquipedWp();
	CurrentWp = *model.GetCurrentWp();

	HitPoints = new EnergyBar(*model.GetHitPoints());

	Title = *model.GetTitle();

	for (i = 0; i < TOTAL_CHAR_SWATCH; i++)
		Swatch[i] = *model.GetSwatch(i);

	Target = model.GetTarget();
	AttackTarget = model.GetAttackTarget();

	GoSound = model.GetGoSound();
	AttackSound = model.GetAttackSound();
	HitSound = model.GetHitSound();
	SleepSound = model.GetSleepSound();

	StatPack = *model.GetStatPack();
	Stat2Pack = *model.GetStat2Pack();
	CombatSkillPack = *model.GetCombatSkillPack();

	DisplayedName = *model.GetDisplayedName();
}

Character::~Character()
{
	delete HitPoints;
	StatPack.RemoveAllElement(0, true);
	Stat2Pack.RemoveAllElement(0, true);
	CombatSkillPack.RemoveAllElement(0, true);
}

void Character::SetTarget(Character *pCharacter)
{
	Status.attacking = Status.spy = false;

	if (!pCharacter) pCharacter = this;
	Target = pCharacter;

	Target->GetIgnoredBy()->RemoveElement(this);
}

bool Character::Init()
{
	int i;
	CharStat *Stat, *Stat2;
	CharSkill *CombatSkill;
	DataGroup *Element, *InnerElement;
	DataGroupList StatTable, Stat2Table, CombatSkillTable, CombatSkillStatTable;

	if (Rules.GetStatus() != DataFileList::OPENED_READONLY)
		return false;

	if (!Rules.CreateDataGroupList("CharStat", &StatTable))
		return false;

	if (!Rules.CreateDataGroupList("CharStat2", &Stat2Table))
		return false;

	if (!Rules.CreateDataGroupList("CharCombatSkill", &CombatSkillTable))
		return false;

	if (!Rules.CreateDataGroupList("CharCombatSkillStat", &CombatSkillStatTable))
		return false;

	StatPack.RemoveAllElement(0, true);
	StatPack.RemoveAllElement(0, true);
	CombatSkillPack.RemoveAllElement(0, true);

	for (i = 0; i < StatTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &StatTable);
		Stat = new CharStat();
		Stat->SetStatId(Str::ToInt(Element->GetValue("id")));
		Stat->SetDescription(Element->GetValue("description"));
		Stat->SetLabel(Element->GetValue("label"));
		StatPack.AddElement(Stat);
	}

	for (i = 0; i < Stat2Table.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &Stat2Table);
		Stat2 = new CharStat();
		Stat2->SetStatId(Str::ToInt(Element->GetValue("id")));
		Stat2->SetDescription(Element->GetValue("description"));
		Stat2->SetLabel(Element->GetValue("formule"));
		Stat2Pack.AddElement(Stat2);
	}

	for (i = 0; i < CombatSkillTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &CombatSkillTable);
		CombatSkill = new CharSkill();
		CombatSkill->SetSkillId(Str::ToInt(Element->GetValue("id")));
		CombatSkill->SetDifficulty(Str::ToInt(Element->GetValue("difficulty")));
		CombatSkill->SetDescription(Element->GetValue("description"));
		InnerElement = CombatSkillStatTable.Select("ex_CharCombatSkill", CombatSkill->GetSkillId());
		while (InnerElement)
		{
			CombatSkill->GetStatPack()->AddElement(StatPack.GetStat(Str::ToInt(InnerElement->GetValue("ex_CharStat"))));
			InnerElement = CombatSkillStatTable.NextSelect();
		}
		CombatSkillPack.AddElement(CombatSkill);
	}

	return true;
}

void Character::InitOrientation()
{
	int random;

	if ((Class == NONE_CHAR_CLASS) | (Class == PNJ1))
		angle = PI / 2;
	else
	{
		random = Clock::random(1, 4);
		switch (random)
		{
		case 1:
			angle = PI;
			break;
		case 2:
			angle = 0;
			break;
		case 3:
			angle = 3 * PI / 2;
			break;
		default:
			angle = PI / 2;
			break;
		}
	}
}

void Character::initAni()
{
	HitPoints->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	HitPoints->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
}

void Character::restoreAni()
{
	Visual::restoreAni();
	HitPoints->restoreAni();
	HitPoints->GetBar()->restoreAni();
}

void Character::destroyAni()
{
	Visual::destroyAni();
	HitPoints->destroyAni();
	HitPoints->GetBar()->destroyAni();
}

void Character::GenerateDisplacement(TileEngine *pTileEngine, CardinalPoint *MaxDistance, CardinalPoint Obstacle[], int ObstacleCount, bool stop, bool rebound)
{
	int i;

	if (pTileEngine)
		pTileEngine->Scroll(this, MaxDistance, stop, rebound);
	else
		Move(MaxDistance, stop, rebound);

	GenerateAniRotation();

	if (ObstacleCount)
		for (i = 0; i < ObstacleCount; i++)
			GenerateCollision(&Obstacle[i], Obstacle, ObstacleCount, stop, rebound);
}

void Character::GenerateStatus(short *MeditateProgress, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	if ((Status.fight == false) && (Status.attack == false) && (Status.concentrate == false) && (Status.sleep == false) && (Status.dead == false))
		Status.access = true;

	if (Status.sleep)
	{
		Recover(pMsgReporter);
		if (HitPoints->GetAmount() == HitPoints->GetMaxAmount())
		{
			if (Race == ANIMAL)
				Status.sleep = false;
			if (!Status.sleep)
			{
				if (pMsgReporter)
				{
					pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s récupère %.2f points de vie", GetDisplayedName()->Get(), CharEvent.score), ACTION_MSG_COLOR);
					if (Race != ANIMAL)
						pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s récupère %.2f points de mana", GetDisplayedName()->Get(), CharEvent.RealScore), ACTION_MSG_COLOR);
				}
			}
		}
	}
}

void Character::GenerateAni(List *pSoundList)
{
	int i;

	//Définit l'animation courante par rapport aux événements
	if (magnitude)
		currentAni = MOVE;
	else
		currentAni = STOP;

	if (Status.attack)
		switch (CurrentWp.GetCombatSkill())
		{
		case DAGGERS:
			currentAni = ATTACK_DAGGER;
			HitSound = (Sound*)pSoundList->GetElement(HUMAN_SCREAM);
			break;
		case SWORDS:
			currentAni = ATTACK_SWORD;
			HitSound = (Sound*)pSoundList->GetElement(HUMAN_SCREAM);
			break;
		case MACES:
			currentAni = ATTACK_MACE;
			HitSound = (Sound*)pSoundList->GetElement(HUMAN_HIT_MACE);
			break;
		case AXES:
			currentAni = ATTACK_AXE;
			HitSound = (Sound*)pSoundList->GetElement(HUMAN_HIT_AXE);
			break;
		default:
			currentAni = ATTACK_HTH;
			if ((Race == NONE_CHAR_RACE) | (Race == HUMANIZED))
				HitSound = (Sound*)pSoundList->GetElement(HUMAN_HIT_HTH);
			break;
		}

	if (Status.concentrate)
		currentAni = CONCENTRATE;

	if (Status.hitted)
	{
		Status.sleep = false;
		currentAni = HITTED;
	}

	if (Status.sleep)
		currentAni = SLEEP;

	if (Status.dead)
		currentAni = DEAD;
	else
		Animation::getAnimationElement(DEAD, &animations)->GetFluidity()->setCounter(0);

	//Redimensionne la zone à afficher par rapport à l'animation courante
	setVisibleZone((int)(position.x + Animation::getAnimationElement(currentAni, &animations)->GetOriginPoint()->x), (int)(position.x + Animation::getAnimationElement(currentAni, &animations)->GetFrameDimension()->x + Animation::getAnimationElement(currentAni, &animations)->GetOriginPoint()->x), (int)(position.y + Animation::getAnimationElement(currentAni, &animations)->GetOriginPoint()->y), (int)(position.y + Animation::getAnimationElement(currentAni, &animations)->GetFrameDimension()->y + Animation::getAnimationElement(currentAni, &animations)->GetOriginPoint()->y));

	//Diffusement de l'animation courante
	if (Animation::getAnimationElement(DEAD, &animations)->GetFluidity()->getCounter() == 3)
	{
		for (i = DEAD; i < animations.GetElementCount(); i+=TOTAL_CHAR_ANI)
			Animation::getAnimationElement(i, &animations)->GetFluidity()->setCounter(3);
		runAni(DEAD, 3);
	}
	else
		runAni(currentAni, (double)12);

	//Crée des événements à partir de l'état d'une animation
	if (Status.attack)
		switch (Animation::getAnimationElement(currentAni, &animations)->GetFluidity()->getCounter())
		{
		case 0:
			if (!Animation::getAnimationElement(currentAni, &animations)->GetFrameHit(0))
				Status.attack = false;
			break;
		case 1:
			if (Animation::getAnimationElement(currentAni, &animations)->GetFrameHit(1) == 0)
				PlaySound(AttackSound, 0, false);
			break;
		case 3:
			if ((AttackTarget != this) && (GetAdjacent(AttackTarget) == true))
				if (!Animation::getAnimationElement(currentAni, &animations)->GetFrameHit(3))
					Status.hit = true;
				else
					Status.hit = false;
			break;
		}

	if (Status.hitted)
		if (Animation::getAnimationElement(currentAni, &animations)->GetFluidity()->getCounter() == 0)
			Status.hitted = false;
}

void Character::GenerateAniRotation()
{
	if ((Direction.west == 1) && (Direction.north == 0) && (Direction.south == 0)) rotateAni(180);
	if ((Direction.east == 1) && (Direction.north == 0) && (Direction.south == 0)) rotateAni(0);
	if ((Direction.north == 1) && (Direction.west == 0) && (Direction.east == 0)) rotateAni(90);
	if ((Direction.south == 1) && (Direction.west == 0) && (Direction.east == 0)) rotateAni(270);
}

void Character::GenerateStat2()
{
	int i;

	Stat2Pack.GetStat(HIT_POINTS)->SetScore(StatPack.GetStat(STRENGTH)->GetScore() + StatPack.GetStat(HEALTH)->GetScore() * 2 + HPBonus);
	Stat2Pack.GetStat(MAGICAL_AFFINITY)->SetScore((StatPack.GetStat(INTELLIGENCE)->GetScore() + StatPack.GetStat(WILLPOWER)->GetScore()) / 2);
	Stat2Pack.GetStat(COMBAT_SPEED)->SetScore((StatPack.GetStat(AGILITY)->GetScore() + StatPack.GetStat(PERCEPTION)->GetScore()) / 2);

	HitPoints->SetMaxAmount((float)Stat2Pack.GetStat(HIT_POINTS)->GetScore());
	HitPoints->GenerateBar();

	for (i = 0; i < Stat2Pack.GetElementCount(); i++)
		Stat2Pack.getCharStatElement(i)->SetModifier();
}

EVENT_TYPE Character::GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i;
	Str Result;

	for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
		if (CombatSkillPack.getCharSkillElement(i)->GetUsed())
		{
			CombatSkillPack.getCharSkillElement(i)->GenerateResult(ORCData);
			switch (CombatSkillPack.getCharSkillElement(i)->GetResult())
			{
			case CRITICAL_SUCCESS:
				Result = BuildString("%s réussit brillamment son attaque", GetDisplayedName()->Get());
				if (pMiniEvents)
					if (CombatSkillPack.getCharSkillElement(i)->GetSkillId() != DAGGERS)
						pMiniEvents->ReleaseEvent(position.x, position.y, "Bonus!", SKILL_RESULT_MSG_COLOR);
				break;
			case SUCCESS:
				Result = BuildString("%s réussit son attaque", GetDisplayedName()->Get());
				break;
			case FAILURE:
				Result = BuildString("%s rate son attaque", GetDisplayedName()->Get());
				if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Raté", SKILL_RESULT_MSG_COLOR);
				break;
			case CRITICAL_FAILURE:
				Result = BuildString("%s rate lamentablement son attaque", GetDisplayedName()->Get());
				if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Gros raté!", SKILL_RESULT_MSG_COLOR);
				break;
			}
			if (Race != ANIMAL)
				Result = BuildString("%s de %s", Result.Get(), CombatSkillPack.getCharSkillElement(i)->GetDescription().Get());
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, Result, SKILL_RESULT_MSG_COLOR);
		}

	if (MagicResistSkill.GetUsed())
	{
		MagicResistSkill.GenerateResult(ORCData);
		switch (MagicResistSkill.GetResult())
		{
		case CRITICAL_SUCCESS:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Sort resisté", SKILL_RESULT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s résiste bien au sort magique!", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		case SUCCESS:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Sort resisté", SKILL_RESULT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s résiste au sort magique", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		}
	}

	return NONE_EVENT_TYPE;
}

EVENT_TYPE Character::CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	EventData2 ActionData;

	if (Status.Action == ATTACK)
	{
		if (CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->GetUsed())
			CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->SetUsed(false);
		//Vérifie si l'action est valable.
		if ((CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->GetResult() >= FAILURE) | (AttackTarget == this) | (GetAdjacent(AttackTarget) == false) | (AttackTarget->GetStatus()->dead == true))
			Status.hit = false;
	}
	else
	{
		Status.hit = false;
		CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->SetUsed(false);
	}

	if (Status.hit)
	{
		Status.hit = false;
		PlaySound(HitSound, 0, false);
		//Offre un avantage de l'arme pour un coup critique.
		if (CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->GetResult() == CRITICAL_SUCCESS)
		{
			if (CurrentWp.GetCombatSkill() == SWORDS)
			{
				CurrentWp.SetMinDamage(CurrentWp.GetMinDamage() * 2);
				CurrentWp.SetMaxDamage(CurrentWp.GetMaxDamage() * 2);
			}
			if ((CurrentWp.GetCombatSkill() == HAND_TO_HAND) | (CurrentWp.GetCombatSkill() == MACES))
				CurrentWp.SetStunning(true);
			if (CurrentWp.GetCombatSkill() == AXES)
				CurrentWp.SetTranspiercing(true);
		}
		if ((AttackTarget->GetStatus()->reflecting == false) | (GetEnchant(MAGIC_TRANSPERCOR) != -1))
		{
			ActionData = Hit(&CurrentWp, pMsgReporter);
			if (pMiniEvents) pMiniEvents->ReleaseEvent(AttackTarget->getPosition()->x, AttackTarget->getPosition()->y, BuildString("-%.2f vie", ActionData.RealScore), DAMAGE_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige %.2f (%.2f) points de dégats à %s", GetDisplayedName()->Get(), ActionData.RealScore, ActionData.score, AttackTarget->GetDisplayedName()->Get()), DAMAGE_MSG_COLOR);
		}
		else
		{
			AttackTarget = this;
			ActionData = Hit(&CurrentWp, pMsgReporter);
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("-%.2f vie", ActionData.RealScore), DAMAGE_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige %.2f (%.2f) points de dégats à lui-même", GetDisplayedName()->Get(), ActionData.RealScore, ActionData.score), DAMAGE_MSG_COLOR);
		}
	}

	return NONE_EVENT_TYPE;
}

void Character::MagicSavingRoll(MagicSpell *pMagicSpell)
{
	MagicResistSkill.SetUsed(true);

	//Calcul le SRS pour la résistance à un sort.
	//Ref: "The Freestyle RPG Basic Rules", page 24.

	MagicResistSkill.SetSRS(MagicResistSkill.GetLevel() + Stat2Pack.GetStat(MAGICAL_AFFINITY)->GetModifier() - pMagicSpell->GetLevel() - pMagicSpell->GetDifficulty());
}

bool Character::UpdateCombat(CharCombat CombatData[])
{
	int i, j;

	//Définit le nombres d'actions par phase pour les combats.
	//Ref: "The Freestyle RPG Basic Rules", page 17.
	for (i = 0; i < TOTAL_COMBAT_RECORD; i++)
		if (CombatData[i].MinCS <= Stat2Pack.GetStat(COMBAT_SPEED)->GetScore() && CombatData[i].MaxCS >= Stat2Pack.GetStat(COMBAT_SPEED)->GetScore())
		{
			Combat.MinCS = CombatData[i].MinCS;
			Combat.MaxCS = CombatData[i].MaxCS;
			Combat.MaxDistance = CombatData[i].MaxDistance;
			for (j = 0; j < TOTAL_COMBAT_PHASE; j++)
				Combat.action[j] = CombatData[i].action[j];
			return true;
		}

	return false;
}

void Character::EngageCombat(MsgReporter *pMsgReporter)
{
	Status.fight = true;
	Status.access = Status.turn = Status.sleep = Status.invisible = false;
	if ((Status.Action != NONE_CHAR_ACTION) && (Status.Action != ATTACK) && (Status.Action != CAST_SPELL))
		Status.Action = NONE_CHAR_ACTION;

	StopSound(SleepSound);
	PlaySound(GoSound, 0, false);

	pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s engage le combat", GetDisplayedName()->Get()), ACTION_MSG_COLOR);
}

void Character::ExitCombat(MsgReporter *pMsgReporter)
{
	Status.fight = Status.attacking = false;
	Status.Action = NONE_CHAR_ACTION;

	if (!Status.dead)
	{
		setPlan(2);
		Status.access = Status.turn = true;
	}

	Target = this;

	AttackedBy.RemoveAllElement();
	pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s quitte le combat", GetDisplayedName()->Get()), ACTION_MSG_COLOR);
}

void Character::WinCombat(Object *Corpse, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Target = this;
	FreeAdjNode(Corpse->getId());
}

bool Character::Attack()
{
	if (EquipedWp)
		CurrentWp = Weapon(*EquipedWp);
	else
		CurrentWp = Weapon();

	if ((Status.attack == false) && (Status.hitted == false) && (CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->GetLevel() > 0))
	{
		Status.attacking = Status.attack = true;
		Status.spy = false;
		Status.Action = ATTACK;
		AttackTarget = Target;
		CurrentSkill = CombatSkillPack.GetSkill(CurrentWp.GetCombatSkill())->Use();
		return true;
	}

	return false;
}

EventData2 Character::Hit(Weapon *pWeapon, MsgReporter *pMsgReporter)
{
	float DiceScore;
	EventData2 HitData;

	DiceScore = (float)Clock::random(pWeapon->GetMinDamage(), pWeapon->GetMaxDamage());
	HitData.score = HitData.RealScore = DiceScore;

	if (pWeapon->GetStunning())
		AttackTarget->GetStatus()->WpStunned = true;
	AttackTarget->GetStatus()->hitted = true;
	AttackTarget->GetHitPoints()->Decrease(DiceScore, 1, false);
	AttackTarget->GetHitPoints()->GenerateBar();

	return HitData;
}

bool Character::Wait(int index, int time, int TimeSpeed)
{
	Swatch[index].goCounter(TimeSpeed, 0, time, false);
	
	return Swatch[index].isGoing();
}

void Character::Sleep()
{
	if ((Status.sleep == false) && (Status.fight == false) && (Status.dead == false))
	{
		Status.access = Status.attacking = false;
		Status.sleep = true;
		CharEvent = EventData2();
	}
}

void Character::Recover(MsgReporter *pMsgReporter)
{
	float RecoverPoints = (float)StatPack.GetStat(HEALTH)->GetScore() / 3 + 0.5f;

	if (HitPoints->GetAmount() < HitPoints->GetMaxAmount())
	{
		RecoverPoints /= 50;
		HitPoints->Increase(RecoverPoints, 1, false);
		HitPoints->GenerateBar();
		CharEvent.score += RecoverPoints;
	}
}

void Character::Diiiiiie(MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i;

	Status.dead = true;
	Status.access = Status.fight = false;
	Target = this;
	setPlan(3);

	Mover::StopMove();
	FreeAllAdjNodes();

	for (i = 0; i < AttackedBy.GetElementCount(); i++)
		Character::getCharacterElement(i, &AttackedBy)->WinCombat(this, pMsgReporter, pMiniEvents);

	if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s est mort", GetDisplayedName()->Get()), ACTION_MSG_COLOR);
}

void Character::Reborn(bool start, MsgReporter *pMsgReporter)
{
	int i;

	if (start)
	{
		Status = CharStatus();
		for (i = 0; i < CombatSkillPack.GetElementCount(); i++)
			*CombatSkillPack.getCharSkillElement(i)->GetBPData() = BonusPoints();
		CurrentSkill = CombatSkillPack.GetSkill(HAND_TO_HAND);
		Combat = CharCombat();
		if (Race != ANIMAL)
			CurrentWp = Weapon();
	}

	for (i = 0; i < TOTAL_CHAR_SWATCH; i++)
		Swatch[i] = Clock();

	Target = this;

	Mover::StopMove();

	for (i = 0; i < animations.GetElementCount(); i++)
		*Animation::getAnimationElement(i, &animations)->GetFluidity() = Clock();

	Start = PathNode();

	for (i = 0; i < 8; i++)
		AdjNode[i].ActorID = -1;

	AttackedBy.RemoveAllElement();
	IgnoredBy.RemoveAllElement();
}

void Character::PlaySound(Sound *pSound, DWORD repeats, bool RepeatOnce)
{
	if (pSound)
	{
		if (repeats)
			Status.PlayingRepeatSound = true;

		if ((Status.playingSound == false) && (pSound->isPlaying() == false) | (repeats == 0))
		{
			Status.playingSound = true;
			pSound->setRepeats(repeats);
			pSound->play(DMUS_SEGF_SECONDARY);
		}

		if (!repeats && !RepeatOnce)
			Status.playingSound = false;
	}
}

void Character::StopSound(Sound *pSound, bool OnlyIfRepeats)
{
	if (pSound)
		if ((OnlyIfRepeats == false) | (pSound->getRepeats() > 0))
		{
			Status.playingSound = Status.PlayingRepeatSound = false;
			pSound->stop();
		}
}

int Character::GetEnchant(PICKUP_ENCHANT EnchantEffect)
{
	int i;

	for (i = 0; i < EnchantPack.GetElementCount(); i++)
		if (Enchant::getEnchantElement(i, &EnchantPack)->GetEffect() == EnchantEffect)
			return i;

	return -1;
}
