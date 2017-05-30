#include <canimation.h>
#include "chumanized.h"
#include "../RLRes/RLRes/resource.h"

extern List SoundPack;
extern Pickup *MagicCircle;
extern DataFileList Rules;

Humanized::Humanized()
{
	int i;

	purse = PotionEffectDuration = MagicPoints = 0;

	MagicSkill.SetDifficulty(4);
	MagicResistSkill.SetDifficulty(4);
	MagicSkill.SetDescription("Magie");

	ManaPoints = new EnergyBar(0, NULL, 60, 12, MANA, NULL, NULL, 0, 16);
	ChargePoints = new EnergyBar(0, NULL, 60, 12, CHARGE, NULL, NULL, 0, 16);

	EquipedArmor = NULL;
	Stealing = NULL;

	MemorizedSpell = NULL;

	SpellTarget = this;
	HumanizedTarget = NULL;
	TileIATarget = NULL;

	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		Pocket[i].SetOrder(PickupList::ORDER_BY_PTYPE);
}

Humanized::Humanized(Humanized &model)
{
	int i;

	purse = model.GetPurse();
	PotionEffectDuration = model.GetPotionEffectDuration();
	MagicPoints = model.GetMagicPoints();

	ManaPoints = new EnergyBar(*model.GetManaPoints());
	ChargePoints = new EnergyBar(*model.GetChargePoints());

	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		Pocket[i] = *model.GetPocket(i);
	EquipedArmor = model.GetEquipedArmor();
	Stealing = model.GetStealing();

	MemorizedSpell = model.GetMemorizedSpell();
	CurrentSpell = *model.GetCurrentSpell();

	SpellTarget = model.GetSpellTarget();
	HumanizedTarget = model.GetHumanizedTarget();
	TileIATarget = model.GetTileIATarget();

	Start = *model.GetStart();

	MagicSchoolPack = *model.GetMagicSchoolPack();
	MagicSkill = *model.GetMagicSkill();
	SkillPack = *model.GetSkillPack();

	for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
		MagicSpellPack[i] = *model.GetMagicSpellPack(i);
}

Humanized::~Humanized()
{
	delete [] MagicSpellPack;
	delete ManaPoints;
	delete ChargePoints;
	MagicSchoolPack.RemoveAllElement(0, true);
	SkillPack.RemoveAllElement(0, true);
}

void Humanized::SetPrimaryBehavior()
{
	switch (PrimaryBehavior)
	{
	case AGGRO:
		Behavior.thief = false;
		if (Behavior.CurseInfluenced)
			Behavior.aggro = Status.cursed;
		else
			Behavior.aggro = true;
		break;
	case THIEF:
		Behavior.aggro = false;
		if (Behavior.CurseInfluenced)
			Behavior.thief = Status.cursed;
		else
			Behavior.thief = true;
		break;
	default:
		Behavior.thief = false;
		if (Behavior.CurseInfluenced)
			Behavior.aggro = Status.cursed;
		else
			Behavior.aggro = false;
		break;
	}
}

void Humanized::SetMemorizedSpell(MagicSpell *pMagicSpell)
{
	if (MemorizedSpell != pMagicSpell)
	{
		PlaySound(Sound::getSoundElement(SPELL_MEMORIZED, &SoundPack), 0, false);
		MemorizedSpell = pMagicSpell;
	}
}

void Humanized::SetSpellTarget(Character *pCharacter)
{
	Status.attacking = false;

	SpellTarget = pCharacter;
}

void Humanized::SetTarget(Character *pCharacter)
{
	Character::SetTarget(pCharacter);

	HumanizedTarget = dynamic_cast<Humanized*>(pCharacter);
	TileIATarget = dynamic_cast<TileIA*>(pCharacter);
}

bool Humanized::Init()
{
	int i;
	CharStat *MagicSchool;
	CharSkill *Skill;
	DataGroup *Element, *InnerElement;
	DataGroupList MagicSchoolTable, StatTable, SkillTable, SkillStatTable;

	if (!Character::Init())
		return false;

	if (!Rules.CreateDataGroupList("CharMagicSchool", &MagicSchoolTable))
		return false;

	if (!Rules.CreateDataGroupList("CharStat", &StatTable))
		return false;

	if (!Rules.CreateDataGroupList("CharSkill", &SkillTable))
		return false;

	if (!Rules.CreateDataGroupList("CharSkillStat", &SkillStatTable))
		return false;

	MagicSchoolPack.RemoveAllElement(0, true);
	SkillPack.RemoveAllElement(0, true);

	for (i = 0; i < MagicSchoolTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &MagicSchoolTable);
		MagicSchool = new CharStat();
		MagicSchool->SetStatId(Str::ToInt(Element->GetValue("id")));
		MagicSchool->SetDescription(Element->GetValue("description"));
		MagicSchool->SetLabel(Element->GetValue("label"));
		MagicSchoolPack.AddElement(MagicSchool);
	}

	for (i = 0; i < SkillTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &SkillTable);
		Skill = new CharSkill();
		Skill->SetSkillId(Str::ToInt(Element->GetValue("id")));
		Skill->SetDifficulty(Str::ToInt(Element->GetValue("difficulty")));
		Skill->SetDescription(Element->GetValue("description"));
		InnerElement = SkillStatTable.Select("ex_CharSkill", Skill->GetSkillId());
		while (InnerElement)
		{
			Skill->GetStatPack()->AddElement(StatPack.GetStat(Str::ToInt(InnerElement->GetValue("ex_CharStat"))));
			InnerElement = SkillStatTable.NextSelect();
		}
		SkillPack.AddElement(Skill);
	}

	MagicSpellPack = new MagicSpellList[MagicSchoolPack.GetElementCount()];

	for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
		MagicSkill.GetStatPack()->AddElement(MagicSchoolPack.getCharStatElement(i));

	return true;
}

