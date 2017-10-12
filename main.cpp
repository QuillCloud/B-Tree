#include <algorithm>
#include <iostream>
#include <iterator>

#include "btree.h"

void foo(const btree<int> &b) {
    std::copy(b.begin(), b.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

int main(void) {
    btree<int> b;

    b.insert(1);
    b.insert(10);
    b.insert(3);
    b.insert(4);
    const btree<int> c(b);
    for(btree<int>::iterator iter = b.begin(); iter != b.end(); ++iter)
        std::cout << *iter << std::endl;
    btree<int>::iterator it1 = b.begin();
    btree<int>::const_iterator it2 = b.begin();
    if (it1 == it2)
        std::cout << "ok1" << std::endl;
    ++it2;
    if (it1 != it2)
        std::cout << "ok2" << std::endl;
    btree<int>::reverse_iterator it3 = b.rbegin();
    btree<int>::const_reverse_iterator it4 = b.rbegin();
    if (it3 == it4)
        std::cout << "ok3" << std::endl;
    ++it4;
    if (it3 != it4)
        std::cout << "ok4" << std::endl;
    return 0;
}
