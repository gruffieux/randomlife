#ifndef CPOTION
#define CPOTION

#include "cpickup.h"

class Potion : public Pickup
{
private:
	int ElementPower[TOTAL_PICKUP_POTION_ELEMENT];
	float power;
	PICKUP_POTION_EFFECT effect;
public:
	Potion();
	Potion(float Potion_power, PICKUP_POTION_EFFECT Potion_effect, int Pickup_price, float Pickup_weight, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name);
	Potion(Potion &model);
	~Potion() {}
	void SetElementPower(int index, int value) {ElementPower[index] = value;}
	void SetPower(float Potion_power) {power = Potion_power;}
	void SetEffect(PICKUP_POTION_EFFECT Potion_effect) {effect = Potion_effect;}
	void GenerateAni(int frame);
	bool compare(Object *pObject);
	Str GetEffectDescription();
	float GetPower() {return power;}
	int GetElementPower(int index) {return ElementPower[index];}
	PICKUP_POTION_EFFECT GetEffect() {return effect;}
	static Potion* getPotionElement(int index, List *pList) {return dynamic_cast<Potion*>(pList->GetElement(index));}
};

#endif