void Humanized::initAni()
{
	Character::initAni();
	animations.RemoveAllElement(0, true);
	animations.Initialize(TOTAL_CHAR_ANI);

	if (!IsAPlant())
	{
		addAni(STOP, IDB_HU_NUDE_STOP, "HU_NUDE", 1, 27, 19, RGB(255, 255, 255), 270);
		addAni(MOVE, IDB_HU_NUDE_MOVE, "HU_NUDE", 8, 28, 24, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(MOVE, &animations)->SetOriginPoint(0, 0, -2);
		Animation::getAnimationElement(MOVE, &animations)->SetOriginPoint(1, -2, -1);
		Animation::getAnimationElement(MOVE, &animations)->SetOriginPoint(2, -1, -3);
		Animation::getAnimationElement(MOVE, &animations)->SetOriginPoint(3, -3, 0);

		addAni(HITTED, IDB_HU_NUDE_HITTED, "HU_NUDE", 6, 29, 25, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(HITTED, &animations)->SetOriginPoint(0, -1, -5);
		Animation::getAnimationElement(HITTED, &animations)->SetOriginPoint(1, -5, -1);

		addAni(SLEEP, IDB_HU_NUDE_SLEEP, "HU_NUDE", 1, 18, 32, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(SLEEP, &animations)->SetOriginPoint(0, 5, 0);
		Animation::getAnimationElement(SLEEP, &animations)->SetOriginPoint(1, 0, 3);
		Animation::getAnimationElement(SLEEP, &animations)->SetOriginPoint(2, 3, 0);
		Animation::getAnimationElement(SLEEP, &animations)->SetOriginPoint(3, 0, 5);

		addAni(DEAD, IDB_HU_NUDE_DEAD, "HU_NUDE", 4, 30, 32, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(DEAD, &animations)->SetOriginPoint(2, 0, -10);
		Animation::getAnimationElement(DEAD, &animations)->SetOriginPoint(3, -10, 0);

		addAni(ATTACK_HTH, IDB_HU_NUDE_ATTACK_HTH, "HU_NUDE", 8, 27, 21, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(ATTACK_HTH, &animations)->SetOriginPoint(2, 0, -2);
		Animation::getAnimationElement(ATTACK_HTH, &animations)->SetOriginPoint(3, -2, 0);

		addAni(ATTACK_DAGGER, IDB_HU_NUDE_ATTACK_DAGGER, "HU_NUDE", 7, 29, 33, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(ATTACK_DAGGER, &animations)->SetOriginPoint(1, 0, -2);
		Animation::getAnimationElement(ATTACK_DAGGER, &animations)->SetOriginPoint(2, -2, -14);
		Animation::getAnimationElement(ATTACK_DAGGER, &animations)->SetOriginPoint(3, -14, 0);

		addAni(ATTACK_MACE, IDB_HU_NUDE_ATTACK_MACE, "HU_NUDE", 8, 31, 32, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(ATTACK_MACE, &animations)->SetOriginPoint(0, -1, 0);
		Animation::getAnimationElement(ATTACK_MACE, &animations)->SetOriginPoint(1, 0, -3);
		Animation::getAnimationElement(ATTACK_MACE, &animations)->SetOriginPoint(2, -3, -13);
		Animation::getAnimationElement(ATTACK_MACE, &animations)->SetOriginPoint(3, -13, -1);

		addAni(ATTACK_SWORD, IDB_HU_NUDE_ATTACK_SWORD, "HU_NUDE", 7, 29, 43, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(ATTACK_SWORD, &animations)->SetOriginPoint(1, 0, -2);
		Animation::getAnimationElement(ATTACK_SWORD, &animations)->SetOriginPoint(2, -2, -24);
		Animation::getAnimationElement(ATTACK_SWORD, &animations)->SetOriginPoint(3, -24, 0);

		addAni(ATTACK_AXE, IDB_HU_NUDE_ATTACK_AXE, "HU_NUDE", 8, 51, 34, RGB(255, 255, 255), 270);
		Animation::getAnimationElement(ATTACK_AXE, &animations)->SetOriginPoint(0, -12, -2);
		Animation::getAnimationElement(ATTACK_AXE, &animations)->SetOriginPoint(1, -2, -12);
		Animation::getAnimationElement(ATTACK_AXE, &animations)->SetOriginPoint(2, -12, -13);
		Animation::getAnimationElement(ATTACK_AXE, &animations)->SetOriginPoint(3, -13, -12);

		addAni(CONCENTRATE, IDB_HU_NUDE_CONCENTRATE, "HU_NUDE", 2, 27, 18, RGB(255, 255, 255), 270);

		MapSkins();
	}
	else
	{
		addAni(STOP, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(MOVE, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(HITTED, IDB_MARSHPLANT_HITTED, "", 6, 30, 28, RGB(255, 255, 255), 0);
		addAni(SLEEP, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(DEAD, IDB_MARSHPLANT_DEAD, "", 4, 30, 28, RGB(255, 255, 255), 0);
		addAni(ATTACK_HTH, IDB_MARSHPLANT_ATTACK_HTH, "", 7, 30, 28, RGB(255, 255, 255), 0);
		addAni(ATTACK_DAGGER, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(ATTACK_MACE, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(ATTACK_SWORD, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(ATTACK_AXE, IDB_MARSHPLANT_STOP, "", 1, 30, 28, RGB(255, 255, 255), 0);
		addAni(CONCENTRATE, IDB_MARSHPLANT_CONCENTRATE, "", 3, 30, 28, RGB(255, 255, 255), 0);
	}

	ManaPoints->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	ManaPoints->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	ChargePoints->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	ChargePoints->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
}

void Humanized::MapSkins()
{
	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_STOP, "HU_CLOTH"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_SLEEP, "HU_CLOTH"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_MOVE, "HU_CLOTH"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_HITTED, "HU_CLOTH"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_DEAD, "HU_CLOTH"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_CONCENTRATE, "HU_CLOTH"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_ATTACK_SWORD, "HU_CLOTH"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_ATTACK_MACE, "HU_CLOTH"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_ATTACK_HTH, "HU_CLOTH"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_ATTACK_DAGGER, "HU_CLOTH"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_CLOTH_ATTACK_AXE, "HU_CLOTH"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_LEATH_STOP, "HU_LEATH"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_LEATH_SLEEP, "HU_LEATH"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_LEATH_MOVE, "HU_LEATH"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_LEATH_HITTED, "HU_LEATH"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_LEATH_DEAD, "HU_LEATH"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_LEATH_CONCENTRATE, "HU_LEATH"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_LEATH_ATTACK_SWORD, "HU_LEATH"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_LEATH_ATTACK_MACE, "HU_LEATH"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_LEATH_ATTACK_HTH, "HU_LEATH"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_LEATH_ATTACK_DAGGER, "HU_LEATH"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_LEATH_ATTACK_AXE, "HU_LEATH"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_MESH_STOP, "HU_MESH"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_MESH_SLEEP, "HU_MESH"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_MESH_MOVE, "HU_MESH"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_MESH_HITTED, "HU_MESH"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_MESH_DEAD, "HU_MESH"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_MESH_CONCENTRATE, "HU_MESH"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_MESH_ATTACK_SWORD, "HU_MESH"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_MESH_ATTACK_MACE, "HU_MESH"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_MESH_ATTACK_HTH, "HU_MESH"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_MESH_ATTACK_DAGGER, "HU_MESH"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_MESH_ATTACK_AXE, "HU_MESH"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_STOP, "HU_DRESS1"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_SLEEP, "HU_DRESS1"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_MOVE, "HU_DRESS1"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_HITTED, "HU_DRESS1"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_DEAD, "HU_DRESS1"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_CONCENTRATE, "HU_DRESS1"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_ATTACK_SWORD, "HU_DRESS1"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_ATTACK_MACE, "HU_DRESS1"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_ATTACK_HTH, "HU_DRESS1"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_ATTACK_DAGGER, "HU_DRESS1"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_DRESS1_ATTACK_AXE, "HU_DRESS1"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_STOP, "HU_DRESS2"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_SLEEP, "HU_DRESS2"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_MOVE, "HU_DRESS2"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_HITTED, "HU_DRESS2"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_DEAD, "HU_DRESS2"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_CONCENTRATE, "HU_DRESS2"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_ATTACK_SWORD, "HU_DRESS2"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_ATTACK_MACE, "HU_DRESS2"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_ATTACK_HTH, "HU_DRESS2"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_ATTACK_DAGGER, "HU_DRESS2"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_DRESS2_ATTACK_AXE, "HU_DRESS2"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_STOP, "HU_DRESS3"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_SLEEP, "HU_DRESS3"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_MOVE, "HU_DRESS3"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_HITTED, "HU_DRESS3"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_DEAD, "HU_DRESS3"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_CONCENTRATE, "HU_DRESS3"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_ATTACK_SWORD, "HU_DRESS3"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_ATTACK_MACE, "HU_DRESS3"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_ATTACK_HTH, "HU_DRESS3"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_ATTACK_DAGGER, "HU_DRESS3"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_DRESS3_ATTACK_AXE, "HU_DRESS3"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_PADS_STOP, "HU_PADS"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_PADS_SLEEP, "HU_PADS"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_PADS_MOVE, "HU_PADS"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_PADS_HITTED, "HU_PADS"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_PADS_DEAD, "HU_PADS"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_PADS_CONCENTRATE, "HU_PADS"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_PADS_ATTACK_SWORD, "HU_PADS"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_PADS_ATTACK_MACE, "HU_PADS"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_PADS_ATTACK_HTH, "HU_PADS"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_PADS_ATTACK_DAGGER, "HU_PADS"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_PADS_ATTACK_AXE, "HU_PADS"));

	Animation::getAnimationElement(STOP, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_STOP, "HU_ARMOR"));
	Animation::getAnimationElement(SLEEP, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_SLEEP, "HU_ARMOR"));
	Animation::getAnimationElement(MOVE, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_MOVE, "HU_ARMOR"));
	Animation::getAnimationElement(HITTED, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_HITTED, "HU_ARMOR"));
	Animation::getAnimationElement(DEAD, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_DEAD, "HU_ARMOR"));
	Animation::getAnimationElement(CONCENTRATE, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_CONCENTRATE, "HU_ARMOR"));
	Animation::getAnimationElement(ATTACK_SWORD, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_ATTACK_SWORD, "HU_ARMOR"));
	Animation::getAnimationElement(ATTACK_MACE, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_ATTACK_MACE, "HU_ARMOR"));
	Animation::getAnimationElement(ATTACK_HTH, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_ATTACK_HTH, "HU_ARMOR"));
	Animation::getAnimationElement(ATTACK_DAGGER, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_ATTACK_DAGGER, "HU_ARMOR"));
	Animation::getAnimationElement(ATTACK_AXE, &animations)->MapSkin(new Integer(IDB_HU_ARMOR_ATTACK_AXE, "HU_ARMOR"));
}

void Humanized::InitSound(List *pSoundList)
{
	AttackSound = (Sound*)pSoundList->GetElement(HUMAN_ATTACK);
	SleepSound = (Sound*)pSoundList->GetElement(HUMAN_SLEEP);
}

void Humanized::restoreAni()
{
	Character::restoreAni();

	ManaPoints->restoreAni();
	ManaPoints->GetBar()->restoreAni();
	ChargePoints->restoreAni();
	ChargePoints->GetBar()->restoreAni();
}

void Humanized::destroyAni()
{
	Character::destroyAni();

	ManaPoints->destroyAni();
	ManaPoints->GetBar()->destroyAni();
	ChargePoints->destroyAni();
	ChargePoints->GetBar()->destroyAni();
}

void Humanized::GenerateStatus(short *MeditateProgress, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int xp;
	float DiceScore;

	if ((Status.fight == false) && (Status.attack == false) && (Status.concentrate == false) && (Status.sleep == false) && (Status.dead == false))
		Status.access = true;

	if (Status.concentrate)
		CastSpell();

	if (Status.invisible)
	{
		if (!Wait(POTION_EFFECT, PotionEffectDuration, 1))
			Status.invisible = false;
	}

	if (EquipedArmor) EquipedArmor->GenerateSuccessRate(SkillPack.GetSkill(EquipedArmor->GetArmorSkill())->GetLevel());

	if (Status.sleep)
	{
		PlaySound(SleepSound, DMUS_SEG_REPEAT_INFINITE, false);
		Recover(pMsgReporter);
		Meditate();
		if (HitPoints->GetAmount() == HitPoints->GetMaxAmount())
		{
			if (ManaPoints->GetAmount() == ManaPoints->GetMaxAmount())
				Status.sleep = false;
			if (!Status.sleep)
			{
				StopSound(SleepSound);
				if (Behavior.progress)
					if (*MeditateProgress)
					{
						*MeditateProgress = 0;
						xp = SkillPack.GetSkill(MEDITATION)->GainXP(NULL, NULL, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
						if ((pMiniEvents != NULL) && (xp > 0))
							pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
					}
					else
						if (pMsgReporter) pMsgReporter->AddMessage(XP_GAIN_DENIED, 1);
				SkillPack.GetSkill(MEDITATION)->SetUsed(false);
				if (pMsgReporter)
				{
					pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s récupère %.2f points de vie", GetDisplayedName()->Get(), CharEvent.score), ACTION_MSG_COLOR);
					if (Race != ANIMAL)
						pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s récupère %.2f points de mana", GetDisplayedName()->Get(), CharEvent.RealScore), ACTION_MSG_COLOR);
				}
			}
		}				
	}

	if (GetEnchant(LIFE_MANA_GENERATOR) != -1)
	{
		DiceScore = (float)Clock::random(EquipedWp->GetEnchant()->GetMinValue(), EquipedWp->GetEnchant()->GetMaxValue());
		if (HitPoints->GetAmount() < HitPoints->GetMaxAmount())
		{
			HitPoints->Increase(DiceScore, 10, true);
			HitPoints->GenerateBar();
		}
		if (ManaPoints->GetAmount() < ManaPoints->GetMaxAmount())
		{
			ManaPoints->Increase(DiceScore, 10, true);
			ManaPoints->GenerateBar();
		}
	}
}

void Humanized::GenerateStat()
{
	int i;

	RemoveAllStatMalus();
	Character::GenerateStat();

	for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
		MagicSchoolPack.getCharStatElement(i)->SetModifier();

	AddAllStatMalus();
}

void Humanized::GenerateStat2()
{
	Stat2Pack.GetStat(MANA_POINTS)->SetScore(StatPack.GetStat(INTELLIGENCE)->GetScore() + StatPack.GetStat(WILLPOWER)->GetScore() * 2 + 10 + 2 * MagicSkill.GetLevel());
	Stat2Pack.GetStat(CHARGE_POINTS)->SetScore(10 * (StatPack.GetStat(STRENGTH)->GetScore() + StatPack.GetStat(HEALTH)->GetScore()));

	ManaPoints->SetMaxAmount((float)Stat2Pack.GetStat(MANA_POINTS)->GetScore());
	ChargePoints->SetMaxAmount((float)Stat2Pack.GetStat(CHARGE_POINTS)->GetScore());

	ManaPoints->GenerateBar();
	ChargePoints->GenerateBar();

	Character::GenerateStat2();
}

EVENT_TYPE Humanized::GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i;

	Character::GenerateSkill(ORCData, pMsgReporter, pMiniEvents);

	for (i = 0; i < SkillPack.GetElementCount(); i++)
		if (SkillPack.getCharSkillElement(i)->GetUsed())
		{
			SkillPack.getCharSkillElement(i)->GenerateResult(ORCData);
			if (SkillPack.getCharSkillElement(i)->GetSkillId() != MEDITATION && SkillPack.getCharSkillElement(i)->GetSkillId() != LIGHT_ARMOR && SkillPack.getCharSkillElement(i)->GetSkillId() != HEAVY_ARMOR)
				switch (SkillPack.getCharSkillElement(i)->GetResult())
				{
				case CRITICAL_SUCCESS:
					if (pMiniEvents)
						if (SkillPack.getCharSkillElement(i)->GetSkillId() == SPYING)
							pMiniEvents->ReleaseEvent(position.x, position.y, "200m", SKILL_RESULT_MSG_COLOR);
						else
							if (SkillPack.getCharSkillElement(i)->GetSkillId() == DISCRETION)
								pMiniEvents->ReleaseEvent(position.x, position.y, "Discret", SKILL_RESULT_MSG_COLOR);
					if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s réussit brillamment son action de %s!", GetDisplayedName()->Get(), SkillPack.getCharSkillElement(i)->GetDescription().Get()), SKILL_RESULT_MSG_COLOR);
					break;
				case SUCCESS:
					if (pMiniEvents)
						if (SkillPack.getCharSkillElement(i)->GetSkillId() == SPYING)
							pMiniEvents->ReleaseEvent(position.x, position.y, "100m", SKILL_RESULT_MSG_COLOR);
						else
							if (SkillPack.getCharSkillElement(i)->GetSkillId() == DISCRETION)
								pMiniEvents->ReleaseEvent(position.x, position.y, "Discret", SKILL_RESULT_MSG_COLOR);
					if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s réussit son action de %s", GetDisplayedName()->Get(), SkillPack.getCharSkillElement(i)->GetDescription().Get()), SKILL_RESULT_MSG_COLOR);
					break;
				case FAILURE:
					if (pMiniEvents)
						if (SkillPack.getCharSkillElement(i)->GetSkillId() == DISCRETION)
							pMiniEvents->ReleaseEvent(position.x, position.y, "Pas discret", SKILL_RESULT_MSG_COLOR);
						else
							pMiniEvents->ReleaseEvent(position.x, position.y, "Raté", SKILL_RESULT_MSG_COLOR);
					if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s rate son action de %s", GetDisplayedName()->Get(), SkillPack.getCharSkillElement(i)->GetDescription().Get()), SKILL_RESULT_MSG_COLOR);
					break;
				case CRITICAL_FAILURE:
					if (pMiniEvents)
						if (SkillPack.getCharSkillElement(i)->GetSkillId() == DISCRETION)
							pMiniEvents->ReleaseEvent(position.x, position.y, "Pas discret", SKILL_RESULT_MSG_COLOR);
						else
							pMiniEvents->ReleaseEvent(position.x, position.y, "Gros raté!", SKILL_RESULT_MSG_COLOR);
					if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s rate lamentablement son action de %s", GetDisplayedName()->Get(), SkillPack.getCharSkillElement(i)->GetDescription().Get()), SKILL_RESULT_MSG_COLOR);
					break;
				}
		}

	if (MagicSkill.GetUsed())
	{
		MagicSkill.GenerateResult(ORCData);
		switch (MagicSkill.GetResult())
		{
		case CRITICAL_SUCCESS:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Bonus!", SKILL_RESULT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s réussit brillamment son incantation magique!", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		case SUCCESS:
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s réussit son incantation magique", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		case FAILURE:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Raté", SKILL_RESULT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s rate son incantation magique", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		case CRITICAL_FAILURE:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "Gros raté!", SKILL_RESULT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s rate lamentablement son incantation magique", GetDisplayedName()->Get()), SKILL_RESULT_MSG_COLOR);
			break;
		}
	}

	return NONE_EVENT_TYPE;
}

