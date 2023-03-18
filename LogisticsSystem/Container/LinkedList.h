#pragma once
#ifndef LinkedList_h__
#define LinkedList_h__
#include "iterable.h"
#include <assert.h>
#include<type_traits>
#include<functional>

template<typename T> class LinkedList;

template<typename T>
class LinkedList_Iterator : public Iterator<LinkedList_Iterator<T>, T>
{
private:
    using Node = LinkedList<T>::template Node;
    Node* pointer;
    LinkedList_Iterator(Node* val) : pointer(val) {}
public:
    LinkedList_Iterator():pointer(nullptr) {}
    LinkedList_Iterator(const LinkedList_Iterator& val) noexcept : pointer(val.pointer) {}

    /*
    * 迭代器后移函数, 不返回迭代器, 效率比运算符++更高, 主要用于内部调用
    * 一般情况下, 编译器编译时会将其内联, 从而消除调用函数的开销
    */
    void _setSelfNext() {
        pointer = pointer->next;
    }

    void _setSelfPrevious() {
        pointer = pointer->pre;
    }

    /*
    * 获取指向下一个元素的迭代器
    * 迭代器
    */
    LinkedList_Iterator next() const {
        return  LinkedList_Iterator(pointer->next);
    }

    /*
    * 判断是否有下一个节点, last 不视为有效的节点
    */
    bool hasNext() const {
        return pointer!=nullptr && pointer->next != nullptr && pointer->next->next != nullptr;
    }

    bool hasPrevious() const {
        return pointer!=nullptr && pointer->pre != nullptr;
    }

    /*
    * 使迭代器指向前一个元素,前置--
    */
    LinkedList_Iterator& operator--() {
        _setSelfPrevious();
        return *this;
    }

    /*
    * 使迭代器指向前一个元素,后置--
    */
    LinkedList_Iterator operator--(int) {
        LinkedList_Iterator oldIter(*this);
        _setSelfPrevious();
        return oldIter;
    }

    /*
    * 使迭代器指向后一个元素,前置++
    */
    LinkedList_Iterator& operator++() override {
        _setSelfNext();
        return *this;
    }

    /*
    * 使迭代器指向后一个元素,后置++
    */
    LinkedList_Iterator operator++(int) override {
        LinkedList_Iterator oldIter(*this);
        _setSelfNext();
        return oldIter;
    }

    /*
    * 赋值拷贝函数
    */
    LinkedList_Iterator& operator=(const LinkedList_Iterator& rgt) override {
        pointer = ((const LinkedList_Iterator&)rgt).pointer;
        return *this;
    }


    bool operator==(const LinkedList_Iterator& rgt) const override {
        return pointer == ((const LinkedList_Iterator&)rgt).pointer;
    }

    bool operator!=(const LinkedList_Iterator& rgt) const override {
        return pointer != ((const LinkedList_Iterator&)rgt).pointer;
    }


    /*
    * 获取迭代器指向的元素
    */
    T& operator*() const override {
        return *pointer->elem;
    }

    T* operator->() const override {
        return pointer->elem;
    }

    friend class LinkedList<T>;
};

template<typename T>
class LinkedList : Iterable<LinkedList_Iterator<T>>
{
public:
    using Iterator = LinkedList_Iterator<T>;
    friend class LinkedList_Iterator<T>;
    template<typename K, typename V> friend class HashMap;
private:
    struct Node
    {
        T* elem;
        Node* pre;
        Node* next;
    };
    Node* head;
    Node* last;
    int _size;
    /*
    * 在迭代器指向的位置前面插入一个节点, 并更新此迭代器使其指向刚插入的新节点
    * 指向头节点时则在链表头节点前面添加一个新头, 并更新 head 节点
    * size+1
    */
    template<typename VT, typename Iter_t>
    void _insert(Iter_t&& iter, VT&& val) {
        Node* node = new Node{ new T(std::forward<VT>(val)), iter.pointer->pre,iter.pointer };
        if (iter.pointer == head) {
            head = node;
        } else {
            iter.pointer->pre->next = node;
        }
        iter.pointer->pre = node;
        iter.pointer = node;
        ++_size;
    }

    /*
    * 在链表尾部添加节点 (填充 last 节点并创建一个新的 last 节点)
    * 注意: last 节点的 elem 变量永远为null
    */
    template<typename VT>
    void _pushBack(VT&& val) {
        last->elem = new T(std::forward<VT>(val));
        last->next = new Node{ nullptr,last,nullptr };
        last = last->next;
        ++_size;
    }

