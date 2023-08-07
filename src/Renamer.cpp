#include "Renamer.h"
#include "Settings.h"


// TODO: Make name configurable per form type
std::string GetFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_name)
{
	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();
	const auto settings = Settings::GetSingleton();

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
			return settings->npc_show.text;
		}
	case RE::FormType::Door:
		return settings->door_show.text;
	case RE::FormType::Activator:
		return a_name;
	case RE::FormType::Container:
		return settings->container_show.text;
	case RE::FormType::Furniture:
		return a_name;
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		return settings->resource_show.text;
	case RE::FormType::Ingredient:
		return settings->ingredient_show.text;
	case RE::FormType::AlchemyItem:  // TODO: Potion/Poison detection
		return settings->alchemy_item_show.text;
	case RE::FormType::Ammo:
		return settings->ammo_show.text;
	case RE::FormType::Weapon:
		return settings->weapon_show.text;
	case RE::FormType::Armor:
		if (a_object->IsJewelry()) {
			return "Jewel";
		} else {
			return settings->armor_show.text;
		}
	case RE::FormType::Scroll:
	case RE::FormType::Note:
		return settings->scroll_note_show.text;
	case RE::FormType::Book:
		return settings->book_show.text;
	case RE::FormType::SoulGem:
		return settings->soulgem_show.text;
	case RE::FormType::KeyMaster:
		return settings->key_show.text;
	default:  // TODO: Detect septim as money. FormID F in hex.
		if (a_object->IsLockpick()) {
			return "Lockpick";
		} else {
			return settings->item_show.text;
		}
	}
}