void Humanized::GenerateTitle()
{
	int MaxLevel, BestSkill;
	CharSkill *pCharSkill = NULL;

	MaxLevel = MAX_CHAR_SKILL_LEVEL;
	BestSkill = 0;

	if ((Behavior.FavoriteSkill1 != NONE_CHAR_SKILL) && (Behavior.FavoriteCombatSkill != NONE_CHAR_COMBAT_SKILL))
	{
		if (SkillPack.GetSkill(Behavior.FavoriteSkill1)->GetLevel() > CombatSkillPack.GetSkill(Behavior.FavoriteCombatSkill)->GetLevel())
			pCharSkill = SkillPack.GetSkill(Behavior.FavoriteSkill1);
		else
			pCharSkill = CombatSkillPack.GetSkill(Behavior.FavoriteCombatSkill);
	}
	else
	{
		pCharSkill = SkillPack.GetBestSkill();
		if (CombatSkillPack.GetBestCombatSkill()->GetLevel() > pCharSkill->GetLevel())
			pCharSkill = CombatSkillPack.GetBestCombatSkill();
	}

	if (MagicSkill.GetLevel() > pCharSkill->GetLevel())
	{
		MaxLevel = MAX_CHAR_MAGIC_LEVEL;
		pCharSkill = &MagicSkill;
	}

	if (pCharSkill->GetLevel() > 1)
	{
		if (pCharSkill->GetLevel() >= 2) Title = BuildString("Débutant en %s", pCharSkill->GetDescription().Get());
		if (pCharSkill->GetLevel() >= 6) Title = BuildString("Expert en %s", pCharSkill->GetDescription().Get());
		if (pCharSkill->GetLevel() == MaxLevel) Title = BuildString("Maître en %s", pCharSkill->GetDescription().Get());
		if (pCharSkill == &MagicSkill) Title = BuildString("%s de %s", Title.Get(), MagicSchoolPack.GetBestStat()->GetDescription().Get());
	}
	else
		Title = "Aucun";
}

