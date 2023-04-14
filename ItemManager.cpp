#define _CRT_SECURE_NO_WARNINGS

#include "ItemManager.h"
#include <fstream>
#include <algorithm>

template<typename src_type>
void Read(src_type& src, char*& data, size_t size = sizeof(src_type)) {
	if constexpr (std::is_same_v<src_type, std::string>) {
		std::string::size_type string_size{};
		Read(string_size, data, 2);
		std::string* str = reinterpret_cast<std::string*>(&src);
		str->resize(string_size);
		std::copy_n(data, string_size, str->data());
		data += string_size;
		return;
	}

	std::copy_n(data, size, reinterpret_cast<char*>(&src));
	data += size;
}

void ItemManager::ReadItem(Item* item, char*& data) {
	Read(item->id, data);
	Read(item->editable_type, data);
	Read(item->item_category, data);
	Read(item->action_type, data);
	Read(item->hit_sound_type, data);
	Read(item->name, data);
	Read(item->texture_path, data);
	Read(item->texture_hash, data);
	Read(item->item_kind, data);
	data += 4;
	Read(item->texture_x, data);
	Read(item->texture_y, data);
	data += 2;
	Read(item->collision_type, data);
	data += 1;
	Read(item->drop_chance, data);
	Read(item->clothing_type, data);
	Read(item->rarity, data);
	data += 1;
	Read(item->unknown_file_path, data);
	data += 8;
	Read(item->pet_name, data);
	Read(item->pet_prefix, data);
	Read(item->pet_suffix, data);
	Read(item->pet_ability, data);
	data += 4;
	Read(item->seed_base_color, data);
	Read(item->seed_overlay_color, data);
	data += 4;
	Read(item->growth, data);
	data += 4;
	Read(item->extra_texture_positions1, data);
	Read(item->secondary_texture_path, data);
	Read(item->extra_texture_positions2, data);
	data += 80;
	Read(item->punch_stuff, data);
	data += 46;
	Read(item->player_sit_path, data);
	DecodeName(item);
}

void ItemManager::LoadItems(char* data, size_t data_size) {
	if (IsLoaded()) {
		return;
	}

	Read(version, data);
	if (version != 15) {
		std::cout << "Version isn't supported.\n";
		return;
	}

	int item_count;
	Read(item_count, data);
	items.resize(item_count);
	std::for_each(items.begin(), items.end(), [&](Item& item) {
		ReadItem(&item, data);
	});
}

void ItemManager::LoadItems(const std::string& items_dat_path) {
	std::ifstream ifs(items_dat_path, std::ios::ate | std::ios::binary);
	if (!ifs.is_open() || !ifs.good()) {
		return;
	}

	std::streamsize data_size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<char> items_data(data_size);
	if (!ifs.read(items_data.data(), data_size)) {
		return;
	}

	LoadItems(items_data.data(), data_size);
	ifs.close();
}

void ItemManager::LoadItems() {
	const static std::string path_to_items_dat = "\\Growtopia\\cache\\items.dat";
	
	const char* local_appdata = getenv("LOCALAPPDATA");
	if (!local_appdata) {
		return;
	}

	LoadItems(std::string{ local_appdata } + path_to_items_dat);
}

void ItemManager::DecodeName(Item* item) {
	static std::string hash_key = "PBG892FXX982ABC*";
	for (int j = 0; j < item->name.length(); j++) {
		item->name[j] = item->name[j] ^ (hash_key[(j + item->id) % 16]);
	}
}

Item* ItemManager::GetItem(int id) {
	if (!IsValidItem(id)) {
		return nullptr;
	}

	return &items.at(id);
}

std::vector<const Item*> ItemManager::GetItems(const std::string& keyword) {
	std::vector<const Item*> matched_items{};

	auto LowerString = [](std::string str) {
		std::transform(str.begin(), str.end(), str.data(), ::tolower);
		return str;
	};

	std::string item_name = LowerString(keyword);
	std::for_each(items.begin(), items.end(), [&](const Item& item) {
		if (LowerString(item.name).find(item_name) != std::string::npos) {
			matched_items.push_back(&item);
		}
		});

	return matched_items;
}
