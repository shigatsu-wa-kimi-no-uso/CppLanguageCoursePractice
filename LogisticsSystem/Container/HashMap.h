#pragma once
#ifndef HashMap_h__
#define HashMap_h__
#include "iterable.h"
#include "LinkedList.h"
#include "String.h"

using DWORD = unsigned long;
using BYTE = unsigned char;

template<typename K, typename V> class HashMap;

template<typename K, typename V>
struct Entry
{
    const K key;
    V value;
};

template<typename K, typename V>
class HashMap_Iterator : public Iterator<HashMap_Iterator<K, V>, Entry<K, V>>
{
private:
    friend class HashMap<K, V>;

    using Elem = typename HashMap<K, V>::Elem;
    using HashTable = typename HashMap<K, V>::HashTable;

    HashTable* const* const hashTables;
    int groupIndex;
    int tableIndex;
    typename LinkedList<Elem>::Iterator iter;

    static constexpr HashMap_Iterator _end() {
        HashMap_Iterator it;
        it.groupIndex = 32;
        return it;
    }

    HashMap_Iterator() : groupIndex(32), tableIndex(0), hashTables(nullptr) {}
    HashMap_Iterator(const int& groupIndex, const int& tableIndex, HashTable* const* hashTables)
        : groupIndex(groupIndex), tableIndex(tableIndex), hashTables(hashTables), iter(hashTables[groupIndex][tableIndex].lst.begin()) {}
public:
    HashMap_Iterator(const HashMap_Iterator& val) noexcept : groupIndex(val.groupIndex), tableIndex(val.tableIndex), hashTables(val.hashTables), iter(val.iter) {}
    HashMap_Iterator(HashMap_Iterator&& val) noexcept : groupIndex(val.groupIndex), tableIndex(val.tableIndex), hashTables(val.hashTables), iter(val.iter) {}

    /*
    * 使迭代器指向下一个元素
    */
    void _setSelfNext() {
        if (iter.hasNext()) {
            ++iter;
        } else {
            //遍历该哈希表组, 找到有元素的那个
            ++tableIndex; //跳过当前的哈希表
            for (; groupIndex < 32; ++groupIndex) {
                //检测哈希表组是否已分配
                if (hashTables[groupIndex] == nullptr) {
                    //未分配, 直接设置groupIndex=32 (迭代器迭代到end的标志)
                    groupIndex = 32;
                    return;
                }
                const int thisGroupCap = HashMap<K, V>::_getTableGroupCap(groupIndex);
                for (; tableIndex < thisGroupCap; ++tableIndex) {
                    if (hashTables[groupIndex][tableIndex].lst.size() != 0) {
                        iter = hashTables[groupIndex][tableIndex].lst.begin();
                        return;
                    }
                }
                tableIndex = 0;
            }
        }

    }

    /*
    * 获取指向下一个元素的迭代器
    * 迭代器
    */
    HashMap_Iterator next() const {
        HashMap_Iterator newIt = *this;
        return ++newIt;
    }

    /*
    * 判断是否有下一个元素
    */
    bool hasNext() const {
        return next().groupIndex != 32;
    }

    /*
    * 使迭代器指向下一个元素,前置++
    */
    HashMap_Iterator& operator++() override {
        _setSelfNext();
        return *this;
    }

    /*
    * 使迭代器指向后一个元素,后置++
    */
    HashMap_Iterator operator++(int) override {
        HashMap_Iterator oldIter = *this;
        _setSelfNext();
        return oldIter;
    }

    /*
    * 赋值拷贝函数
    */
    HashMap_Iterator& operator=(const HashMap_Iterator& rgt) override {
        groupIndex = rgt.groupIndex;
        tableIndex = rgt.tableIndex;
        iter = rgt.iter;
        return *this;
    }

    bool operator==(const HashMap_Iterator& rgt) const override {
        return (groupIndex == rgt.groupIndex && tableIndex == rgt.tableIndex && iter == rgt.iter) || (groupIndex == 32 && groupIndex == rgt.groupIndex);
    }