void Humanized::GenerateEquipment()
{
	int i, j;

	purse = 0;
	ChargePoints->SetAmount(0);
	PotionGift.RemoveAllElement();

	//Refait l'inventaire et recalcule la bourse et la charge.
	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		for (j = 0; j < Pocket[i].GetElementCount(); j++)
		{
			switch (Pickup::getPickupElement(j, &Pocket[i])->GetType())
			{
			case MONEY:
				if (Pickup::getPickupElement(j, &Pocket[i])->GetType() == MONEY)
					purse += Pickup::getPickupElement(j, &Pocket[i])->GetPrice();
				break;
			case POTION:
				if ((Potion::getPotionElement(j, &Pocket[i]) != NULL) && (Potion::getPotionElement(j, &Pocket[i])->GetEffect() == LIFE_REGEN) && (Potion::getPotionElement(j, &Pocket[i])->GetPower() >= POTION_GIFT_POWER) && (PotionGift.GetElementCount() < TOTAL_POTION_GIFT))
					PotionGift.AddElement(Potion::getPotionElement(j, &Pocket[i]));
				break;
			}
			ChargePoints->Increase(Pickup::getPickupElement(j, &Pocket[i])->GetWeight(), 1, false);
		}

	ChargePoints->GenerateBar();
}

EVENT_TYPE Humanized::AddToEquipment(Pickup *pPickup, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	//Ajoute un objet dans l'équipement si il y a la place
	if (Stat2Pack.GetStat(CHARGE_POINTS)->GetScore() - ChargePoints->GetAmount() >= pPickup->GetWeight())
	{
		if (!Pocket[pPickup->GetGroup()].AddElement(pPickup))
		{
			PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
			if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 7);
			return ACTION_FAILED;
		}
		pPickup->SetOwnerID(id);
		pPickup->GetStatus()->picked = true;
		pPickup->remove();
		GenerateEquipment();
		if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "+1 objet", ACTION_MSG_COLOR);
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s prend %s", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get()), ACTION_MSG_COLOR);
		return ACTION_SUCCESS;
	}
	else
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s ne peut pas prendre %s car il est trop chargé", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get()), ACTION_MSG_COLOR);

	return ACTION_FAILED;
}

void Humanized::AddStatMalus(Pickup *pPickup)
{
	StatPack.GetStat(AGILITY)->IncreaseMalus(pPickup->GetMalus()->AGModifier);
	StatPack.GetStat(STRENGTH)->IncreaseMalus(pPickup->GetMalus()->STModifier);
	StatPack.GetStat(WILLPOWER)->IncreaseMalus(pPickup->GetMalus()->WPModifier);

	if ((pPickup->GetEnchant()->GetEffect() == CURE_SCHOOL_BONUS) | (pPickup->GetEnchant()->GetEffect() == DESTRUCTION_SCHOOL_BONUS) | (pPickup->GetEnchant()->GetEffect() == PROTECTION_SCHOOL_BONUS))
		MagicSchoolPack.GetStat(pPickup->GetEnchant()->GetEffect())->IncreaseMalus(pPickup->GetEnchant()->GetMaxValue());

	UpdateMagicSpell();
}

void Humanized::AddAllStatMalus()
{
	int i, j;

	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		for (j = 0; j < Pocket[i].GetElementCount(); j++)
			if (Pickup::getPickupElement(j, &Pocket[i])->GetStatus()->equiped)
				AddStatMalus(Pickup::getPickupElement(j, &Pocket[i]));
}

void Humanized::RemoveStatMalus(Pickup *pPickup)
{
	StatPack.GetStat(AGILITY)->DecreaseMalus(pPickup->GetMalus()->AGModifier);
	StatPack.GetStat(STRENGTH)->DecreaseMalus(pPickup->GetMalus()->STModifier);
	StatPack.GetStat(WILLPOWER)->DecreaseMalus(pPickup->GetMalus()->WPModifier);

	if ((pPickup->GetEnchant()->GetEffect() == CURE_SCHOOL_BONUS) | (pPickup->GetEnchant()->GetEffect() == DESTRUCTION_SCHOOL_BONUS) | (pPickup->GetEnchant()->GetEffect() == PROTECTION_SCHOOL_BONUS))
		MagicSchoolPack.GetStat(pPickup->GetEnchant()->GetEffect())->DecreaseMalus(pPickup->GetEnchant()->GetMaxValue());

	UpdateMagicSpell();
}

void Humanized::RemoveAllStatMalus()
{
	int i, j;

	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		for (j = 0; j < Pocket[i].GetElementCount(); j++)
			if (Pickup::getPickupElement(j, &Pocket[i])->GetStatus()->equiped)
				RemoveStatMalus(Pickup::getPickupElement(j, &Pocket[i]));
}

void Humanized::UpdateMagicSpell()
{
	int i, j;

	for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
		for (j = 0; j < MagicSpellPack[i].GetElementCount(); j++)
			MagicSpell::getMagicSpellElement(j, &MagicSpellPack[i])->GetManaCost(StatPack.GetStat(WILLPOWER)->GetModifier(), true);

	CurrentSpell.GetManaCost(StatPack.GetStat(WILLPOWER)->GetModifier(), true);
}

bool Humanized::Consume(Potion *pPotion, List *PickupPack, MsgReporter *pMsgReporter)
{
	float score;

	switch (pPotion->GetEffect())
	{
	case LIFE_REGEN:
		score = HitPoints->GetMaxAmount() - HitPoints->GetAmount();
		if (score > pPotion->GetPower()) score = pPotion->GetPower();
		HitPoints->Increase(score, 1, false);
		HitPoints->GenerateBar();
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s consomme %s et récupère %.2f points de vie", GetDisplayedName()->Get(), pPotion->GetDisplayedName()->Get(), score), ACTION_MSG_COLOR);
		break;
	case MANA_REGEN:
		score = ManaPoints->GetMaxAmount() - ManaPoints->GetAmount();
		if (score > pPotion->GetPower()) score = pPotion->GetPower();
		ManaPoints->Increase(score, 1, false);
		ManaPoints->GenerateBar();
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s consomme %s et récupère %.2f points de mana", GetDisplayedName()->Get(), pPotion->GetDisplayedName()->Get(), score), ACTION_MSG_COLOR);
		break;
	case MANA_TRANS_LIFE:
		score = HitPoints->GetMaxAmount() - HitPoints->GetAmount();
		if (score > ManaPoints->GetAmount()) score = ManaPoints->GetAmount();
		if (ManaPoints->GetAmount())
		{
			HitPoints->Increase(score, 1, false);
			ManaPoints->Decrease(score, 1, false);
			HitPoints->GenerateBar();
			ManaPoints->GenerateBar();
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s consomme %s et son mana est transformé en %.2f points de vie", GetDisplayedName()->Get(), pPotion->GetDisplayedName()->Get(), score), ACTION_MSG_COLOR);
		}
		else
		{
			PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
			if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 2);
			return false;
		}
		break;
	case INVISIBILITY:
		Status.invisible = true;
		if (pPotion->GetPower() >= 0)
			PotionEffectDuration = (int)pPotion->GetPower();
		else
			PotionEffectDuration = -(int)pPotion->GetPower();
		Swatch[POTION_EFFECT].setCounter(0);
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s consomme %s et devient invisible pendant %d secondes", GetDisplayedName()->Get(), pPotion->GetDisplayedName()->Get(), PotionEffectDuration), ACTION_MSG_COLOR);
		break;
	}

	pPotion->SetOwnerID(-1);
	pPotion->GetStatus()->picked = false;
	Pocket[OTHERS].RemoveElement(pPotion);
	GenerateEquipment();

	return true;
}

