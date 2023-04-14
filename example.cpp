#include "ItemManager.h"

int main() {
	if (!item_manager.get()) {
		return EXIT_FAILURE;
	}

	item_manager->LoadItems();

	int start_item = 13100;
	std::cout << "Scanning Items from: " << start_item << std::endl; 
	for (const auto* item : item_manager->GetItems(start_item)) {
		std::cout << "Potential New Item: " << item->name << std::endl;
	}

	return EXIT_SUCCESS;
}
