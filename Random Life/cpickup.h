#ifndef CPICKUP
#define CPICKUP

#include <cvisual.h>
#include <ctileactor.h>
#include "cenchant.h"
#include "main.h"

class Pickup : public Visual, public TileActor
{
private:
	int OwnerID, price;
	float weight;
	bool summoned;
	PICKUP_UNIQUE_ID UniqueID;
	PickupMalus Malus;
	PickupStatus Status;
	Enchant PickupEnchant;
	Str DisplayedName;
protected:
	PICKUP_TYPE type;
	PICKUP_GROUP Group;
	CHAR_CLASS Class;
public:
	Pickup();
	Pickup(int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, PICKUP_TYPE Pickup_type, PICKUP_GROUP Pickup_Group, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant);
	Pickup(int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, PICKUP_TYPE Pickup_type, PICKUP_GROUP Pickup_Group, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name);
	Pickup(int Pickup_price, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name);
	Pickup(Pickup &model);
	~Pickup() {}
	void SetOwnerID(int Pickup_OwnerID) {OwnerID = Pickup_OwnerID;}
	void SetPrice(int Pickup_price) {price = Pickup_price;}
	void SetWeight(float Pickup_weight) {weight = Pickup_weight;}
	void initAni();
	virtual void GenerateAni(int frame);
	bool compare(Object *pObject);
	virtual Str GetTypeDescription();
	int GetOwnerID() {return OwnerID;}
	int GetPrice() {return price;}
	float GetWeight() {return weight;}
	bool GetSummoned() {return summoned;}
	PICKUP_UNIQUE_ID GetUniqueID() {return UniqueID;}
	PICKUP_TYPE GetType() {return type;}
	PICKUP_GROUP GetGroup() {return Group;}
	CHAR_CLASS GetClass() {return Class;}
	PickupMalus* GetMalus() {return &Malus;}
	PickupStatus* GetStatus() {return &Status;}
	Enchant* GetEnchant() {return &PickupEnchant;}
	Str* GetDisplayedName() {return &DisplayedName;}
	static Pickup* getPickupElement(int index, List *pList) {return dynamic_cast<Pickup*>(pList->GetElement(index));}
};

#endif