EVENT_TYPE Humanized::Equip(Pickup *pPickup, MsgReporter *pMsgReporter)
{
	int i;
	EVENT_TYPE result = ACTION_SUCCESS;
	Weapon *pWeapon = dynamic_cast<Weapon*>(pPickup);
	Armor *pArmor = dynamic_cast<Armor*>(pPickup);

	if ((pPickup->GetType() == JEWEL) | (pPickup->GetType() == WEAPON) | (pPickup->GetType() == ARMOR))
	{
		//Enlève d'abord tous les objets du même type
		if (pPickup->GetType() != JEWEL)
			for (i = 0; i < Pocket[pPickup->GetGroup()].GetElementCount(); i++)
				if (Pickup::getPickupElement(i, &Pocket[pPickup->GetGroup()])->GetStatus()->equiped)
				{
					if (Pickup::getPickupElement(i, &Pocket[pPickup->GetGroup()])->GetUniqueID() == NECROMISERY)
					{
						result = UnEquip(MagicCircle, false, pMsgReporter);
						if (result == ACTION_FAILED)
							return ACTION_FAILED;
					}
					else
						if (UnEquip(Pickup::getPickupElement(i, &Pocket[pPickup->GetGroup()]), false, pMsgReporter) == ACTION_FAILED)
							return ACTION_FAILED;
				}
		//Applique, si l'objet en possède, l'effet d'enchantement au personnage
		if (pPickup->GetEnchant()->GetEffect())
		{
			EnchantPack.AddElement(pPickup->GetEnchant());
			if (pPickup->GetEnchant()->GetEffect() == SUMMON_NECROMISERY)
				if (SummonNecroMisery(pMsgReporter) == ACTION_FAILED)
					return ACTION_FAILED;
				else
					result = PICKUP_SUMMONED;
		}
		if (pWeapon) EquipedWp = pWeapon;
		if (pArmor)
		{
			EquipedArmor = pArmor;
			updateAni(GetMappedSkin(EquipedArmor->GetSkin()));
		}
		pPickup->GetStatus()->equiped = true;
		AddStatMalus(pPickup);
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s s'équipe de %s", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get()), ACTION_MSG_COLOR);
	}
	else
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 8);
		return ACTION_FAILED;
	}

	return result;
}

EVENT_TYPE Humanized::UnEquip(Pickup *pPickup, bool EvenSummoned, MsgReporter *pMsgReporter)
{
	int i;
	EVENT_TYPE result = ACTION_SUCCESS;

	if ((pPickup->GetSummoned() == true) && (EvenSummoned == false))
	{
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, "Il est impossible d'enlever un objet invoqué", ACTION_MSG_COLOR);
		return ACTION_FAILED;
	}

	if (pPickup->GetEnchant()->GetEffect())
	{
		EnchantPack.RemoveElement(GetEnchant(pPickup->GetEnchant()->GetEffect()), 1);
		if (pPickup->GetEnchant()->GetEffect() == SUMMON_NECROMISERY)
			for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
				if (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetUniqueID() == NECROMISERY)
				{
					if (UnEquip(Weapon::getWeaponElement(i, &Pocket[WEAPONS]), true, pMsgReporter) == ACTION_SUCCESS)
					{
						result = PICKUP_SUMMONED;
						Pocket[WEAPONS].RemoveElement(i, 1, true);
						GenerateEquipment();
					}
					break;
				}
	}

	switch(pPickup->GetType())
	{
	case WEAPON:
		CurrentWp.SetMinDamage(HTH_MIN_DAMAGE);
		CurrentWp.SetMaxDamage(HTH_MAX_DAMAGE);
		CurrentWp.SetCombatSkill(HAND_TO_HAND);
		EquipedWp = NULL;
		break;
	case ARMOR:
		if (EquipedArmor)
		{
			SkillPack.GetSkill(EquipedArmor->GetArmorSkill())->InitXP();
			EquipedArmor = NULL;
		}
		updateAni("HU_NUDE");
		break;
	}

	pPickup->GetStatus()->equiped = false;
	RemoveStatMalus(pPickup);
	if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s enlève %s", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get()), ACTION_MSG_COLOR);

	return result;
}

EVENT_TYPE Humanized::Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	if (pPickup->GetOwnerID() == id)
	{
		if (pPickup->GetStatus()->equiped)
			if (UnEquip(pPickup, false, pMsgReporter) == ACTION_FAILED)
			{
				PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
				if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 7);
				return ACTION_FAILED;
			}
		if (pHumanized->AddToEquipment(pPickup, pMsgReporter, pMiniEvents) == ACTION_SUCCESS)
		{
			if (!Pocket[pPickup->GetGroup()].RemoveElement(pPickup))
			{
				PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
				if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 7);
				return ACTION_FAILED;
			}
			GenerateEquipment();
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, "-1 objet", ACTION_MSG_COLOR);
			return ACTION_SUCCESS;
		}
	}
	else
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 7);
	}

	return ACTION_FAILED;
}

EVENT_TYPE Humanized::Buy(Pickup *pPickup, Humanized *Seller, List *PickupPack, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int i, remain;
	Pickup *BuyerMoneyBag;

	if (!GetAdjacent(Target))
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 4);
		return ACTION_FAILED;
	}

	if (Status.fight)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(ACTION_DENIED, 6);
		return ACTION_FAILED;
	}

	if ((pPickup->GetType() == MONEY) | (pPickup->GetType() == MESSAGE))
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(BUY_PICKUP_DENIED, 0);
		return ACTION_FAILED;
	}

	if (pPickup->GetStatus()->equiped)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(BUY_PICKUP_DENIED, 1);
		return ACTION_FAILED;
	}

	if (pPickup->GetPrice() > purse)
	{
		PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s n'a pas assez d'or pour acheter cet objet", GetDisplayedName()->Get()), TRADING_MSG_COLOR);
		return ACTION_FAILED;
	}

	if (Seller->Give(pPickup, this, pMsgReporter, pMiniEvents) == ACTION_SUCCESS)
	{
		remain = pPickup->GetPrice();
		//Soustrait la somme du Pickup sac par sac
		for (i = 0; i < Pocket[OTHERS].GetElementCount(); i++)
			if (Pickup::getPickupElement(i, &Pocket[OTHERS])->GetType() == MONEY)
			{
				BuyerMoneyBag = Pickup::getPickupElement(i, &Pocket[OTHERS]);
				BuyerMoneyBag->SetPrice(BuyerMoneyBag->GetPrice() - remain);
				if (BuyerMoneyBag->GetPrice() <= 0)
					remain = -BuyerMoneyBag->GetPrice();
				else
					break;
			}
		i = 0;
		//Enlève les sacs d'or vide
		while (i < Pocket[OTHERS].GetElementCount())
			if ((Pickup::getPickupElement(i, &Pocket[OTHERS])->GetType() == MONEY) && (Pickup::getPickupElement(i, &Pocket[OTHERS])->GetPrice() <= 0))
				Pocket[OTHERS].RemoveElement(i, 1);
			else
				i++;
		GenerateEquipment();
		for (i = 0; i < Seller->GetPocket(OTHERS)->GetElementCount(); i++)
			if (Pickup::getPickupElement(i, Seller->GetPocket(OTHERS))->GetType() == MONEY)
				break;
		//Si le vendeur n'a pas de sac d'or, en créé un nouveau
		if (i == Seller->GetPocket(OTHERS)->GetElementCount())
		{
			Pickup *MoneyBag = new Pickup(pPickup->GetPrice(), NONE_PICKUP_UNIQUE_ID, Seller->GetClass(), 17, 17, "Sac d'or");
			MoneyBag->cloneAni(BuyerMoneyBag);
			Seller->AddToEquipment(MoneyBag);
			if (PickupPack) PickupPack->AddElement(MoneyBag);
		}
		else
			Pickup::getPickupElement(i, Seller->GetPocket(OTHERS))->SetPrice(Pickup::getPickupElement(i, Seller->GetPocket(OTHERS))->GetPrice() + pPickup->GetPrice());
		Seller->GenerateEquipment();
		if (pMiniEvents)
		{
			pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("-%d or", pPickup->GetPrice()), TRADING_MSG_COLOR);
			pMiniEvents->ReleaseEvent(Seller->getPosition()->x, Seller->getPosition()->y, BuildString("+%d or", pPickup->GetPrice()), TRADING_MSG_COLOR);
		}
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s achète %s pour %d pièces d'or", GetDisplayedName()->Get(), pPickup->GetDisplayedName()->Get(), pPickup->GetPrice()), TRADING_MSG_COLOR);
		return ACTION_SUCCESS;
	}

	return ACTION_FAILED;
}

