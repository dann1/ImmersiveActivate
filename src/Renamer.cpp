#include "Renamer.h"

// TODO: Make name configurable per form type
std::string GetFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_name)
{
	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();

	logger::debug("CrossHair FormType: {}", a_formType);

	switch (a_formType) {
	case RE::FormType::NPC:
	case RE::FormType::LeveledNPC:
		if (a_object->IsDead()) {
			return "Corpse";
		} else if (a_object->IsAnimal()) {
			return "Animal";
		} else if (a_object->IsChild()) {
			return "Child";
		} else if (a_object->IsDragon()) {
			return "Dragon";
		} else {
			return "Person";
		}
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
	// TODO: Potion/Poison detection
	case RE::FormType::AlchemyItem:
		return "Provision";
	case RE::FormType::Ammo:
		return "Ammunition";
	case RE::FormType::Weapon:
		return "Weapon";
	case RE::FormType::Armor:
		if (a_object->IsJewelry()) {
			return "Jewel";
		} else {
			return "Equipment";
		}
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
		// TODO: Detect septim as money
		const auto a_formID = a_object->GetFormID();

		if (a_object->IsLockpick()) {
			return "Lockpick";
		} else {
			return "Item";
		}
	}
}
