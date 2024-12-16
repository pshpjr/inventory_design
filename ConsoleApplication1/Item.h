#pragma once
#include <unordered_map>
#include <string>

using itemID = int;

enum class ItemType {

};

//아이템 정보는 아이템 id마다 하나씩만 있어도 충분함. 
class ItemInfo {
public:
    ItemInfo(itemID id, std::string name, ItemType type, size_t maxStackSize) :_id(id),_name(name),_type(type), _maxStackSize(maxStackSize) {}

    itemID ID() const { return _id; }
    std::string Name() const { return _name; }
    ItemType Type() const { return _type; }
    int MaxStackCount() const { return _maxStackSize; }

private:
    itemID _id;
    ItemType _type;
    std::string _name;
    size_t _maxStackSize;
};

class Item {

public:

    bool operator==(const Item& other) const
    {
        return other._id == this->_id;
    }

    itemID getID() const { return _id; }
    bool stackable() const { return _stackable; }
    int maxStackSize() const { return _maxStackSize; }
private:
    itemID _id;
    bool _stackable;
    int _maxStackSize;
};

namespace std {
    template <>
    struct hash<Item> {
        std::size_t operator()(const Item& other) const {
            return other.getID();
        }
    };
}

