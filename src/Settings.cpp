#include "Settings.h"

bool Settings::LoadSettings()
{
	CSimpleIniA ini;

	constexpr auto path = L"Data/SKSE/Plugins/ImmersiveActivate.ini";
	ini.SetUnicode();
	ini.LoadFile(path);

	const auto get_value = [&]<class T>(T& a_value, const char* a_section, const char* a_key) {
		if constexpr (std::is_same_v<bool, T>) {
			a_value = ini.GetBoolValue(a_section, a_key, a_value);
		} else {
			a_value = ini.GetValue(a_section, a_key, a_value.c_str());
		}
	};

	const char* section = "Hide All Text";

	get_value(npc.hideAll, section, npc.type.c_str());
	get_value(activators.hideAll, section, activators.type.c_str());
	get_value(containers.hideAll, section, containers.type.c_str());
	get_value(doors.hideAll, section, doors.type.c_str());
	get_value(flora.hideAll, section, flora.type.c_str());
	get_value(furniture.hideAll, section, furniture.type.c_str());
	get_value(items.hideAll, section, items.type.c_str());
	get_value(projectiles.hideAll, section, projectiles.type.c_str());

	section = "Hide Button";

	get_value(npc.hideButton, section, npc.type.c_str());
	get_value(activators.hideButton, section, activators.type.c_str());
	get_value(containers.hideButton, section, containers.type.c_str());
	get_value(doors.hideButton, section, doors.type.c_str());
	get_value(flora.hideButton, section, flora.type.c_str());
	get_value(furniture.hideButton, section, furniture.type.c_str());
	get_value(items.hideButton, section, items.type.c_str());
	get_value(projectiles.hideButton, section, projectiles.type.c_str());

	section = "Hide Text";

	get_value(npc.hideText, section, npc.type.c_str());
	get_value(activators.hideText, section, activators.type.c_str());
	get_value(containers.hideText, section, containers.type.c_str());
	get_value(doors.hideText, section, doors.type.c_str());
	get_value(flora.hideText, section, flora.type.c_str());
	get_value(furniture.hideText, section, furniture.type.c_str());
	get_value(items.hideText, section, items.type.c_str());
	get_value(projectiles.hideText, section, projectiles.type.c_str());

	section = "Steal/Pickpocket";

	get_value(steal.useColoredName, section, "Show Indicator Using Name");
	get_value(steal.nameColor, section, "Custom Indicator Color");

	section = "Owned";

	get_value(owned.useColoredName, section, "Show Indicator Using Name");
	get_value(owned.nameColor, section, "Custom Indicator Color");

	section = "Locked";

	get_value(locked.hideTag, section, "Hide Locked Tag");
	get_value(locked.tag, section, "Custom Locked Tag");
	get_value(locked.color.useColoredName, section, "Show Indicator Using Name");
	get_value(locked.color.nameColor, section, "Custom Indicator Color");

	section = "Empty";

	get_value(empty.hideTag, section, "Hide Empty Tag");
	get_value(empty.tag, section, "Custom Empty Tag");
	get_value(empty.color.useColoredName, section, "Show Indicator Using Name");
	get_value(empty.color.nameColor, section, "Custom Indicator Color");

	section = "Display Replacers";

	get_value(rAmmo.text, section, "Ammo");
	get_value(rArmor.text, section, "Armor");
	get_value(rArmorChest.text, section, "Cuirasses");
	get_value(rArmorCloth.text, section, "Clothing");
	get_value(rArmorFeet.text, section, "Boots");
	get_value(rArmorHands.text, section, "Gauntlets");
	get_value(rArmorHead.text, section, "Helmets");
	get_value(rArmorShield.text, section, "Shields");
	get_value(rBook.text, section, "Books");
	get_value(rConsumable.text, section, "Consumables");
	get_value(rConsumableAlchemy.text, section, "Flasks");
	get_value(rContainer.text, section, "Containers");
	get_value(rDoor.text, section, "Doors");
	get_value(rIngredient.text, section, "Ingredients");
	get_value(rJewel.text, section, "Jewelry");
	get_value(rJewelFinger.text, section, "Rings");
	get_value(rJewelHead.text, section, "Circlets");
	get_value(rJewelNeck.text, section, "Necklaces");
	get_value(rKey.text, section, "Keys");
	get_value(rMisc.text, section, "MiscItems");
	get_value(rMiscBard.text, section, "Instruments");
	get_value(rMiscGem.text, section, "Gems");
	get_value(rMiscLockpick.text, section, "Lockpicks");
	get_value(rMiscOre.text, section, "Metals");
	get_value(rMiscRemain.text, section, "Remains");
	get_value(rMiscSkin.text, section, "Skins");
	get_value(rMoney.text, section, "Septims");
	get_value(rNPC.text, section, "NPCs");
	get_value(rNPCAnimal.text, section, "NPCsAnimals");
	get_value(rNPCChild.text, section, "NPCsChildren");
	get_value(rNPCCorpse.text, section, "NPCsCorpses");
	get_value(rNPCDragon.text, section, "NPCsDragons");
	get_value(rPaper.text, section, "Papers");
	get_value(rResource.text, section, "Resources");
	get_value(rWeapon.text, section, "Weapons");
	get_value(rWeaponAxe.text, section, "WeaponsAxe");
	get_value(rWeaponBlade.text, section, "WeaponsBlade");
	get_value(rWeaponBlunt.text, section, "WeaponsBlunt");
	get_value(rWeaponBow.text, section, "WeaponsBow");
	get_value(rWeaponCrossbow.text, section, "WeaponsCrossBow");
	get_value(rWeaponStaff.text, section, "WeaponsStaff");

	(void)ini.SaveFile(path);

	return true;
}

const Settings::Text* Settings::GetText(const RE::FormType a_formType) const
{
	switch (a_formType) {
	case RE::FormType::Activator:
		return &activators;
	case RE::FormType::Container:
		return &containers;
	case RE::FormType::NPC:
		return &npc;
	case RE::FormType::Door:
		return &doors;
	case RE::FormType::Furniture:
		return &furniture;
	case RE::FormType::Projectile:
		return &projectiles;
	case RE::FormType::Flora:
	case RE::FormType::Tree:
		return &flora;
	default:
		return &items;
	}
}

const Settings::Text* Settings::GetText(const RE::TESObjectREFRPtr& a_object) const
{
	const auto base = a_object->GetBaseObject();
	return base ? GetText(base->GetFormType()) : nullptr;
}

const Settings::Color* Settings::GetColor(const RE::TESObjectREFRPtr& a_object, std::string_view a_text) const
{
	if (detail::is_owned(a_object)) {
		return &owned;
	}
	if (a_text.contains("#FF0000"sv)) {
		return &steal;
	}
	if (detail::is_empty(a_object)) {
		return &empty.color;
	}
	if (a_object->IsLocked()) {
		return &locked.color;
	}

	return nullptr;
}

const Settings::Tag* Settings::GetTag(const RE::TESObjectREFRPtr& a_object) const
{
	if (detail::is_empty(a_object)) {
		return &empty;
	}
	if (a_object->IsLocked()) {
		return &locked;
	}
	return nullptr;
}