void Humanized::EmptyBag()
{
	int i, j;

	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
	{
		for (j = 0; j < Pocket[i].GetElementCount(); j++)
		{
			if (Pickup::getPickupElement(j, &Pocket[i])->GetStatus()->equiped)
				UnEquip(Pickup::getPickupElement(j, &Pocket[i]), true, NULL);
			Pickup::getPickupElement(j, &Pocket[i])->SetOwnerID(-1);
			Pickup::getPickupElement(j, &Pocket[i])->GetStatus()->picked = false;
		}
		Pocket[i].Initialize();
	}

	GenerateEquipment();
}

EVENT_TYPE Humanized::CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	int xp;
	float RecoverPoints;
	EVENT_TYPE result = NONE_EVENT_TYPE;
	EventData1 ActionData;
	Humanized *pHumanized;

	if (Status.Action == ATTACK)
		if ((EquipedArmor != NULL) && (SkillPack.GetSkill(EquipedArmor->GetArmorSkill())->GetUsed() == true))
			SkillPack.GetSkill(EquipedArmor->GetArmorSkill())->SetUsed(false);

	switch (Status.Action)
	{
	case CAST_SPELL:
		if (MagicSkill.GetUsed())
		{
			MagicSkill.SetUsed(false);
			if (MagicSkill.GetResult() < FAILURE)
			{
				//Double une propriété du sort lors d'un critique.
				//Ref: "The Freestyle RPG Basic Rules", page 24.
				if (MagicSkill.GetResult() == CRITICAL_SUCCESS)
				{
					if (CurrentSpell.GetDuration()->flag)
						CurrentSpell.GetDuration()->data *= 2;
					if (CurrentSpell.GetRange()->flag)
						CurrentSpell.GetRange()->data *= 2;
					if (CurrentSpell.GetMinEffect()->flag)
						CurrentSpell.GetMinEffect()->data *= 2;
					if (CurrentSpell.GetMaxEffect()->flag)
						CurrentSpell.GetMaxEffect()->data *= 2;
				}
			}
			ManaPoints->Decrease((float)CurrentSpell.GetManaCost()->RealScore, 1, false);
			ManaPoints->GenerateBar();
			if ((MagicSkill.GetResult() >= FAILURE) | ((GetRange(SpellTarget->GetTileNodeID()) > CurrentSpell.GetRange()->data) && (CurrentSpell.GetRange()->data > 0)))
				CurrentSpell.SetHit(false);
			if ((CurrentSpell.GetHit() == true) && (CurrentSpell.GetSchool() == DESTRUCTION))
			{
				SpellTarget->MagicSavingRoll(&CurrentSpell);
				SpellTarget->GenerateSkill(ORCData, pMsgReporter, pMiniEvents);
				if (SpellTarget->GetMagicResistSkill()->GetUsed())
				{
					SpellTarget->GetMagicResistSkill()->SetUsed(false);
					if (SpellTarget->GetMagicResistSkill()->GetResult() < FAILURE)
						SpellTarget->GetStatus()->ResistSpell = true;
					else
						SpellTarget->GetStatus()->ResistSpell = false;
				}
			}
		}
		break;
	case MEDITATE:
		if (SkillPack.GetSkill(MEDITATION)->GetUsed())
		{
			SkillPack.GetSkill(MEDITATION)->SetUsed(false);
			//Définit les points de Mana récupérés par rapport à la réussite d'une méditation.
			//Ref: "The Freestyle RPG Basic Rules", page 24.
			switch (SkillPack.GetSkill(MEDITATION)->GetResult())
			{
			case CRITICAL_SUCCESS:
				RecoverPoints = 2 * (float)Stat2Pack.GetStat(MAGICAL_AFFINITY)->GetScore();
				break;
			case SUCCESS:
				RecoverPoints = (float)Stat2Pack.GetStat(MAGICAL_AFFINITY)->GetScore();
				break;
			case FAILURE:
				RecoverPoints = (float)Stat2Pack.GetStat(MAGICAL_AFFINITY)->GetScore() / 2 + 0.5f;
				break;
			case CRITICAL_FAILURE:
				RecoverPoints = 0;
				break;
			}
			if (ManaPoints->GetAmount() < ManaPoints->GetMaxAmount())
				RecoverPoints /= 50;
			else
				RecoverPoints = 0;
			ManaPoints->Increase(RecoverPoints, 1, false);
			ManaPoints->GenerateBar();
			CharEvent.RealScore += RecoverPoints;
		}
		break;
	case SPY:
		if (SkillPack.GetSkill(SPYING)->GetUsed())
		{
			SkillPack.GetSkill(SPYING)->SetUsed(false);
			switch (SkillPack.GetSkill(SPYING)->GetResult())
			{
			case CRITICAL_SUCCESS:
				ActionData.RealScore = 200;
				Status.spy = true;
				break;
			case SUCCESS:
				ActionData.RealScore = 100;
				Status.spy = true;
				break;
			case FAILURE:
				ActionData.RealScore = 0;
				Status.spy = false;
				break;
			case CRITICAL_FAILURE:
				ActionData.RealScore = 0;
				Status.spy = false;
				break;
			}
			if (GetRange(Target->GetTileNodeID()) > ActionData.RealScore)
				Status.spy = false;
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s peut espionner à une distance maximale de %d", GetDisplayedName()->Get(), ActionData.RealScore), ACTION_MSG_COLOR);
		}
		break;
	case STEAL:
		if (SkillPack.GetSkill(STEALING)->GetUsed())
		{
			SkillPack.GetSkill(STEALING)->SetUsed(false);
			if (SkillPack.GetSkill(STEALING)->GetResult() < FAILURE)
			{
				bool validate = false;
				pHumanized = dynamic_cast<Humanized*>(Target);
				if (pHumanized)
				{
					if (!Stealing->GetStatus()->equiped)
						validate = true;
					else
						if (SkillPack.GetSkill(STEALING)->GetResult() == CRITICAL_SUCCESS)
							validate = true;
					if (validate)
					{
						if (pHumanized->Give(Stealing, this, pMsgReporter, pMiniEvents) == ACTION_SUCCESS)
						{
							CheckEquipment(pMsgReporter);
							if (Behavior.progress)
							{
								xp = SkillPack.GetSkill(STEALING)->GainXP(NULL, NULL, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
								if ((xp > 0) && (pMiniEvents != NULL))
									pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
							}
							result = PICKUP_STEALED;
						}
						Stealing = NULL;
					}
				}
			}
		}
		if (SkillPack.GetSkill(DISCRETION)->GetUsed())
		{
			SkillPack.GetSkill(DISCRETION)->SetUsed(false);
			if (SkillPack.GetSkill(DISCRETION)->GetResult() >= FAILURE)
			{
				if ((Status.invisible == false) | (SkillPack.GetSkill(DISCRETION)->GetResult() == CRITICAL_FAILURE))
				{
					StopSound(SleepSound);
					Target->GetStatus()->sleep = Status.invisible = false;
					if (TileIATarget)
					{
						Target->SetTarget(this);
						Target->Attack();
					}
					else
						if (Target->GetStatus()->fight)
							Status.attacking = true;
				}
			}
			if (Behavior.progress)
			{
				xp = SkillPack.GetSkill(DISCRETION)->GainXP(NULL, NULL, MAX_CHAR_SKILL_LEVEL, pMsgReporter);
				if ((xp > 0) && (pMiniEvents != NULL))
					pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%d xp", xp), XP_MSG_COLOR);
			}
		}
		break;
	default:
		MagicSkill.SetUsed(false);
		SkillPack.GetSkill(MEDITATION)->SetUsed(false);
		SkillPack.GetSkill(SPYING)->SetUsed(false);
		SkillPack.GetSkill(STEALING)->SetUsed(false);
		SkillPack.GetSkill(DISCRETION)->SetUsed(false);
		result = Character::CheckActionResult(ORCData, pMsgReporter, pMiniEvents);
		break;
	}

	if (CurrentSpell.GetHit())
	{
		if (((SpellTarget == Target) && (SpellTarget->GetStatus()->ResistSpell == false)) | (CurrentSpell.GetSchool() != DESTRUCTION))
		{
			if (CurrentSpell.GetHitSound())
				if (CurrentSpell.GetDuration()->data)
				{
					if (CurrentSpell.GetCheckTurn())
						PlaySound(CurrentSpell.GetHitSound(), 0, true);
					else
						PlaySound(CurrentSpell.GetHitSound(), DMUS_SEG_REPEAT_INFINITE, false);
				}
				else
					PlaySound(CurrentSpell.GetHitSound(), 0, false);
			if ((SpellTarget->GetStatus()->reflecting == false) | (CurrentSpell.GetSchool() != DESTRUCTION))
				SpellHit(&CurrentSpell, pMsgReporter, pMiniEvents);
			else
			{
				pHumanized = dynamic_cast<Humanized*>(SpellTarget);
				if (pHumanized)
				{
					pHumanized->SetSpellTarget(this);
					pHumanized->SpellHit(&CurrentSpell, pMsgReporter, pMiniEvents);
				}
			}
		}
		else
			StopSpellEffect(&CurrentSpell, pMsgReporter, pMiniEvents);
	}

	return result;
}