    /*
    * 删除迭代器pos所指向的节点, 保持被删节点前后的连接, 并使size-1
    * pos 不能指向last
    */
    template<typename Iter_t>
    void _deleteElem(Iter_t&& pos) {
        assert(pos.pointer != last);
        delete pos.pointer->elem;
        Node* nextPtr = pos.pointer->next;
        Node* prePtr = pos.pointer->pre;
        nextPtr->pre = prePtr;
        if (pos.pointer == head) {
            head = nextPtr;
        } else { //如果待删节点不是头, 则prePtr一定不是null, 否则一定为null
            prePtr->next = nextPtr;
        }
        delete pos.pointer;
        --_size;
    }

    void _removeAll() {
        //链表被移动时, head==last==nullptr, 不会进入该循环
        for (Node* node = head, *next; node != last; node = next) {
            delete node->elem;
            next = node->next;
            delete node;
        }
        last->pre = nullptr;
        head = last;
        _size = 0;
    }


public:
    /*
    * class Iterator 链表的迭代器
    * 用于遍历链表和操作元素
    * Node* pointer永不为nullptr, 只能指向链表的某一元素(end算作一个元素)
    */

    LinkedList() :head(new Node{ nullptr,nullptr,nullptr }), last(head), _size(0) {}

    LinkedList(const LinkedList& val) : head(new Node{ nullptr,nullptr,nullptr }), last(head) {
        for (T& elem : val) {
            pushBack(elem);
        }
        _size = val._size;
    }

    LinkedList(LinkedList&& val) : head(val.head), last(val.last), _size(val._size) {
        val.head = nullptr;
        val.last = nullptr;
        val._size = 0;
    }

    LinkedList& operator=(const LinkedList& rgt) {
        for (T& elem : rgt) {
            pushBack(elem);
        }
        _size = rgt._size;
        return *this;
    }

    LinkedList& operator=(const LinkedList&& rgt) {
        head = rgt.head;
        last = rgt.last;
        _size = rgt._size;
        rgt.head = nullptr;
        rgt.last = nullptr;
        rgt._size = 0;
        return *this;
    }


    LinkedList duplicate(LinkedList& src) {
        return LinkedList(src);
    }

    /*
    * 获取指向head的迭代器
    */
    Iterator begin() const {
        return Iterator(head);
    }

    /*
    * 获取指向last的迭代器
    * 注意: last 的elem永远为null
    */
    Iterator end() const {
        return Iterator(last);
    }

    /*
    * 移除迭代器所指的节点
    * 调用左值重载函数时, 迭代器自动后移
    */
    void remove(Iterator& pos) {
        Node* nextPtr = pos.pointer->next;
        _deleteElem(pos);
        pos.pointer = nextPtr;
    }

    /*
    * 移除迭代器所指的节点
    * 调用右值重载函数时, 迭代器不自动后移(作为右值的迭代器应当为一次性的或者是将要丢弃的)
    */
    void remove(Iterator&& pos) {
        _deleteElem(std::move(pos));
    }

    /*
    * void removeIf(Func_t&& conditionFunc)
    * 允许用户以lambda等方式传入一个条件函数, 对每个节点遍历, 返回true则删除该节点
    */
    template<typename Func_t>
    void removeIf(Func_t&& conditionFunc) {
        for (Iterator i = begin(); i != end();) {
            if (conditionFunc(i)) {
                remove(i);
                continue;
            }
            i._setSelfNext();
        }
    }

    void insert(Iterator& iter, T& val) {
        _insert(iter, val);
    }

    void insert(Iterator& iter, T&& val) {
        _insert(iter, std::move(val));
    }

    void insert(Iterator&& iter, T&& val) {
        _insert(std::move(iter), std::move(val));
    }

    void insert(Iterator&& iter, T& val) {
        _insert(std::move(iter), val);
    }

    void pushBack(T& val) {
        _pushBack(val);
    }

    void pushBack(T&& val) {
        _pushBack(std::move(val));
    }

    int size() const {
        return _size;
    }

    /*
    * 清空链表中的所有元素, 并创建last节点(效果相当于恢复链表到刚初始化后)
    */
    void clear() {
        _removeAll();
    }

    /*
    * 析构函数
    */
    ~LinkedList() {
        _removeAll();
        delete head;
    }
};

#endif // LinkedList_h__