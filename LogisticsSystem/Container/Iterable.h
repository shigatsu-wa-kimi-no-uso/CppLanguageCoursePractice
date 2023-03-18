#pragma once
#ifndef Iterable_h__
#define Iterable_h__

/*
* 可迭代容器的基本接口声明
*/
template<typename Iter_t>
class Iterable
{
public:
    using Iterator = Iter_t;

    virtual Iterator begin() const = 0;

    virtual Iterator end() const = 0;
};


/*
* 迭代器的基本接口声明
*/
template<typename Iter_t, typename Elem_t>
class Iterator
{
public:

    virtual Iter_t next() const = 0;

    virtual bool hasNext() const = 0;

    /*
    * 使迭代器指向后一个元素,前置++
    */
    virtual Iter_t& operator++() = 0;

    /*
    * 使迭代器指向后一个元素,后置++
    */
    virtual Iter_t operator++(int) = 0;

    /*
    * 赋值拷贝函数
    */
    virtual Iter_t& operator=(const Iter_t& rgt) = 0;

    virtual bool operator==(const Iter_t& rgt) const = 0;

    virtual bool operator!=(const Iter_t& rgt) const = 0;

    /*
    * 获取迭代器指向的元素
    */
    virtual Elem_t& operator*() const = 0;

    virtual Elem_t* operator->() const = 0;
};

#endif // Iterable_h__