    bool operator!=(const HashMap_Iterator& rgt) const override {
        return !operator==(rgt);
    }

    /*
    * 获取迭代器指向的元素
    */
    Entry<K, V>& operator*() const override {
        return *iter->entry;
    }

    Entry<K, V>* operator->() const override {
        return iter->entry;
    }

    DWORD hashCode() const {
        return iter->hashCode;
    }
};


/*
* class HashMap
* 哈希散列表
* 原理: 每个对象V与一个键K绑定, 组成条目Entry<K, V>. 通过计算键的Hash值, 将其映射到哈希表上
* 哈希表使用 HashTable , 此次设置了32个Hash表, 第 i 个Hash表有 2^i 个 Entry 元素, i从0开始
*/
template<typename K, typename V>
class HashMap : public Iterable<HashMap_Iterator<K, V>>
{
public:
    friend class HashMap_Iterator<K, V>;
    using Entry = Entry<K, V>;
    using Iterator = HashMap_Iterator<K, V>;

    /*
    * Elem, 链表的元素, 链在哈希表的头节点上
    * 包含键值对和键的哈希值
    */
private:
    struct Elem
    {
        Entry* entry;
        DWORD hashCode;
    };

    struct HashTable
    {
        LinkedList<Elem> lst;
    };
    /*
    * HashTable* hashTables
    * 存储元素的32个哈希表组, 每个哈希表组中有2^i个HashTable元素, i为hashTables数组的下标
    */
    HashTable* hashTables[32];
    int currGroupCnt;
    DWORD _size;
    DWORD _capacity;
    static constexpr float loadFactor = 0.75;

    /*
    * 二分法求最高有效位, 作为HashTable组的组索引
    */
    static int _getGroupIndex(const DWORD& truncatedCode) {
        int ub = 32;
        int lb = 0;
        while (lb <= ub) {
            int mid = lb + (ub - lb) / 2;
            DWORD mask = ~0U << mid;
            DWORD maskedVal = truncatedCode & mask;
            if (maskedVal == 0) {
                ub = mid - 1;
            } else if (maskedVal == ~mask + 1) {
                return mid + 1;
            } else {
                lb = mid + 1;
            }
        }
        return 0;
    }

    static int _getTableGroupCap(const int& groupIndex) {
        return groupIndex < 2 ? 1 : (1 << (groupIndex - 1));
    }

    static int _getTableIndex(const DWORD& truncatedCode, const int& groupIndex) {
        return (_getTableGroupCap(groupIndex) - 1) & truncatedCode;
    }
    /*
    * 针对字符串的hash计算函数
    */
    DWORD _strHash32(const String& str) const {
		DWORD h = 0;
        DWORD len = str.length();
		for (size_t i = 0; i < len; ++i) {
			h = 31 * h + str[i];
		}
		return h;
    }