void Humanized::Meditate()
{
	if ((SkillPack.GetSkill(MEDITATION)->GetUsed() == false) && (SkillPack.GetSkill(MEDITATION)->GetLevel() > 0) && (Status.fight == false))
	{
		Status.access = false;
		Status.Action = MEDITATE;
		CurrentSkill = SkillPack.GetSkill(MEDITATION)->Use();
	}
}

void Humanized::Spy()
{
	if ((SkillPack.GetSkill(SPYING)->GetUsed() == false) && (SkillPack.GetSkill(SPYING)->GetLevel() > 0) && (Status.spy == false) && (Status.fight == false))
	{
		Status.Action = SPY;
		CurrentSkill = SkillPack.GetSkill(SPYING)->Use();
	}
}

void Humanized::BeDiscret()
{
	if ((SkillPack.GetSkill(DISCRETION)->GetUsed() == false) && (SkillPack.GetSkill(DISCRETION)->GetLevel() > 0))
		CurrentSkill = SkillPack.GetSkill(DISCRETION)->Use();
}

void Humanized::Steal(Pickup *pPickup)
{
	if ((SkillPack.GetSkill(STEALING)->GetUsed() == false) && (SkillPack.GetSkill(STEALING)->GetLevel() > 0) && (Status.fight == false) && (GetAdjacent(Target) == true))
	{
		Status.Action = STEAL;
		Stealing = pPickup;
		BeDiscret();
		CurrentSkill = SkillPack.GetSkill(STEALING)->Use();
	}
}

bool Humanized::Attack()
{
	if (Character::Attack())
	{
		if (EquipedArmor)
			CurrentSkill = SkillPack.GetSkill(EquipedArmor->GetArmorSkill())->Use();
		return true;
	}

	return false;
}

bool Humanized::Concentrate(bool RingIfFailure, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	if (CurrentSpell.GetHit())
		StopSpellEffect(&CurrentSpell, pMsgReporter, pMiniEvents);

	if (MemorizedSpell)
		CurrentSpell = *MemorizedSpell;
	else
	{
		if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 2);
		return false;
	}

	if (ManaPoints->GetAmount() < CurrentSpell.GetManaCost()->RealScore)
	{
		if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 0);
		return false;
	}

	if (MagicSkill.GetLevel() < CurrentSpell.GetLevel())
	{
		if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 1);
		return false;
	}

	if (CurrentSpell.GetSchool() == DESTRUCTION)
		if ((Target == this) | (Target->GetStatus()->dead == true))
		{
			if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
			if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 3);
			return false;
		}
		else
			if (GetRange(Target->GetTileNodeID()) > CurrentSpell.GetRange()->data * 2)
			{
				if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
				if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 4);
				return false;
			}

	if ((Status.concentrate == true) | (Status.attack == true) | (Status.hitted == true) | ((HitPoints->GetAmount() == HitPoints->GetMaxAmount()) && (CurrentSpell.GetSchool() == CURE)))
	{
		if (RingIfFailure) PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
		if (pMsgReporter) pMsgReporter->AddMessage(CAST_SPELL_DENIED, 5);
		return false;
	}

	Status.access = false;
	Status.concentrate = true;
	Status.Action = CAST_SPELL;
	*CurrentSpell.GetHitData() = EventData2();

	return true;
}

void Humanized::CastSpell()
{
	if (Status.turn)
	{
		if (Status.Action == NONE_CHAR_ACTION)
			Status.Action = CAST_SPELL;
		if (CurrentSpell.GetSchool() == DESTRUCTION)
			Status.attacking = true;
		Wait(SPELL_CASTING, CurrentSpell.GetCastTime(), 1);
	}

	if (!Swatch[SPELL_CASTING].isGoing())
	{
		Status.concentrate = Status.ResistSpell = false;
		if ((Status.stunned == false) && (ManaPoints->GetAmount() >= CurrentSpell.GetManaCost()->RealScore))
		{
			CurrentSpell.SetHit(true);
			SpellTarget = Target;
			CurrentSkill = MagicSkill.Use(&CurrentSpell, Stat2Pack.GetStat(MAGICAL_AFFINITY));
		}
	}
}

EventData2 Humanized::Hit(Weapon *pWeapon, MsgReporter *pMsgReporter)
{
	float DiceScore;
	EventData2 HitData;

	DiceScore = (float)Clock::random(pWeapon->GetMinDamage(), pWeapon->GetMaxDamage());
	HitData.score = DiceScore;

	if (pWeapon->GetStunning())
		AttackTarget->GetStatus()->WpStunned = true;

	if ((pWeapon->GetCombatSkill() != HAND_TO_HAND) && (pWeapon->GetTranspiercing() == false))
	{
		Humanized *pHumanized = dynamic_cast<Humanized*>(AttackTarget);
		if ((pHumanized != NULL) && (pHumanized->GetEquipedArmor() != NULL))
			pHumanized->ReduceDamage(&DiceScore, pMsgReporter);
	}

	HitData.RealScore = DiceScore;
	AttackTarget->GetStatus()->hitted = true;
	AttackTarget->GetHitPoints()->Decrease(DiceScore, 1, false);
	AttackTarget->GetHitPoints()->GenerateBar();

	return HitData;
}

void Humanized::SpellHit(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	float DiceScore, buffer;
	Humanized *pHumanized = dynamic_cast<Humanized*>(SpellTarget);

	DiceScore = (float)Clock::random(pMagicSpell->GetMinEffect()->data, pMagicSpell->GetMaxEffect()->data);

	switch (pMagicSpell->GetEffect())
	{
	case SELF_HEAL:
		DiceScore /= 10;
		break;
	case ANTIMAGIC:
		DiceScore /= 10;
		buffer = DiceScore;
		break;
	default:
		pMagicSpell->GetHitData()->score = DiceScore;
		break;
	}

	if ((pMagicSpell->GetDuration()->data > 0) && (pMagicSpell->GetCheckTurn() == 0))
		pMagicSpell->GetEffectDuration()->goCounter(1, 0, pMagicSpell->GetDuration()->data, false);

	if (pHumanized)
		if ((pMagicSpell->GetSchool() == DESTRUCTION) && (pHumanized != this))
			pHumanized->AbsorbMagicEffect(&DiceScore);

	//Applique l'effet du sort sur le sujet.
	switch (pMagicSpell->GetEffect())
	{
	case SELF_HEAL:
		HitPoints->Increase(DiceScore, 1, true);
		HitPoints->GenerateBar();
		if (!HitPoints->GetRegulator()->isGoing())
			pMagicSpell->GetHitData()->RealScore += DiceScore;
		break;
	case LIFE_DAMAGE:
		if (SpellTarget != this)
		{
			SpellTarget->GetStatus()->hitted = true;
			SpellTarget->GetHitPoints()->Decrease(DiceScore, 1, false);
			SpellTarget->GetHitPoints()->GenerateBar();
			pMagicSpell->GetHitData()->RealScore = DiceScore;
		}
		break;
	case CHOC_WAVE:
		if (SpellTarget != this)
		{
			SpellTarget->GetStatus()->hitted = true;
			SpellTarget->GetStatus()->stunned = true;
		}
		break;
	case ANTIMAGIC:
		if (SpellTarget != this)
		{
			SpellTarget->GetStatus()->hitted = true;
			if (pHumanized)
			{
				pHumanized->GetManaPoints()->Decrease(DiceScore, 1, true);
				pHumanized->GetManaPoints()->GenerateBar();
				if (!pHumanized->GetManaPoints()->GetRegulator()->isGoing())
				{
					pMagicSpell->GetHitData()->score += buffer;
					pMagicSpell->GetHitData()->RealScore += DiceScore;
				}
			}
		}
		break;
	case ABSORBING:
		Status.absorbing = true;
		break;
	case REFLECTING:
		Status.reflecting = true;
		break;
	}

	if (Status.dead)
	{
		StopSound(pMagicSpell->GetHitSound(), true);
		StopSpellEffect(pMagicSpell, pMsgReporter, pMiniEvents);
	}

	//L'effet du sort est terminé.
	if (((pMagicSpell->GetEffectDuration()->isGoing() == false) | (pMagicSpell->GetDuration()->data == 0)) && (pMagicSpell->GetCheckTurn() == 0))
		StopSpellEffect(pMagicSpell, pMsgReporter, pMiniEvents);
	else
		if ((pMagicSpell->GetCheckTurn() == 1) && (pMagicSpell->GetEffectDuration()->getCounter() >= pMagicSpell->GetDuration()->data))
			StopSpellEffect(pMagicSpell, pMsgReporter, pMiniEvents);
}

