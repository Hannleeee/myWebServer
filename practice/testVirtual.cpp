#include <iostream>
using namespace std;

class A {
public:
    A() {
        cout << "constructing A\n";
    }

    ~A() {
        cout << "destroy A\n";
    }
};

class B : public A {
public:
    B() {
        cout << "constructing B\n";
    }

    virtual ~B() {
        cout << "destroy B\n";
    }
};

int main() {
    B *b = new B;
    delete b;
}