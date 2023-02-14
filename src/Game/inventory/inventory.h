// #pragma once

// #include <vector>
// #include <string>

// #include "../../common.h"


// namespace Inventory {
//   enum class ItemFlag: unsigned;
//   struct Item;
//   class PlayerInventory;
// };


// enum class Inventory::ItemFlag: unsigned {
//   NONE          = 0,
//   CONSUMABLE    = 1 << 0,
//   WEAPON        = 1 << 1
// };


// struct Inventory::Item {

//   private:
//     BitFlag<unsigned, ItemFlag> _flags;

//   public:
//     std::string name = "default";
//     int quantity = 0;

//     Item() { };
//     Item(ItemFlag flags) { *this->_flags.bits() = static_cast<unsigned>(flags); };
//     BitFlag<unsigned, ItemFlag> *flags() { return &this->_flags; };
// };



// class Inventory::PlayerInventory {

//   private:
//     static const int _w = 5;
//     static const int _h = 5;
//     Item _items[_w][_h];


//   public:
//     PlayerInventory();
//     void storeItem(Item item, int x, int y);
    

// };

