#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <math.h>
#include <set>
#include <sstream>
#include <unordered_map>
using namespace std;

/*
1단계: 기본 인벤토리 및 아이템 관리

고정된 개수의 슬롯(예: 10칸)을 가진 인벤토리.
각 슬롯에는 하나의 아이템을 보관하거나 비울 수 있다.
아이템은 단순히 id, 이름, 타입(기본) 정도의 최소 정보만을 가진다.
인벤토리에 아이템 추가/제거, 슬롯 조회 기능 구현.
2단계: 스택 기능 추가

동일한 아이템은 한 슬롯에 스택 가능(예: 포션 n개).
슬롯별 최대 스택 가능 개수 지정.
추가 시 가능한 슬롯(이미 해당 아이템이 있는 슬롯, 혹은 빈 슬롯) 확인 로직.
제거 시 스택 감소, 0개면 해당 슬롯 비움.
3단계: 아이템 타입 및 전용 슬롯 제한

아이템 타입 예: 무기, 방어구, 소비 아이템, 재료 아이템 등.
인벤토리 일부 슬롯은 특정 타입 아이템만 허용(예: 1~2번 슬롯은 무기 전용).
추가 시 타입 체크 로직으로 인해 슬롯 선택 알고리즘 복잡해짐.
4단계: 정렬, 검색 기능 제공

인벤토리 내 아이템을 이름 순, 타입 우선 순위 등 기준으로 정렬하는 기능.
id나 이름으로 아이템 검색 기능 제공.
동일 아이템 다수 존재 시, 첫 번째 슬롯 반환 또는 모든 슬롯 인덱스 반환 등의 모드 지원.
5단계: 인벤토리 확장 및 축소

인벤토리 슬롯 수 동적 변경 가능(확장권 사용 등).
확장 시 새로운 슬롯 추가, 축소 시 아이템 재배치 혹은 삭제 처리 필요.
인벤토리 변경 시 이벤트(예: 콜백)를 발생시켜 외부 모듈에 알려줄 수 있는 구조 고려.
6단계: 장비 슬롯과 연계

캐릭터 장비 슬롯(무기 슬롯, 방어구 슬롯) 추가.
인벤토리 아이템 중 장비 타입을 장착 슬롯에 장착/해제 가능.
장착 조건(레벨 제한, 직업 제한 등) 검사.
장비 교체 시 스텟 변동 등을 반영하는 인터페이스 준비(구현은 단순화).
7단계: 아이템 사용/소비 로직

소비 아이템(포션 사용 등) 사용 시 수량 감소.
사용 조건(쿨다운, 퀘스트 상태, 지역 제한) 체크.
사용 시 효과(체력 회복 등)는 단순한 콜백으로 처리, 상세 구현은 나중에.
8단계: NPC 상점 연계(구매/판매)

NPC 상점과의 인터페이스 추가.
플레이어 인벤토리에서 아이템을 골드(또는 게임 내 화폐)로 판매할 수 있다.
상점 목록에서 아이템을 구매하여 인벤토리에 추가 가능.
상점 품목 및 재고 관리, 구매 조건(플레이어 화폐 부족, 아이템 재고 없음) 검사.
9단계: 플레이어 간 거래 시스템

두 플레이어 인벤토리 간 아이템을 교환하는 기능.
거래 창을 열고, 각자 아이템 및 화폐를 올려놓은 후 상호 확인 시 거래 완료.
거래 취소 시 아이템/화폐 롤백.
거래 불가능한 아이템(계정 귀속, 퀘스트 아이템 등)은 거래창에 올리려 할 때 거부.


*/


using itemID = int;

class Item {

public:
    Item(itemID id, bool stackable, size_t maxStackSize):_id(id),_stackable(stackable),_maxStackSize(maxStackSize) {}
    ~Item() = default;
    Item(const Item& other) = default;
    Item(Item&& other) = default;
    Item& operator=(const Item& other) = default;
    Item& operator=(Item&& other) = default;
    


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

class inventory
{
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

        _items.emplace_back( addItem,1 );

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

    int size() const { return _items.size();}

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

int main() {
    inventory i(10);
    i.addItem({ 0,true,3 });
    i.addItem({ 0,true,3 });
    i.addItem({ 0,true,3 });

    i.removeItem({ 0,true,3 });
    i.removeItem({ 0,true,3 });
    i.removeItem({ 0,true,3 });

    cout << i.hasItem({ 0,true,3 });
    cout << i.size();


    return 0;
}