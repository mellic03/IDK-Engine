// #include "inventory.h"


// using namespace Inventory;



// PlayerInventory::PlayerInventory()
// {
//   for (int i=0; i<this->_w; i++)
//   {
//     for (int j=0; j<this->_h; j++)
//     {
//       this->_items[i][j] = Item(ItemFlag::NONE);
//     }
//   }
// }



// void PlayerInventory::storeItem(Item item, int x, int y)
// {
//   if (x >= 5 || y >= 5)
//   {
//     printf("[Inventory::PlayerInventory::storeItem()] Index out of bounds\n");
//     exit(1);
//   }


//   if (this->_items[x][y].flags()->get() == ItemFlag::NONE)
//   {
//     this->_items[x][y] = item;
//   }

//   else
//   {
//     printf("[Inventory::PlayerInventory::storeItem()] Item already exists at index [%d][%d]\n", x, y);
//     exit(1);
//   }

// }
