#include <iostream>
using namespace std;





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