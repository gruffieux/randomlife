#ifndef CINGREDIENT
#define CINGREDIENT

#include "cpickup.h"

class Ingredient : public Pickup
{
private:
	int ChemicalID, EffectFactor[TOTAL_PICKUP_POTION_EFFECT];
	PICKUP_INGREDIENT_TYPE IngredientType;
public:
	Ingredient();
	Ingredient(PICKUP_INGREDIENT_TYPE Ingredient_type, int Pickup_price, float Pickup_weight, PICKUP_TYPE Pickup_type, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name);
	Ingredient(Ingredient &model);
	~Ingredient() {}
	void SetChemicalID(int Ingredient_ChemicalID) {ChemicalID = Ingredient_ChemicalID;}
	void initAni();
	void GenerateLivingTile();
	void GenerateEffectFactor();
	void GenerateAni(int frame);
	bool compare(Object *pObject);
	Str GetTypeDescription() {return "Ingrédient";}
	int GetChemicalID() {return ChemicalID;}
	int GetEffectFactor(int index) {return EffectFactor[index];}
	PICKUP_INGREDIENT_TYPE GetIngredientType() {return IngredientType;}
	static Ingredient* getIngredientElement(int index, List *pList) {return dynamic_cast<Ingredient*>(pList->GetElement(index));}
};

#endif