#include <iostream>
#include "Producer.h"

int main() {
    auto* p1 = new Producer(1, 5, 30);
    p1->ShowArticles();
    delete p1;
    return 0;
}
