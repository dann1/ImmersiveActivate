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

std::string ReplaceFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_text, const Settings* s)
{
	const auto a_baseObject = a_object->GetBaseObject();
	const auto a_formType = a_baseObject->GetFormType();
	const auto a_baseFormID = a_baseObject->GetFormID();

#ifdef _DEBUG
	auto clogger = RE::ConsoleLog::GetSingleton();

	clogger->Print("------------- Immersive Activate Debugging ----------");
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
			return ReplaceRefText(a_text, s->rNPCCorpse.text);
		} else if (a_object->IsAnimal()) {
			return ReplaceRefText(a_text, s->rNPCAnimal.text);
		} else if (a_object->IsChild()) {
			return DecideSkip(s->rNPCChild.text, a_text);
		} else if (a_object->IsDragon()) {
			return ReplaceRefText(a_text, s->rNPCDragon.text);
		}
		return DecideSkip(s->rNPC.text, a_text);
	case RE::FormType::Door:
		return ReplaceRefText(a_text, s->rDoor.text);
	case RE::FormType::Activator:
	case RE::FormType::Furniture:
		return a_text;
	case RE::FormType::Container:
		return ReplaceRefText(a_text, s->rContainer.text);
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		if (a_object->NameIncludes("Purse")) return ReplaceRefText(a_text, s->rMoneyPurse.text);

		return ReplaceRefText(a_text, s->rResource.text);
	case RE::FormType::Ingredient:
		return ReplaceRefText(a_text, s->rIngredient.text);
	case RE::FormType::AlchemyItem:
		{
			Flasks flasks;

			if (a_baseObject->HasAnyKeywordByEditorID(flasks.flasks) || a_baseObject->IsSkooma()) {
				return ReplaceRefText(a_text, s->rConsumableAlchemy.text);
			}

			return ReplaceRefText(a_text, s->rConsumable.text);
		}
	case RE::FormType::Ammo:
		return ReplaceRefText(a_text, s->rAmmo.text);
	case RE::FormType::Weapon:
		{
			Weapons weapons;

			if (a_baseObject->HasAnyKeywordByEditorID(weapons.blades)) {
				return ReplaceRefText(a_text, s->rWeaponBlade.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.axes)) {
				return ReplaceRefText(a_text, s->rWeaponAxe.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.staffs)) {
				return ReplaceRefText(a_text, s->rWeaponStaff.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.bludgeons)) {
				return ReplaceRefText(a_text, s->rWeaponBlunt.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID({ "WeapTypeBow" })) {
				auto a_baseWeapon = a_baseObject->As<RE::TESObjectWEAP>();
				return ReplaceRefText(a_text, a_baseWeapon->IsCrossbow() ? s->rWeaponCrossbow.text : s->rWeaponBow.text);
			}

			return ReplaceRefText(a_text, s->rWeapon.text);
		}
	case RE::FormType::Armor:
		{
			if (a_object->IsJewelry()) {
				Jewels jewels;

				if (a_baseObject->HasAnyKeywordByEditorID(jewels.rings)) {
					return ReplaceRefText(a_text, s->rJewelFinger.text);
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.necklaces)) {
					return ReplaceRefText(a_text, s->rJewelNeck.text);
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.circlets)) {
					return ReplaceRefText(a_text, s->rJewelHead.text);
				}

				return ReplaceRefText(a_text, s->rJewel.text);
			}

			Armors armors;

			if (a_baseObject->HasAnyKeywordByEditorID(armors.head)) {
				return ReplaceRefText(a_text, s->rArmorHead.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.feet)) {
				return ReplaceRefText(a_text, s->rArmorFeet.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.hands)) {
				return ReplaceRefText(a_text, s->rArmorHands.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.chest)) {
				return ReplaceRefText(a_text, s->rArmorChest.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.shield)) {
				return ReplaceRefText(a_text, s->rArmorShield.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(armors.cloth)) {
				return ReplaceRefText(a_text, s->rArmorCloth.text);
			}

			return ReplaceRefText(a_text, s->rArmor.text);
		}
	case RE::FormType::Scroll:
	case RE::FormType::Note:
		return ReplaceRefText(a_text, s->rPaper.text);
	case RE::FormType::Book:
		return ReplaceRefText(a_text, s->rBook.text);
	case RE::FormType::KeyMaster:
		return ReplaceRefText(a_text, s->rKey.text);
	default:
		{
			Misc misc;

			if (a_baseObject->IsGold()) {
				return ReplaceRefText(a_text, s->rMoney.text);
			} else if (a_baseObject->IsLockpick()) {
				return a_text;
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.gems)) {
				return ReplaceRefText(a_text, s->rMiscGem.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.metals)) {
				return ReplaceRefText(a_text, s->rMiscOre.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.remains)) {
				return ReplaceRefText(a_text, s->rMiscRemain.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.leather)) {
				return ReplaceRefText(a_text, s->rMiscSkin.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID({ "VendorItemFirewood" })) {
				return ReplaceRefText(a_text, s->rMiscWood.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.instruments)) {
				return ReplaceRefText(a_text, s->rMiscBard.text);
			} else if (a_baseFormID == books.bookBurnt) {
				return ReplaceRefText(a_text, s->rBook.text);
			}
		}

		return ReplaceRefText(a_text, s->rMisc.text);
	}
}
