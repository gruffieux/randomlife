#include "cingredient.h"
#include "../RLRes/RLRes/resource.h"

Ingredient::Ingredient()
{
	int i;

	ChemicalID = 0;
	for (i = 0; i < TOTAL_PICKUP_POTION_EFFECT; i++)
		EffectFactor[i] = 0;
	IngredientType = NONE_PICKUP_INGREDIENT_TYPE;
}

Ingredient::Ingredient(PICKUP_INGREDIENT_TYPE Ingredient_type, int Pickup_price, float Pickup_weight, PICKUP_TYPE Pickup_type, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, false, NONE_PICKUP_UNIQUE_ID, Pickup_type, INGREDIENTS, Pickup_Class, PickupMalus(), Enchant()), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	int i;

	ChemicalID = 0;
	for (i = 0; i < TOTAL_PICKUP_POTION_EFFECT; i++)
		EffectFactor[i] = 0;
	IngredientType = Ingredient_type;
}

Ingredient::Ingredient(Ingredient &model) : Pickup(model), Item(model)
{
	int i;

	ChemicalID = model.GetChemicalID();
	for (i = 0; i < TOTAL_PICKUP_POTION_EFFECT; i++)
		EffectFactor[i] = model.GetEffectFactor(i);
	IngredientType = model.GetIngredientType();
}

void Ingredient::initAni()
{
	animations.RemoveAllElement(0, true);
	animations.Initialize(1);

	switch (type)
	{
	case FLOWER:
		addAni(0, IDB_FLOWERS, "", 3, 32, 32, RGB(255, 255, 255), 90);
		break;
	case FRUIT:
		addAni(0, IDB_FRUITS, "", 5, 22, 22, RGB(255, 255, 255), 90);
		break;
	case DRINK:
		addAni(0, IDB_DRINKS, "", 3, 16, 20, RGB(255, 255, 255), 90);
		break;
	}
}

void Ingredient::GenerateLivingTile()
{
	int i;

	for (i = 0; i < MAX_LIVING_TILE_ID; i++)
		LivingTileID[i] = 0;

	switch (ChemicalID / CHEMICAL_SCALE)
	{
	case LIFE_REGEN:
		AddLivingTile(6);
		break;
	case MANA_REGEN:
		AddLivingTile(7);
		break;
	case MANA_TRANS_LIFE:
		AddLivingTile(1);
		break;
	default:
		AddLivingTile(1);
		AddLivingTile(7);
		break;
	}
}

void Ingredient::GenerateEffectFactor()
{
	int i, n;

	// Les fleurs et fruits génèrent un facteur additionnant en fonction de l'effet
	// Les liquides génèrent un facteur divisant pour tous les effets

	if ((type == FLOWER) | (type == FRUIT))
	{
		for (i = 1; i < TOTAL_PICKUP_POTION_EFFECT; i++)
		{
			n = i * CHEMICAL_SCALE - ChemicalID;
			if (n < 0)
				n = -n;
			EffectFactor[i] = CHEMICAL_SCALE - n;
		}
	}
	else
	{
		switch (IngredientType)
		{
		case DRINK_WINE:
			n = 3;
			break;
		case DRINK_WATER:
			n = 2;
			break;
		case DRINK_TROLLFLUID:
			n = -3;
			break;
		}
		for (i = 1; i < TOTAL_PICKUP_POTION_EFFECT; i++)
			EffectFactor[i] = n;
	}
}

void Ingredient::GenerateAni(int frame)
{
	switch (IngredientType)
	{
	case FLOWER_WHITE:
		frame = 0;
		break;
	case FLOWER_RED:
		frame = 1;
		break;
	case FLOWER_BLUE:
		frame = 2;
		break;
	case FRUIT_RED:
		frame = 0;
		break;
	case FRUIT_PINK:
		frame = 1;
		break;
	case FRUIT_BLUE:
		frame = 2;
		break;
	case FRUIT_BLACK:
		frame = 3;
		break;
	case FRUIT_ORANGE:
		frame = 4;
		break;
	case DRINK_WATER:
		frame = 0;
		break;
	case DRINK_WINE:
		frame = 1;
		break;
	case DRINK_TROLLFLUID:
		frame = 2;
		break;
	}

	Pickup::GenerateAni(frame);
}

bool Ingredient::compare(Object *pObject)
{
	Ingredient *pIngredient = dynamic_cast<Ingredient*>(pObject);

	if (pIngredient)
		return (Pickup::compare(pIngredient) && IngredientType == pIngredient->GetIngredientType());
	
	return false;
}
