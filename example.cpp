#include "ItemManager.h"

int main() {
	if (!item_manager.get()) {
		return EXIT_FAILURE;
	}

	item_manager->LoadItems();

	for (const auto* item : item_manager->GetItems(" Lock")) {
		std::cout << "Item Found: " << item->name << std::endl;
	}

	return EXIT_SUCCESS;
}