    /*
    * int hash32(T&& t)
    * 计算类型为T的变量t的哈希值, 哈希值为32位
    * 若T为基本类型(int, float, double 等), 若大小小于4字节, 直接返回对应的32位int值作为hash值, 若大小为8字节, 高32位与低32位异或后的32位作为哈希值
    * 若为数组(典型如字符串), 数组长度为n, 则使用公式 t[0]*31^(n-1) + t[1]*31^(n-2) + ... + t[n-2]*31 + t[n-1] 的结果作为32位hash值(大于32位int溢出舍弃)
    */
    template<typename T>
    DWORD _hash32(const T& t) const {
        static_assert(std::is_fundamental<T>() || std::is_same<T,String>(), "Unsupported type");
        if (std::is_fundamental<T>()) {
            DWORD d;
            switch (sizeof(T)) {
            case 1:
                ((BYTE*)&d)[0] = ((BYTE*)&t)[0];
                ((BYTE*)&d)[1] = 0;
                ((BYTE*)&d)[2] = 0;
                ((BYTE*)&d)[3] = 0;
                return d;
            case 2:
                ((BYTE*)&d)[0] = ((BYTE*)&t)[0];
                ((BYTE*)&d)[1] = ((BYTE*)&t)[1];
                ((BYTE*)&d)[2] = 0;
                ((BYTE*)&d)[3] = 0;
                return d;
            case 4:
                ((BYTE*)&d)[0] = ((BYTE*)&t)[0];
                ((BYTE*)&d)[1] = ((BYTE*)&t)[1];
                ((BYTE*)&d)[2] = ((BYTE*)&t)[2];
                ((BYTE*)&d)[3] = ((BYTE*)&t)[3];
                return d;
            case 8:
                DWORD high32 = *((unsigned long long*)(&t)) >> 32;
                DWORD low32 = *((DWORD*)(&t));
                return (const DWORD)(high32 ^ low32);
            }
        } else if (std::is_same<T, String>()) {
            return _strHash32(*(String*)(&t));
        } else {
            abort();
        }
    }


    /*
    * void _resize()
    * 检测哈希表是否有扩容需求
    * 检测标准: size/capacity>=loadFactor时, 需要扩容以减少哈希冲突, loadFactor一般取0.75
    * 扩容后, 需要针对旧的哈希表中的节点重新计算其哈希表索引, 并移动节点
    */
    void _resize() {
        if (_capacity == 0 || (float)((float)_size / (float)_capacity) >= loadFactor) {
            /*开辟新哈希表*/
            const int newAllocCap = _capacity == 0 ? 1 : _capacity;
            const int oldGroupCnt = currGroupCnt;
            hashTables[currGroupCnt++] = new HashTable[newAllocCap];
            _capacity += newAllocCap;
            /*重新计算旧哈希表中的元素的哈希值对应的哈希表索引, 并移动节点*/
            int thisTableCap = 1;
            for (int i = 0; i < oldGroupCnt; ++i) {
                thisTableCap <<= (i < 2 ? 0 : 1);       //i=0,1时不左移, i不为0,1时每次左移1位
                for (int j = 0; j < thisTableCap; ++j) {
                    for (LinkedList<Elem>::template Iterator it = hashTables[i][j].lst.begin(); it != hashTables[i][j].lst.end(); ) {
                        const int truncatedCode = it->hashCode & (_capacity - 1);
                        const int groupIndex = _getGroupIndex(truncatedCode);
                        /*哈希表索引落在老哈希表中时, 无需移动. 一旦是落在先前分配的哈希表中, 则其位置一定没有变化, 而不会在先前分配的哈希表中改变位置*/
                        if (groupIndex >= oldGroupCnt) {
                            const int tableIndex = _getTableIndex(truncatedCode, groupIndex);
                            hashTables[groupIndex][tableIndex].lst.pushBack(std::move(*it));
                            hashTables[i][j].lst.remove(it);
                            continue;
                        }
                        ++it;
                    }
                }
            }
        }
    }

    template<typename Iter_t>
    void _remove(Iter_t&& pos) {
        LinkedList<Elem>& targetList = hashTables[pos.groupIndex][pos.tableIndex].lst;
        targetList.remove(std::forward<Iter_t>(pos).iter); //保持左值性或右值性
        --_size;
    }

    template<typename KT,typename VT>
    void _put(KT&& key,VT&& value) {
        _resize();
        DWORD hashCode = _hash32(key);
        const int truncatedCode = hashCode & (_capacity - 1);
        const int groupIndex = _getGroupIndex(truncatedCode);
        const int tableIndex = _getTableIndex(truncatedCode, groupIndex);
        HashTable& targetTable = hashTables[groupIndex][tableIndex];
        LinkedList<Elem>& targetList = targetTable.lst;
        int oldSize = targetList._size;
        targetList.removeIf([&](typename LinkedList<Elem>::Iterator& it) {
            return it->hashCode == hashCode && it->entry->key == key;
            });
        targetList.pushBack({ new Entry{std::forward<KT>(key),std::forward<VT>(value)} ,hashCode });
        _size += targetList._size - oldSize;
    }
public:
    void put(const K& key, const V& value) {
        _put(key, value);
    }

