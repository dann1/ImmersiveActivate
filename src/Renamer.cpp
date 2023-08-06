#include "Renamer.h"

// TODO: Make name configurable per form type
std::string GetFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_name)
{
	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();

	logger::debug("CrossHair FormType: {}", a_formType);

	switch (a_formType) {
	// TODO: Refine by keyword. Humanoid, Creature, race, etc.
	case RE::FormType::NPC:
	case RE::FormType::LeveledNPC:
		return "NPC";
	case RE::FormType::Door:
		return "Door";
	case RE::FormType::Activator:
		return a_name;
	case RE::FormType::Container:
		return "Storage";
	case RE::FormType::Furniture:
		return a_name;
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		return "Resource";
	case RE::FormType::Ingredient:
		return "Reagent";
	case RE::FormType::AlchemyItem:
		return "Provision";
	case RE::FormType::Ammo:
		return "Ammunition";
	case RE::FormType::Weapon:
		return "Weapon";
	case RE::FormType::Armor:
		return "Equipment";
	case RE::FormType::Scroll:
	case RE::FormType::Note:
		return "Document";
	case RE::FormType::Book:
		return "Book";
	case RE::FormType::SoulGem:
		return "SoulGem";
	case RE::FormType::KeyMaster:
		return "Key";
	default:
		return "Item";
	}
}
