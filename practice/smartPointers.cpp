#include <iostream>
#include <mutex>

template<typename T>
class mySharedPtr {
public:
    mySharedPtr(T* p = nullptr) : ptr(p), refCount(new size_t(1)), mtx(new std::mutex) {}

    ~mySharedPtr() {
        release();
    }

    mySharedPtr(const mySharedPtr &other) : ptr(other.ptr), refCount(other.refCount), mtx(other.mtx) {
        addRefCount();
    }

    mySharedPtr<T> &operator=(const mySharedPtr &other) {
        if (other.ptr != ptr) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            mtx = other.mtx;
            addRefCount();
        }
        return *this;
    }

    T &operator*() {
        return *ptr;
    }

    T *operator->() {
        return ptr;
    }

    int useCount() { return *refCount; }
    void addRefCount() {
        mtx->lock();
        ++(*refCount);
        mtx->unlock();
    }

private:
    void release() {
        bool deleteFlag = false;
        mtx->lock();
        if (--(*refCount) == 0) {
            delete refCount;
            delete ptr;
            deleteFlag = true;
        }
        mtx->unlock();
        if (deleteFlag) delete mtx;
    }
    T *ptr;
    size_t *refCount;
    std::mutex *mtx;
};

template<typename T>
class myUniquePtr {
public:
    
private:
    T *ptr;
};

struct ListNode {
    int val;
    mySharedPtr<ListNode> prev;
    mySharedPtr<ListNode> next;
    ~ListNode() {
        std::cout << "Deconstructing...\n";
    }
};

int main() {
    mySharedPtr<ListNode> node1(new ListNode());
    mySharedPtr<ListNode> node2(new ListNode());

    std::cout << node1.useCount() << std::endl;
    std::cout << node2.useCount() << std::endl;

    node1->next = node2;
    node2->prev = node1;

    std::cout << node1.useCount() << std::endl;
    std::cout << node2.useCount() << std::endl;
}