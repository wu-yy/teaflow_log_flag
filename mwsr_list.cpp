//
// Created by wuyongyu on 2020-03-21.
//

#include "mwsr_list.h"

#ifdef TEST

#include <cassert>
#include <thread>
#include <iostream>
using namespace std;
int main() {
    test(1000, 1000, 3);
    test(1000, 100000, 3);
    test(100, 10000, 8);
    test(100000, 10, 16);
    test(1000, 100000, 16);
    return 0;
}
#endif