void Humanized::ReduceDamage(float *damage, MsgReporter *pMsgReporter)
{
	float reduction = *damage * EquipedArmor->GetSuccessRate() / 100;

	*damage -= reduction;

	if (*damage < 0)
		*damage = 0;
}

void Humanized::AbsorbMagicEffect(float *effect)
{
	int DiceScore, EnchantID;
	float reduction = 0;

	DiceScore = 0;

	if ((Status.absorbing == true) && (CurrentSpell.GetEffect() == ABSORBING))
		DiceScore += Clock::random(CurrentSpell.GetMinEffect()->data, CurrentSpell.GetMaxEffect()->data);

	EnchantID = GetEnchant(MAGIC_ABSORBER);

	if (EnchantID != -1)
		DiceScore += Clock::random(Enchant::getEnchantElement(EnchantID, &EnchantPack)->GetMinValue(), Enchant::getEnchantElement(EnchantID, &EnchantPack)->GetMaxValue());

	if (DiceScore) reduction = *effect * DiceScore / 100;

	*effect -= reduction;

	if (*effect < 0)
		*effect = 0;
}

void Humanized::StopSpellEffect(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	if (pMagicSpell->GetDuration()->data)
		StopSound(pMagicSpell->GetHitSound(), !pMagicSpell->GetCheckTurn());
	else
		StopSound(pMagicSpell->GetHitSound(), true);

	switch (pMagicSpell->GetEffect())
	{
	case CHOC_WAVE:
		if (SpellTarget->GetStatus()->stunned)
			SpellTarget->GetStatus()->stunned = false;
		if (Status.stunned)
			Status.stunned = false;
		break;
	case ABSORBING:
		Status.absorbing = false;
		break;
	case REFLECTING:
		Status.reflecting = false;
		break;
	}

	pMagicSpell->SetHit(false);
	pMagicSpell->GetEffectDuration()->setCounter(0);
	
	if ((SpellTarget->GetStatus()->ResistSpell == false) | (pMagicSpell->GetSchool() != DESTRUCTION))
		switch (pMagicSpell->GetEffect())
		{
		case SELF_HEAL:
			if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("+%.2f vie", pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s s'est regénéré de %.2f points de vie", GetDisplayedName()->Get(), pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
			break;
		case LIFE_DAMAGE:
			if (Status.reflecting)
			{
				if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("-%.2f vie", pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
				if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige %.2f (%.2f) points de dégats magiques à lui-même", SpellTarget->GetDisplayedName()->Get(), pMagicSpell->GetHitData()->RealScore, pMagicSpell->GetHitData()->score), MAGIC_EFFECT_MSG_COLOR);
			}
			else
			{
				if (pMiniEvents) pMiniEvents->ReleaseEvent(SpellTarget->getPosition()->x, SpellTarget->getPosition()->y, BuildString("-%.2f vie", pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
				if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige %.2f (%.2f) points de dégats magiques à %s", GetDisplayedName()->Get(), pMagicSpell->GetHitData()->RealScore, pMagicSpell->GetHitData()->score, SpellTarget->GetDisplayedName()->Get()), MAGIC_EFFECT_MSG_COLOR);
			}
			break;
		case CHOC_WAVE:
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s a terminé l'effet Onde de Choc", GetDisplayedName()->Get()), MAGIC_EFFECT_MSG_COLOR);
			break;
		case ANTIMAGIC:
			if (Status.reflecting)
			{
				if (pMiniEvents) pMiniEvents->ReleaseEvent(position.x, position.y, BuildString("-%.2f mana", pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
				if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige une perte de %.2f (%.2f) points de mana à lui-même", SpellTarget->GetDisplayedName()->Get(), pMagicSpell->GetHitData()->RealScore, pMagicSpell->GetHitData()->score), MAGIC_EFFECT_MSG_COLOR);
			}
			else
			{
				if (pMiniEvents) pMiniEvents->ReleaseEvent(SpellTarget->getPosition()->x, SpellTarget->getPosition()->y, BuildString("-%.2f mana", pMagicSpell->GetHitData()->RealScore), MAGIC_EFFECT_MSG_COLOR);
				if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s inflige une perte de %.2f (%.2f) points de mana à %s", GetDisplayedName()->Get(), pMagicSpell->GetHitData()->RealScore, pMagicSpell->GetHitData()->score, SpellTarget->GetDisplayedName()->Get()), MAGIC_EFFECT_MSG_COLOR);
			}
			break;
		case ABSORBING:
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s a terminé l'effet Absorbtion", GetDisplayedName()->Get()), MAGIC_EFFECT_MSG_COLOR);
			break;
		case REFLECTING:
			if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("%s a terminé l'effet Réflection", GetDisplayedName()->Get()), MAGIC_EFFECT_MSG_COLOR);
			break;
	}
}

void Humanized::Curse(bool value, short chao)
{
	Status.cursed = value;

	SetPrimaryBehavior();

	// On change la couleur des cheveux
	if (!IsAPlant())
	{
		initDynColors(2);

		if (Status.cursed)
		{
			addDynColor(0, new DynColor(RGB(232, 180, 0), RGB(248, 71, 20)));
			addDynColor(1, new DynColor(RGB(206, 159, 0), RGB(227, 56, 6)));
		}
		else
		{
			addDynColor(0, new DynColor(RGB(248, 71, 20), RGB(232, 180, 0)));
			addDynColor(1, new DynColor(RGB(227, 56, 6), RGB(206, 159, 0)));
		}

		replaceAniColors();
	}
}

void Humanized::Diiiiiie(MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Character::Diiiiiie(pMsgReporter, pMiniEvents);

	SpellTarget = this;

	if (Status.sleep) StopSound(SleepSound);
}

void Humanized::Reborn(bool start, MsgReporter *pMsgReporter)
{
	int i;

	Character::Reborn(start, pMsgReporter);

	if (start)
	{
		purse = 0;
		for (i = 0; i < SkillPack.GetElementCount(); i++)
			*SkillPack.getCharSkillElement(i)->GetBPData() = BonusPoints();
		*MagicSkill.GetBPData() = BonusPoints();
		for (i = 0; i < MagicSchoolPack.GetElementCount(); i++)
			MagicSpellPack[i].Initialize();
		EquipedWp = NULL;
		EquipedArmor = NULL;
	}

	MemorizedSpell = NULL;
	CurrentSpell = MagicSpell();
	Stealing = NULL;
	SpellTarget = this;
	HumanizedTarget = NULL;
	TileIATarget = NULL;
}

void Humanized::ShutUp()
{
	if (Status.PlayingRepeatSound)
	{
		StopSound(SleepSound);
		StopSound(CurrentSpell.GetHitSound());
	}
}

bool Humanized::IsEmpty()
{
	int i;

	//On regarde si les poches sont pleines
	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
		if (Pocket[i].GetElementCount())
			return false;

	return true;
}

EVENT_TYPE Humanized::SummonNecroMisery(MsgReporter *pMsgReporter)
{
	Weapon *NecroMisery = new Weapon(7, 10, (CHAR_COMBAT_SKILL)CombatSkillPack.GetBestCombatSkill(true)->GetSkillId(), 1000000, 20, true, NECROMISERY, NONE_CHAR_CLASS, PickupMalus(-10, -10, -12), Enchant(0, 0, MAGIC_TRANSPERCOR), 7, 21, "Necro Misery");

	NecroMisery->initAni();
	PlaySound(Sound::getSoundElement(SPELL_EFFECT_ANTIMAGIC, &SoundPack), 0, false);

	if (AddToEquipment(NecroMisery, pMsgReporter) == ACTION_FAILED)
	{
		delete NecroMisery;
		return ACTION_FAILED;
	}

	return Equip(NecroMisery, pMsgReporter);
}

Str Humanized::GetMappedSkin(int SkinId)
{
	switch (SkinId)
	{
	case CLOTHES:
		return "HU_CLOTH";
	case LEATHER_CLOTHES:
		return "HU_LEATH";
	case MESH_COAT:
		return "HU_MESH";
	case WHITE_DRESS:
		return "HU_DRESS1";
	case BLACK_DRESS:
		return "HU_DRESS2";
	case BLUE_DRESS:
		return "HU_DRESS3";
	case PADS_ARMOR:
		return "HU_PADS";
	case IRON_ARMOR:
		return "HU_ARMOR";
	default:
		return "HU_NUDE";
	}
}

Humanized * Humanized::GetHumanizedTarget()
{
	if (HumanizedTarget)
		return HumanizedTarget;

	return dynamic_cast<Humanized*>(Target);
}

TileIA * Humanized::GetTileIATarget()
{
	if (TileIATarget)
		return TileIATarget;

	return dynamic_cast<TileIA*>(Target);
}
