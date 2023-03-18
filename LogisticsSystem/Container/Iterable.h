#pragma once
#ifndef Iterable_h__
#define Iterable_h__

/*
* �ɵ��������Ļ����ӿ�����
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
* �������Ļ����ӿ�����
*/
template<typename Iter_t, typename Elem_t>
class Iterator
{
public:

    virtual Iter_t next() const = 0;

    virtual bool hasNext() const = 0;

    /*
    * ʹ������ָ���һ��Ԫ��,ǰ��++
    */
    virtual Iter_t& operator++() = 0;

    /*
    * ʹ������ָ���һ��Ԫ��,����++
    */
    virtual Iter_t operator++(int) = 0;

    /*
    * ��ֵ��������
    */
    virtual Iter_t& operator=(const Iter_t& rgt) = 0;

    virtual bool operator==(const Iter_t& rgt) const = 0;

    virtual bool operator!=(const Iter_t& rgt) const = 0;

    /*
    * ��ȡ������ָ���Ԫ��
    */
    virtual Elem_t& operator*() const = 0;

    virtual Elem_t* operator->() const = 0;
};

#endif // Iterable_h__