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
    * ʹ������ָ����һ��Ԫ��
    */
    void _setSelfNext() {
        if (iter.hasNext()) {
            ++iter;
        } else {
            //�����ù�ϣ����, �ҵ���Ԫ�ص��Ǹ�
            ++tableIndex; //������ǰ�Ĺ�ϣ��
            for (; groupIndex < 32; ++groupIndex) {
                //����ϣ�����Ƿ��ѷ���
                if (hashTables[groupIndex] == nullptr) {
                    //δ����, ֱ������groupIndex=32 (������������end�ı�־)
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
    * ��ȡָ����һ��Ԫ�صĵ�����
    * ������
    */
    HashMap_Iterator next() const {
        HashMap_Iterator newIt = *this;
        return ++newIt;
    }

    /*
    * �ж��Ƿ�����һ��Ԫ��
    */
    bool hasNext() const {
        return next().groupIndex != 32;
    }

    /*
    * ʹ������ָ����һ��Ԫ��,ǰ��++
    */
    HashMap_Iterator& operator++() override {
        _setSelfNext();
        return *this;
    }

    /*
    * ʹ������ָ���һ��Ԫ��,����++
    */
    HashMap_Iterator operator++(int) override {
        HashMap_Iterator oldIter = *this;
        _setSelfNext();
        return oldIter;
    }

    /*
    * ��ֵ��������
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
    * ��ȡ������ָ���Ԫ��
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
* ��ϣɢ�б�
* ԭ��: ÿ������V��һ����K��, �����ĿEntry<K, V>. ͨ���������Hashֵ, ����ӳ�䵽��ϣ����
* ��ϣ��ʹ�� HashTable , �˴�������32��Hash��, �� i ��Hash���� 2^i �� Entry Ԫ��, i��0��ʼ
*/
template<typename K, typename V>
class HashMap : public Iterable<HashMap_Iterator<K, V>>
{
public:
    friend class HashMap_Iterator<K, V>;
    using Entry = Entry<K, V>;
    using Iterator = HashMap_Iterator<K, V>;

    /*
    * Elem, �����Ԫ��, ���ڹ�ϣ���ͷ�ڵ���
    * ������ֵ�Ժͼ��Ĺ�ϣֵ
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
    * �洢Ԫ�ص�32����ϣ����, ÿ����ϣ��������2^i��HashTableԪ��, iΪhashTables������±�
    */
    HashTable* hashTables[32];
    int currGroupCnt;
    DWORD _size;
    DWORD _capacity;
    static constexpr float loadFactor = 0.75;

    /*
    * ���ַ��������Чλ, ��ΪHashTable���������
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
    * ����ַ�����hash���㺯��
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
    * ��������ΪT�ı���t�Ĺ�ϣֵ, ��ϣֵΪ32λ
    * ��TΪ��������(int, float, double ��), ����СС��4�ֽ�, ֱ�ӷ��ض�Ӧ��32λintֵ��Ϊhashֵ, ����СΪ8�ֽ�, ��32λ���32λ�����32λ��Ϊ��ϣֵ
    * ��Ϊ����(�������ַ���), ���鳤��Ϊn, ��ʹ�ù�ʽ t[0]*31^(n-1) + t[1]*31^(n-2) + ... + t[n-2]*31 + t[n-1] �Ľ����Ϊ32λhashֵ(����32λint�������)
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
    * ����ϣ���Ƿ�����������
    * ����׼: size/capacity>=loadFactorʱ, ��Ҫ�����Լ��ٹ�ϣ��ͻ, loadFactorһ��ȡ0.75
    * ���ݺ�, ��Ҫ��ԾɵĹ�ϣ���еĽڵ����¼������ϣ������, ���ƶ��ڵ�
    */
    void _resize() {
        if (_capacity == 0 || (float)((float)_size / (float)_capacity) >= loadFactor) {
            /*�����¹�ϣ��*/
            const int newAllocCap = _capacity == 0 ? 1 : _capacity;
            const int oldGroupCnt = currGroupCnt;
            hashTables[currGroupCnt++] = new HashTable[newAllocCap];
            _capacity += newAllocCap;
            /*���¼���ɹ�ϣ���е�Ԫ�صĹ�ϣֵ��Ӧ�Ĺ�ϣ������, ���ƶ��ڵ�*/
            int thisTableCap = 1;
            for (int i = 0; i < oldGroupCnt; ++i) {
                thisTableCap <<= (i < 2 ? 0 : 1);       //i=0,1ʱ������, i��Ϊ0,1ʱÿ������1λ
                for (int j = 0; j < thisTableCap; ++j) {
                    for (LinkedList<Elem>::template Iterator it = hashTables[i][j].lst.begin(); it != hashTables[i][j].lst.end(); ) {
                        const int truncatedCode = it->hashCode & (_capacity - 1);
                        const int groupIndex = _getGroupIndex(truncatedCode);
                        /*��ϣ�����������Ϲ�ϣ����ʱ, �����ƶ�. һ����������ǰ����Ĺ�ϣ����, ����λ��һ��û�б仯, ����������ǰ����Ĺ�ϣ���иı�λ��*/
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
        targetList.remove(std::forward<Iter_t>(pos).iter); //������ֵ�Ի���ֵ��
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
        //targetTableһ����Ϊnullptr, ��Ϊ�����ѷ���Ĺ�ϣ���л�ȡ
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
    * �Ƴ�ĳһԪ��, ��λ������Ϊ��ֵ, �Ƴ�������������к���
    * ������һ���Ե���������øú��������õĵ�����
    */
    void remove(Iterator&& pos) {
        //һ������ֵ����_remove, ������������ֵ���ص�remove����, �ú���û�к������������, ������������ڲ�ָ�봦��ɾ��״̬, �ʸõ���������Ҳ�޷���������, �õ�����������
        _remove(std::move(pos));
    }

    /*
    * �Ƴ�ĳһԪ��, ��λ������Ϊ��ֵ, �Ƴ���������Զ�����
    */
    void remove(Iterator& pos) {
        _remove(pos);
        //���������Ƴ����Զ��������������, �����������ָ��endʱ, ˵���������Ѿ�û��Ԫ��, ��Ҫ�ֶ���������hashmap������
        if (pos.iter == hashTables[pos.groupIndex][pos.tableIndex].lst.end()) pos._setSelfNext();
    }


    /*
    * �����Ҫ�����߼����б��Ƿ�ɾ��ĳһԪ�ص�remove����
    * �ɴ���lambda �ȷ���bool���͵ĺ���, ��������trueʱ��ɾ����Ԫ��
    * ������Ҫ������hashmap�������ж�, ����Ч�ʽϵ�
    * �����Ҫ����ĳһ����ɾ��, ��Ӧ�õ��ô˺���
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
    * ��ȡָ���һ��Ԫ�صĵ�����
    */
    Iterator begin() const override {
        Iterator iter(0, 0, hashTables);
        if (hashTables[0][0].lst.size() == 0) iter._setSelfNext();
        return iter;
    }

    /*
    * ��ȡ��־�Ž����ĵ�����
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