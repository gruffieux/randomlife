#ifndef CWEAPON
#define CWEAPON

#include "cpickup.h"

class Weapon : public Pickup
{
private:
	int MinDamage, MaxDamage;
	bool stunning, transpiercing;
	CHAR_COMBAT_SKILL CombatSkill;
public:
	Weapon();
	Weapon(CHAR_COMBAT_SKILL Weapon_CombatSkill, int Pickup_price, float Pickup_weight, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name);
	Weapon(int Weapon_MinDamage, int Weapon_MaxDamage, CHAR_COMBAT_SKILL Weapon_CombatSkill, int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name);
	Weapon(Weapon &model);
	~Weapon() {}
	void SetMinDamage(int Weapon_MinDamage) {MinDamage = Weapon_MinDamage;}
	void SetMaxDamage(int Weapon_MaxDamage) {MaxDamage = Weapon_MaxDamage;}
	void SetStunning(bool Weapon_stunning) {stunning = Weapon_stunning;}
	void SetTranspiercing(bool Weapon_transpiercing) {transpiercing = Weapon_transpiercing;}
	void SetCombatSkill(CHAR_COMBAT_SKILL Weapon_CombatSkill) {CombatSkill = Weapon_CombatSkill;}
	void initAni();
	Str GetTypeDescription() {return "Arme";}
	void GenerateAni(int frame) {Pickup::GenerateAni(0);}
	bool compare(Object *pObject);
	int GetMinDamage() {return MinDamage;}
	int GetMaxDamage() {return MaxDamage;}
	bool GetStunning() {return stunning;}
	bool GetTranspiercing() {return transpiercing;}
	CHAR_COMBAT_SKILL GetCombatSkill() {return CombatSkill;}
	static Weapon* getWeaponElement(int index, List *pList) {return dynamic_cast<Weapon*>(pList->GetElement(index));}
};

#endif