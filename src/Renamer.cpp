#include "Renamer.h"
#include "Settings.h"

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

	Shapes shapes;

	// TODO: Make everything skippable
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
		if (a_object->NameIncludes("Ore Vein"))
			return ReplaceRefText(a_text, "Ore");
	case RE::FormType::Furniture:
		return a_text;
	case RE::FormType::Container:
		return ReplaceRefText(a_text, s->rContainer.text);
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		if (a_object->NameIncludes("Purse"))
			return ReplaceRefText(a_text, s->rMoneyPurse.text);
		return ReplaceRefText(a_text, s->rResource.text);
	case RE::FormType::Ingredient:
		return ReplaceRefText(a_text, s->rIngredient.text);
	case RE::FormType::AlchemyItem:
		{
			Flasks flasks;
			Provisions provisions;

			if (a_baseObject->HasAnyKeywordByEditorID(flasks.flasks) || a_baseObject->IsSkooma()) {
				return ReplaceRefText(a_text, s->rConsumableAlchemy.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.bottles)) {
				return ReplaceRefText(a_text, "Bottle");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.bowls)) {
				return ReplaceRefText(a_text, "Bowl");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.waterskins)) {
				return ReplaceRefText(a_text, "Waterskin");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.organs)) {
				return ReplaceRefText(a_text, "Organ");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.cheeses)) {
				return ReplaceRefText(a_text, "Cheese");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.treats)) {
				return ReplaceRefText(a_text, "Treat");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.breads)) {
				return ReplaceRefText(a_text, "Bread");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.meats)) {
				return ReplaceRefText(a_text, "Meat");
			} else if (a_baseObject->HasAnyKeywordByEditorID(provisions.produces)) {
				return ReplaceRefText(a_text, "Produce");
			}

			return ReplaceRefText(a_text, s->rConsumable.text);
		}
	case RE::FormType::Ammo:
		return ReplaceRefText(a_text, s->rAmmo.text);
	case RE::FormType::Weapon:
		{
			Weapons weapons;

			if (a_baseObject->HasAnyKeywordByEditorID(weapons.rods)) {
				return ReplaceRefText(a_text, "Rod");
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.blades)) {
				return ReplaceRefText(a_text, s->rWeaponBlade.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.axes)) {
				return ReplaceRefText(a_text, s->rWeaponAxe.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.staffs)) {
				return ReplaceRefText(a_text, s->rWeaponStaff.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.bludgeons)) {
				return ReplaceRefText(a_text, s->rWeaponBlunt.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(weapons.ranged)) {
				auto a_baseWeapon = a_baseObject->As<RE::TESObjectWEAP>();
				return ReplaceRefText(a_text, a_baseWeapon->IsCrossbow() ? s->rWeaponCrossbow.text : s->rWeaponBow.text);
			}

			return ReplaceRefText(a_text, s->rWeapon.text);
		}
	case RE::FormType::Armor:
		{
			if (a_object->IsJewelry()) {
				Jewels jewels;

				if (a_baseObject->HasAnyKeywordByEditorID(jewels.finger)) {
					return ReplaceRefText(a_text, s->rJewelFinger.text);
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.neck)) {
					return ReplaceRefText(a_text, s->rJewelNeck.text);
				} else if (a_baseObject->HasAnyKeywordByEditorID(jewels.head)) {
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
		// TODO: Notes have the Type: Book/Tome set in the Form record. Workarounds
		// YNAM - ITMNoteUp
		// INAM - HighPolynote - unreliable
		// MODL - Note.nif - unreliable
		return ReplaceRefText(a_text, s->rPaper.text);
	case RE::FormType::Book:
		return ReplaceRefText(a_text, s->rBook.text);
	case RE::FormType::KeyMaster:
		return ReplaceRefText(a_text, s->rKey.text);
	default:
		{
			Books books;
			Materials materials;
			Misc misc;

			if (a_baseObject->IsGold()) {
				return ReplaceRefText(a_text, s->rMoney.text);
			} else if (a_baseObject->IsLockpick()) {
				return a_text;
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.bottles)) {
				return ReplaceRefText(a_text, "Bottle");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.pots)) {
				return ReplaceRefText(a_text, "Pot");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.baskets)) {
				return ReplaceRefText(a_text, "Basket");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.buckets)) {
				return ReplaceRefText(a_text, "Bucket");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.plates)) {
				return ReplaceRefText(a_text, "Plate");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.bowls)) {
				return ReplaceRefText(a_text, "Bowl");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.jars)) {
				return ReplaceRefText(a_text, "Jar");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.cups)) {
				return ReplaceRefText(a_text, "Cup");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.jugs)) {
				return ReplaceRefText(a_text, "Jug");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.brooms)) {
				return ReplaceRefText(a_text, "Broom");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.shovels)) {
				return ReplaceRefText(a_text, "Shovel");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.lanterns)) {
				return ReplaceRefText(a_text, "Lantern");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.waterskins)) {
				return ReplaceRefText(a_text, "Waterskin");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.strip)) {
				return ReplaceRefText(a_text, "Strip");
			} else if (a_baseObject->HasAnyKeywordByEditorID(shapes.claws)) {
				return ReplaceRefText(a_text, "Claw");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.utensils)) {
				return ReplaceRefText(a_text, "Utensil");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.gems)) {
				return ReplaceRefText(a_text, s->rMiscGem.text);
			} else if (books.contains(a_baseFormID)) {
				return ReplaceRefText(a_text, s->rBook.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.instruments)) {
				return ReplaceRefText(a_text, s->rMiscBard.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(materials.wood)) {
				return ReplaceRefText(a_text, s->rMiscWood.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(materials.metal)) {
				return ReplaceRefText(a_text, s->rMiscOre.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(materials.bone)) {
				return ReplaceRefText(a_text, "Bone");
			} else if (a_baseObject->HasAnyKeywordByEditorID(materials.leather)) {
				return ReplaceRefText(a_text, s->rMiscSkin.text);
			} else if (a_baseObject->HasAnyKeywordByEditorID(materials.shell)) {
				return ReplaceRefText(a_text, "Shell");
			} else if (a_baseObject->HasAnyKeywordByEditorID(misc.remains)) {
				return ReplaceRefText(a_text, s->rMiscRemain.text);
			}
		}

		return ReplaceRefText(a_text, s->rMisc.text);
	}
}
