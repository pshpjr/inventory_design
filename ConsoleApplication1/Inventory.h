#pragma once
#include "Item.h"
class Inventory
{
    class itemSlot {

    };

public:
    inventory(size_t size) :max_item_size(size) { }
    ~inventory() = default;

    //삽입 성공 여부를 리턴
    bool addItem(Item addItem)
    {
        bool find = false;
        for (int i = 0; i < _items.size(); i++)
        {
            if (_items[i].first != addItem)
            {
                continue;
            }

            if (canStackToSlot(i))
            {
                find = true;
                _items[i].second++;
                break;
            }
        }

        if (find)
            return true;

        if (full())
            return false;

        _items.emplace_back(addItem, 1);

        return true;
    }

    //삭제 성공 여부를 리턴.
    bool removeItem(Item delItem)
    {
        if (_items.size() == 0)
            return false;

        auto it = std::find_if(_items.begin(), _items.end(),
            [&delItem](auto data)
            {
                return data.first == delItem;
            });

        if (it == _items.end())
            return false;

        if (it->second > 1)
        {
            --it->second;
        }
        else
        {
            _items.erase(it);
        }
        return true;
    }

    bool hasItem(Item target) const
    {
        return std::find_if(_items.begin(), _items.end(),
            [&target](auto data)
            {
                return data.first == target;
            }) != _items.end();
    }

    bool full() const
    {
        return _items.size() == max_item_size;
    }

    int size() const { return _items.size(); }

private:
    bool canStackToSlot(int slotIndex)
    {
        return _items[slotIndex].first.stackable() && !slotIsFull(slotIndex);
    }

    bool slotIsFull(int slotIndex)
    {
        return _items[slotIndex].second >= _items[slotIndex].first.maxStackSize();
    }


    int max_item_size{};
    //item,count
    vector<pair<Item, int>> _items;
};

