#include "cenchant.h"

Enchant::Enchant()
{
	MinValue = MaxValue = 0;
	effect = NONE_PICKUP_ENCHANT;
}

Enchant::Enchant(int PE_MinValue, int PE_MaxValue, PICKUP_ENCHANT PE_effect)
{
	MinValue = PE_MinValue;
	MaxValue = PE_MaxValue;
	effect = PE_effect;
}

Str Enchant::GetDescription()
{
	switch (effect)
	{
	case MAGIC_TRANSPERCOR:
		return "Contourne la Réflection";
	case MAGIC_ABSORBER:
		return "Absorbe la magie";
	case LIFE_MANA_GENERATOR:
		return "Génère vie et mana";
	case CURE_SCHOOL_BONUS:
		return "Bonus de Guérison";
	case DESTRUCTION_SCHOOL_BONUS:
		return "Bonus de Destruction";
	case PROTECTION_SCHOOL_BONUS:
		return "Bonus de Protection";
	case SUMMON_NECROMISERY:
		return "Invoque NecroMisery";
	default:
		return "Aucun";
	}
}
