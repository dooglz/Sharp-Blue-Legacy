#include "Storage.h"

std::vector<Base_storage*> Base_storage::warehouses;

void Base_storage::Shutdown() {
  for (auto b : warehouses) {
    b->_Shutdown();
  }
}
