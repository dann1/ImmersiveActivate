#include "Renamer.h"
#include "Settings.h"

// Object Names have 2 or lines. Replacing the whole name leads to plain white text.
// Action: Talk, Sleep with optional color formatting
// Name: Camilla Carlotta, Bed (Owned)
// Embedded Images: empty tag triangle, or locked
std::string ReplaceRefText(const std::string& original, const std::string& replacement)
{
	std::vector<std::string> lines;
	std::stringstream ss(original);
	std::string line;
	int line_count = 0;

	while (std::getline(ss, line, '\n')) {
		if (line_count == 1) {  // If it's the second line, replace it
			lines.push_back(replacement);
		}
		else {
			lines.push_back(line);
		}
		line_count++;
	}

	// Combine the modified lines back into a single string
	std::string result;
	for (size_t i = 0; i < lines.size(); ++i) {
		result += lines[i];
		if (i < lines.size() - 1) {
			result += "\n";  // Add newline character between lines
		}
	}

	return result;
}

std::string DecideSkip(std::string a_setting_value, std::string a_text)
{
	if (a_setting_value == "-") {
		return a_text;
	}
	return ReplaceRefText(a_text, a_setting_value);
}

struct SpecialForms {
	const std::uint32_t septim = 0xF;
	const std::uint32_t skooma = 0x00057A7A;
	const std::uint32_t skoomaRedWater = 0x0201391D;
};

struct Weapons {
	const std::vector<std::string> blades = { "WeapTypeSword", "", "WeapTypeGreatsword", "WeapTypeDagger" };
	const std::vector<std::string> axes = { "WeapTypeWarAxe", "WeapTypeBattleaxe" };
	const std::vector<std::string> bludgeons = { "WeapTypeMace", "WeapTypeWarhammer" };
};

struct Consumables {
	const std::vector<std::string> vials = { "VendorItemPotion", "VendorItemPoison" };
};


std::string ReplaceFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_text)
{
	const auto settings = Settings::GetSingleton();

	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();
	const auto a_baseFormID = a_baseObject->GetFormID();

#ifdef _DEBUG
	auto clogger = RE::ConsoleLog::GetSingleton();

	clogger->Print("------------------------ Immersive Activate Debugging");
	clogger->Print("------------- Original Name Start -------------------");
	clogger->Print(SPDLOG_BUF_TO_STRING(a_text).c_str());
	clogger->Print("------------- Original Name End ---------------------");
	clogger->Print("FormID: %s", SPDLOG_BUF_TO_STRING(a_baseFormID).c_str());
	clogger->Print("FormType: %s", SPDLOG_BUF_TO_STRING(a_formType).c_str());
#endif
	SpecialForms spec;
	Weapons weap;
	Consumables cons;

	switch (a_formType) {
	case RE::FormType::NPC:
	case RE::FormType::LeveledNPC:
		if (a_object->IsDead()) {
			return ReplaceRefText(a_text, settings->npc_dead_show.text);
		}
		else if (a_object->IsAnimal()) {
			return ReplaceRefText(a_text, settings->npc_animal_show.text);
		}
		else if (a_object->IsChild()) {
			return DecideSkip(settings->npc_child_show.text, a_text);
		}
		else if (a_object->IsDragon()) {
			return ReplaceRefText(a_text, "Dragon");
		}
		return DecideSkip(settings->npc_show.text, a_text);
	case RE::FormType::Door:
		return ReplaceRefText(a_text, settings->door_show.text);
	case RE::FormType::Activator:
	case RE::FormType::Furniture:
		return a_text;
	case RE::FormType::Container:
		return ReplaceRefText(a_text, settings->container_show.text);
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		return ReplaceRefText(a_text, settings->resource_show.text);
	case RE::FormType::Ingredient:
		return ReplaceRefText(a_text, settings->ingredient_show.text);
	case RE::FormType::AlchemyItem:
		if (a_baseObject->HasAnyKeywordByEditorID(cons.vials) || a_baseFormID == spec.skooma || a_baseFormID == spec.skoomaRedWater) {
			return ReplaceRefText(a_text, "Flask");
		}
		return ReplaceRefText(a_text, settings->alchemy_item_show.text);
	case RE::FormType::Ammo:
		return ReplaceRefText(a_text, settings->ammo_show.text);
	case RE::FormType::Weapon:
		if (a_baseObject->HasAnyKeywordByEditorID(weap.blades)) {
			return ReplaceRefText(a_text, "Blade");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID(weap.axes)) {
			return ReplaceRefText(a_text, "Axe");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID(weap.bludgeons)) {
			return ReplaceRefText(a_text, "Bludgeon");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "WeapTypeStaff" })) {
			return ReplaceRefText(a_text, "Staff");
		}
		return ReplaceRefText(a_text, settings->weapon_show.text);
	case RE::FormType::Armor:
		if (a_object->IsJewelry()) {
			return ReplaceRefText(a_text, settings->jewelry_show.text);
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorHelmet" })) {
			return ReplaceRefText(a_text, "Helmet");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorBoots" })) {
			return ReplaceRefText(a_text, "Boots");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorGauntlets" })) {
			return ReplaceRefText(a_text, "Gauntlets");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorCuirass" })) {
			return ReplaceRefText(a_text, "Cuirass");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorShield" })) {
			return ReplaceRefText(a_text, "Shield");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "ArmorClothing" })) {
			return ReplaceRefText(a_text, "Clothes");
		}
		return ReplaceRefText(a_text, settings->armor_show.text);
	case RE::FormType::Scroll:
	case RE::FormType::Note:
		return ReplaceRefText(a_text, settings->scroll_note_show.text);
	case RE::FormType::Book:
		return ReplaceRefText(a_text, settings->book_show.text);
	case RE::FormType::SoulGem:
		return ReplaceRefText(a_text, settings->soulgem_show.text);
	case RE::FormType::KeyMaster:
		return ReplaceRefText(a_text, settings->key_show.text);
	default:
		if (a_baseFormID == spec.septim) {
			return ReplaceRefText(a_text, settings->money_show.text);
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemGem" })) {
			return ReplaceRefText(a_text, "Gem");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemOreIngot" })) {
			return ReplaceRefText(a_text, "Metal");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemAnimalHide" })) {
			return ReplaceRefText(a_text, "Leather");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemAnimalPart" })) {
			return ReplaceRefText(a_text, "Remains");
		}
		else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemBardInstrument" })) {
			return ReplaceRefText(a_text, "Instrument");
		}
		else if (a_object->IsLockpick()) {
			return ReplaceRefText(a_text, "Lockpick");
		}
		return ReplaceRefText(a_text, settings->item_show.text);
	}
}
