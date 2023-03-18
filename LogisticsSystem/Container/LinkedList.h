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
    * ���������ƺ���, �����ص�����, Ч�ʱ������++����, ��Ҫ�����ڲ�����
    * һ�������, ����������ʱ�Ὣ������, �Ӷ��������ú����Ŀ���
    */
    void _setSelfNext() {
        pointer = pointer->next;
    }

    void _setSelfPrevious() {
        pointer = pointer->pre;
    }

    /*
    * ��ȡָ����һ��Ԫ�صĵ�����
    * ������
    */
    LinkedList_Iterator next() const {
        return  LinkedList_Iterator(pointer->next);
    }

    /*
    * �ж��Ƿ�����һ���ڵ�, last ����Ϊ��Ч�Ľڵ�
    */
    bool hasNext() const {
        return pointer!=nullptr && pointer->next != nullptr && pointer->next->next != nullptr;
    }

    bool hasPrevious() const {
        return pointer!=nullptr && pointer->pre != nullptr;
    }

    /*
    * ʹ������ָ��ǰһ��Ԫ��,ǰ��--
    */
    LinkedList_Iterator& operator--() {
        _setSelfPrevious();
        return *this;
    }

    /*
    * ʹ������ָ��ǰһ��Ԫ��,����--
    */
    LinkedList_Iterator operator--(int) {
        LinkedList_Iterator oldIter(*this);
        _setSelfPrevious();
        return oldIter;
    }

    /*
    * ʹ������ָ���һ��Ԫ��,ǰ��++
    */
    LinkedList_Iterator& operator++() override {
        _setSelfNext();
        return *this;
    }

    /*
    * ʹ������ָ���һ��Ԫ��,����++
    */
    LinkedList_Iterator operator++(int) override {
        LinkedList_Iterator oldIter(*this);
        _setSelfNext();
        return oldIter;
    }

    /*
    * ��ֵ��������
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
    * ��ȡ������ָ���Ԫ��
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
    * �ڵ�����ָ���λ��ǰ�����һ���ڵ�, �����´˵�����ʹ��ָ��ղ�����½ڵ�
    * ָ��ͷ�ڵ�ʱ��������ͷ�ڵ�ǰ�����һ����ͷ, ������ head �ڵ�
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
    * ������β����ӽڵ� (��� last �ڵ㲢����һ���µ� last �ڵ�)
    * ע��: last �ڵ�� elem ������ԶΪnull
    */
    template<typename VT>
    void _pushBack(VT&& val) {
        last->elem = new T(std::forward<VT>(val));
        last->next = new Node{ nullptr,last,nullptr };
        last = last->next;
        ++_size;
    }

    /*
    * ɾ��������pos��ָ��Ľڵ�, ���ֱ�ɾ�ڵ�ǰ�������, ��ʹsize-1
    * pos ����ָ��last
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
        } else { //�����ɾ�ڵ㲻��ͷ, ��prePtrһ������null, ����һ��Ϊnull
            prePtr->next = nextPtr;
        }
        delete pos.pointer;
        --_size;
    }

    void _removeAll() {
        //�����ƶ�ʱ, head==last==nullptr, ��������ѭ��
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
    * class Iterator ����ĵ�����
    * ���ڱ�������Ͳ���Ԫ��
    * Node* pointer����Ϊnullptr, ֻ��ָ�������ĳһԪ��(end����һ��Ԫ��)
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
    * ��ȡָ��head�ĵ�����
    */
    Iterator begin() const {
        return Iterator(head);
    }

    /*
    * ��ȡָ��last�ĵ�����
    * ע��: last ��elem��ԶΪnull
    */
    Iterator end() const {
        return Iterator(last);
    }

    /*
    * �Ƴ���������ָ�Ľڵ�
    * ������ֵ���غ���ʱ, �������Զ�����
    */
    void remove(Iterator& pos) {
        Node* nextPtr = pos.pointer->next;
        _deleteElem(pos);
        pos.pointer = nextPtr;
    }

    /*
    * �Ƴ���������ָ�Ľڵ�
    * ������ֵ���غ���ʱ, ���������Զ�����(��Ϊ��ֵ�ĵ�����Ӧ��Ϊһ���ԵĻ����ǽ�Ҫ������)
    */
    void remove(Iterator&& pos) {
        _deleteElem(std::move(pos));
    }

    /*
    * void removeIf(Func_t&& conditionFunc)
    * �����û���lambda�ȷ�ʽ����һ����������, ��ÿ���ڵ����, ����true��ɾ���ýڵ�
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
    * ��������е�����Ԫ��, ������last�ڵ�(Ч���൱�ڻָ������ճ�ʼ����)
    */
    void clear() {
        _removeAll();
    }

    /*
    * ��������
    */
    ~LinkedList() {
        _removeAll();
        delete head;
    }
};

#endif // LinkedList_h__