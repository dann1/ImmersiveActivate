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
		} else {
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

struct Weapons
{
	const std::vector<std::string> blades = { "WeapTypeSword", "WeapTypeGreatsword", "WeapTypeDagger" };
	const std::vector<std::string> axes = { "WeapTypeWarAxe", "WeapTypeBattleaxe" };
	const std::vector<std::string> bludgeons = { "WeapTypeMace", "WeapTypeWarhammer" };
	const std::vector<std::string> staffs = { "VendorItemStaff", "WeapTypeStaff" };
};

struct Armors
{
	const std::vector<std::string> head = { "ArmorHelmet" };
	const std::vector<std::string> chest = { "ArmorCuirass" };
	const std::vector<std::string> hands = { "ArmorGauntlets" };
	const std::vector<std::string> feet = { "ArmorBoots" };
	const std::vector<std::string> shield = { "ArmorShield" };
	const std::vector<std::string> cloth = { "ArmorClothing" };
};

struct Flasks
{
	const std::uint32_t skooma = 0x00057A7A;
	const std::uint32_t skoomaRedWater = 0x0201391D;
	const std::vector<std::string> flasks = { "VendorItemPotion", "VendorItemPoison" };
};

struct Jewels
{
	const std::vector<std::string> rings = { "ClothingRing" };
	const std::vector<std::string> necklaces = { "ClothingNecklace" };
	const std::vector<std::string> circlets = { "ClothingCirclet" };
};

struct Books
{
	const std::uint32_t bookBurnt = 0x000E3CB7;
};

struct Misc
{
	const std::vector<std::string> gems = { "VendorItemGem", "VendorItemSoulGem" };
	const std::vector<std::string> metals = { "VendorItemOreIngot" };
	const std::vector<std::string> leather = { "VendorItemAnimalHide" };
	const std::vector<std::string> remains = { "VendorItemAnimalPart" };
	const std::vector<std::string> instruments = { "VendorItemBardInstrument" };
};

std::string ReplaceFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_text, const Settings* settings)
{
	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();
	const auto a_baseFormID = a_baseObject->GetFormID();

#ifdef _DEBUG
	auto clogger = RE::ConsoleLog::GetSingleton();

	clogger->Print("------------------------ Immersive Activate Debugging");
	clogger->Print("FormID: %s", SPDLOG_BUF_TO_STRING(a_baseFormID).c_str());
	clogger->Print("FormType: %s", SPDLOG_BUF_TO_STRING(a_formType).c_str());
	clogger->Print("------------- Original Name Start -------------------");
	clogger->Print(SPDLOG_BUF_TO_STRING(a_text).c_str());
	clogger->Print("------------- Original Name End ---------------------");

	logger::debug("FormID: {}", a_baseFormID);
	logger::debug("FormType: {}", a_formType);
	logger::debug("------------- Original Name Start -------------------");
	logger::debug("{}", a_text);
	logger::debug("------------- Original Name End ---------------------");
#endif

	Books books;

	switch (a_formType) {
	case RE::FormType::NPC:
	case RE::FormType::LeveledNPC:
		if (a_object->IsDead()) {
			return ReplaceRefText(a_text, settings->npc_dead_show.text);
		} else if (a_object->IsAnimal()) {
			return ReplaceRefText(a_text, settings->npc_animal_show.text);
		} else if (a_object->IsChild()) {
			return DecideSkip(settings->npc_child_show.text, a_text);
		} else if (a_object->IsDragon()) {
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
		{
			Flasks flasks;

			if (a_baseObject->HasAnyKeywordByEditorID(flasks.flasks) || a_baseFormID == flasks.skooma || a_baseFormID == flasks.skoomaRedWater) {
				return ReplaceRefText(a_text, "Flask");
			}

			return ReplaceRefText(a_text, settings->alchemy_item_show.text);
		}
	case RE::FormType::Ammo:
		return ReplaceRefText(a_text, settings->ammo_show.text);
	case RE::FormType::Weapon:
		{
			Weapons weapons;

			if (a_baseObject->HasAnyKeywordByEditorID(weapons.blades)) {
				return ReplaceRefText(a_text, "Blade");
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.axes)) {
				return ReplaceRefText(a_text, "Axe");
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.staffs)) {
				return ReplaceRefText(a_text, "Staff");  // TODO: Battlestaffs as well
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.bludgeons)) {
				return ReplaceRefText(a_text, "Bludgeon");
			} else if (a_baseObject->HasAnyKeywordByEditorID({ "WeapTypeBow" })) {
				auto a_baseWeapon = a_baseObject->As<RE::TESObjectWEAP>();
				return a_baseWeapon->IsCrossbow() ? ReplaceRefText(a_text, "Crossbow") : ReplaceRefText(a_text, "Bow");
			}

			return ReplaceRefText(a_text, settings->weapon_show.text);
		}
	case RE::FormType::Armor:
		{
			if (a_object->IsJewelry()) {
				Jewels jewels;

				if (a_baseObject->HasAnyKeywordByEditorID(jewels.rings)) {
					return ReplaceRefText(a_text, "Ring");
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.necklaces)) {
					return ReplaceRefText(a_text, "Necklace");
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.circlets)) {
					return ReplaceRefText(a_text, "Circlet");
				}

				return ReplaceRefText(a_text, settings->jewelry_show.text);
			}

			Armors armors;

			if (a_baseObject->HasAnyKeywordByEditorID(armors.head)) {
				return ReplaceRefText(a_text, "Helmet");
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.feet)) {
				return ReplaceRefText(a_text, "Boots");
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.hands)) {
				return ReplaceRefText(a_text, "Gauntlets");
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.chest)) {
				return ReplaceRefText(a_text, "Cuirass");
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.shield)) {
				return ReplaceRefText(a_text, "Shield");
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.head)) {
				return ReplaceRefText(a_text, "Clothes");
			}

			return ReplaceRefText(a_text, settings->armor_show.text);
		}
	case RE::FormType::Scroll:
	case RE::FormType::Note:
		return ReplaceRefText(a_text, settings->scroll_note_show.text);
	case RE::FormType::Book:
		return ReplaceRefText(a_text, settings->book_show.text);
	case RE::FormType::KeyMaster:
		return ReplaceRefText(a_text, settings->key_show.text);
	default:
		{
			Misc misc;

			if (a_baseFormID == 0xF) {  // Septim
				return ReplaceRefText(a_text, settings->money_show.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.gems)) {
				return ReplaceRefText(a_text, settings->gem.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.metals)) {
				return ReplaceRefText(a_text, "Metal");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.remains)) {
				return ReplaceRefText(a_text, "Remains");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.leather)) {
				return ReplaceRefText(a_text, "Leather");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.instruments)) {
				return ReplaceRefText(a_text, "Instrument");
			} else if (a_object->IsLockpick()) {
				return ReplaceRefText(a_text, "Lockpick");
			} else if (a_baseFormID == books.bookBurnt) {
				return ReplaceRefText(a_text, settings->book_show.text);
			}
		}

		return ReplaceRefText(a_text, settings->item_show.text);
	}
}
