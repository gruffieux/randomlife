#ifndef CARMOR
#define CARMOR

#include "cpickup.h"

class Armor : public Pickup
{
private:
	int rating, SuccessRate, skin;
	CHAR_SKILL ArmorSkill;
public:
	Armor();
	Armor(int Armor_skin, CHAR_SKILL Armor_Skill, int Pickup_price, float Pickup_weight, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name);
	Armor(int Armor_rating, int Armor_skin, CHAR_SKILL Armor_Skill, int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name);
	Armor(Armor &model);
	~Armor() {}
	void SetRating(int Armor_rating) {rating = Armor_rating;}
	void GenerateSuccessRate(int CharArmorLevel);
	void GenerateAni(int frame);
	bool compare(Object *pObject);
	Str GetTypeDescription();
	int GetRating() {return rating;}
	int GetSuccessRate() {return SuccessRate;}
	int GetSkin() {return skin;}
	CHAR_SKILL GetArmorSkill() {return ArmorSkill;}
	static Armor* getArmorElement(int index, List *pList) {return dynamic_cast<Armor*>(pList->GetElement(index));}
};

#endif