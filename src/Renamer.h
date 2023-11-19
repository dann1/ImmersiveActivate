#pragma once
#include "Settings.h"

std::string ReplaceFormTypeText(const RE::TESObjectREFRPtr& a_object, std::string a_text, const Settings* s);

struct Weapons
{
	const std::vector<std::string> blades = { "WeapTypeSword", "WeapTypeGreatsword", "WeapTypeDagger" };
	const std::vector<std::string> axes = { "WeapTypeWarAxe", "WeapTypeBattleaxe" };
	const std::vector<std::string> bludgeons = { "WeapTypeMace", "WeapTypeWarhammer" };
	const std::vector<std::string> staffs = { "VendorItemStaff", "WeapTypeStaff" };
	const std::vector<std::string> ranged = { "WeapTypeBow" };
	const std::vector<std::string> rods = { "ccBGSSSE001_FishingPoleKW" };
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
	const std::vector<std::string> finger = { "ClothingRing" };
	const std::vector<std::string> neck = { "ClothingNecklace" };
	const std::vector<std::string> head = { "ClothingCirclet" };
};

struct Provisions
{
	const std::vector<std::string> treats = { "OCF_AlchFood_Treat", "OCF_AlchFood_Baked" };
	const std::vector<std::string> organs = { "OCF_IngrRemains_Organ" };
	const std::vector<std::string> cheeses = { "OCF_AlchFood_Cheese" };
	const std::vector<std::string> breads = { "OCF_AlchFood_Bread" };
	const std::vector<std::string> meats = { "OCF_AlchFood_Seafood", "OCF_AlchFood_Meat" };
	const std::vector<std::string> produces = { "OCF_AlchFood_Vegetable", "OCF_AlchFood_Fruit" };
};

struct Shapes
{
	const std::vector<std::string> claws = { "OCF_RelicNordic_DragonClaw" };
	const std::vector<std::string> strip = { "OCF_AnimalHideStrip" };
	const std::vector<std::string> bottles = { "OCF_VesselBottle" };
	const std::vector<std::string> bowls = { "OCF_VesselBowl" };
	const std::vector<std::string> pots = { "OCF_VesselPot" };
	const std::vector<std::string> baskets = { "OCF_VesselBasket" };
	const std::vector<std::string> buckets = { "OCF_VesselBucket" };
	const std::vector<std::string> plates = { "OCF_VesselPlate" };
	const std::vector<std::string> jars = { "OCF_VesselTankard" };
	const std::vector<std::string> jugs = { "OCF_VesselJug" };
	const std::vector<std::string> cups = { "OCF_VesselCup" };
	const std::vector<std::string> waterskins = { "OCF_VesselWaterskin" };
	const std::vector<std::string> brooms = { "OCF_ToolBroom" };
	const std::vector<std::string> shovels = { "OCF_ToolShovel" };
	const std::vector<std::string> lanterns = { "OCF_ToolLantern" };
};
struct Materials
{
	const std::vector<std::string> bone = { "OCF_IngrRemains_Bone" };
	const std::vector<std::string> shell = { "OCF_IngrRemains_Plate" };
	const std::vector<std::string> metal = { "VendorItemOreIngot" };
	const std::vector<std::string> wood = { "VendorItemFirewood" };
	const std::vector<std::string> leather = { "VendorItemAnimalHide" };
};

struct Misc
{
	const std::vector<std::string> gems = { "VendorItemGem", "VendorItemSoulGem" };
	const std::vector<std::string> remains = { "VendorItemAnimalPart" };
	const std::vector<std::string> instruments = { "VendorItemBardInstrument" };
	const std::vector<std::string> utensils = { "OCF_WeapTypeCutlery1H" };
};

struct Books
{
	const std::uint32_t bookBurnt = 0x000E3CB7;
	const std::uint32_t bookRuined = 0x000CE70B;
	const std::uint32_t bookRuined2 = 0x000E4897;

	std::array<std::uint32_t, 3> allBooks = { bookBurnt, bookRuined, bookRuined2 };

	bool contains(std::uint32_t baseFormID)
	{
		for (const auto& bookID : allBooks) {
			if (baseFormID == bookID) {
				return true;
			}
		}
		return false;
	}
};