    void put(K&& key,const V& value) {
        _put(std::move(key), value);
    }

    void put(K&& key, V&& value) {
        _put(std::move(key), std::move(value));
    }

    void put(const K& key, V&& value) {
        _put(key, std::move(value));
    }

    bool hasKey(const K& key) {
       return get(key) != end();
    }

    Iterator get(const K& key) const {
        if (_size == 0) {
            return end();
        }
        DWORD hashCode = _hash32(key);
        const int truncatedCode = hashCode & (_capacity - 1);
        const int groupIndex = _getGroupIndex(truncatedCode);
        const int tableIndex = _getTableIndex(truncatedCode, groupIndex);
        //targetTable一定不为nullptr, 因为是在已分配的哈希表中获取
        LinkedList<Elem>& targetList = hashTables[groupIndex][tableIndex].lst;
        if (targetList.size() == 0) {
            return end();
        }
        Iterator iter(groupIndex, tableIndex, hashTables);
        for (; iter.iter != targetList.end(); iter.iter._setSelfNext()) {
            if (iter.hashCode() == hashCode && iter->key == key) {
                return iter;
            }
        }
        return end();
    }

    DWORD size() const {
        return _size;
    }

    DWORD capacity() const {
        return _capacity;
    }
    /*
    * 移除某一元素, 定位迭代器为右值, 移除后迭代器不进行后移
    * 适用于一次性迭代器或调用该函数后即弃用的迭代器
    */
    void remove(Iterator&& pos) {
        //一旦以右值传入_remove, 则调用链表的右值重载的remove函数, 该函数没有后移链表迭代器, 且链表迭代器内部指针处于删除状态, 故该迭代器将再也无法继续后移, 该迭代器将报废
        _remove(std::move(pos));
    }

    /*
    * 移除某一元素, 定位迭代器为左值, 移除后迭代器自动后移
    */
    void remove(Iterator& pos) {
        _remove(pos);
        //在链表中移除后自动后移链表迭代器, 当链表迭代器指向end时, 说明该链表已经没有元素, 需要手动继续后移hashmap迭代器
        if (pos.iter == hashTables[pos.groupIndex][pos.tableIndex].lst.end()) pos._setSelfNext();
    }


    /*
    * 针对需要复杂逻辑来判别是否删除某一元素的remove函数
    * 可传入lambda 等返回bool类型的函数, 函数返回true时则删除该元素
    * 由于需要对整个hashmap遍历并判断, 故其效率较低
    * 如果需要查找某一键并删除, 则不应该调用此函数
    */
    template<typename Func_t>
    void removeIf(Func_t&& conditionFunc) {
        for (Iterator it = begin(); it != end(); ) {
            if (conditionFunc(it)) {
                remove(it);
            } else {
                it._setSelfNext();
            }
        }
    }


    /*
    * 获取指向第一个元素的迭代器
    */
    Iterator begin() const override {
        Iterator iter(0, 0, hashTables);
        if (hashTables[0][0].lst.size() == 0) iter._setSelfNext();
        return iter;
    }

    /*
    * 获取标志着结束的迭代器
    */
    Iterator end() const override {
        return Iterator::_end();
    }

    V& operator[](const K& key) const {
        Iterator target = get(key);
        assert(target != end());
        return target->value;
    }

    HashMap() : _size(0), _capacity(1), currGroupCnt(1) {
        hashTables[0] = new HashTable[1];
        for (int i = 1; i < _countof(hashTables); ++i) {
            hashTables[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < currGroupCnt; ++i) {
            delete[] hashTables[i];
        }
    }
};

#endif // HashMap_h__