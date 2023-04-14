#include <iostream>
#include <vector>
#include <memory>

struct Item {
	uint32_t id;
	uint8_t editable_type;
	uint8_t item_category;
	uint8_t action_type;
	uint8_t hit_sound_type;
	uint8_t item_kind;
	uint8_t collision_type;
	uint8_t clothing_type;
	uint16_t rarity;
	uint32_t unk_1;
	uint16_t unk_2;
	uint32_t seed_base_color;
	uint32_t seed_overlay_color;
	uint32_t growth;
	float drop_chance;
	uint32_t texture_hash;
	uint8_t texture_x;
	uint8_t texture_y;

	std::string name;
	std::string pet_name;
	std::string pet_prefix;
	std::string pet_suffix;
	std::string pet_ability;
	std::string texture_path;
	std::string unknown_file_path;
	std::string extra_texture_positions1;
	std::string extra_texture_positions2;
	std::string secondary_texture_path;
	std::string player_sit_path;
	std::string punch_stuff;
};

struct ItemManager {

	std::uint16_t version;

	void ReadItem(Item* item, char*& data);
	void LoadItems(char* data, size_t data_size);
	void LoadItems(const std::string& items_dat_path);
	void LoadItems();

	void DecodeName(Item* item);

	inline bool IsLoaded() const { return !items.empty(); }
	inline bool IsValidItem(int id) const { return id >= 0 && id < items.size(); }

	Item* GetItem(int id);
	std::vector<const Item*> GetItems(const std::string& keyword);
	std::vector<const Item*> GetItems(int minimum_id);

private:
	std::vector<Item> items;
};

inline std::unique_ptr<ItemManager> item_manager = std::make_unique<ItemManager>();
