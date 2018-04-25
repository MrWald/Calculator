//Created by Volodymyr Fomin on 20/02/2018

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <vector>
#include <iostream>
#include "Iterator.h"

using namespace std;

template <class Key, class Value> 
class HashMap : public Iterator<HashMap<Key, Value>, Value>
{
public:
	class BadHashMap;

	explicit HashMap(const size_t alloc=100);
	HashMap(const vector<Key>&, const vector<Value>&, const size_t alloc=100);
	HashMap(const Key [], const Value [], const unsigned int size, const size_t alloc=100);
	HashMap(const HashMap<Key, Value>& map);
	HashMap<Key, Value>& operator=(const HashMap<Key, Value>& map);
	virtual ~HashMap();

	const Value& operator[](const Key& key) const;
	HashMap<Key, Value>& add(const Key& key, const Value& value);
	HashMap<Key, Value>& remove(const Key& key);
	const bool isEmpty() const;
    virtual const HashMap<Key, Value>& start() const;
    virtual const HashMap<Key, Value>& next() const;
    virtual const Value& current() const;
    virtual const bool end() const;
private:
	const size_t getHashCode(const Key& key) const;

	vector<vector<Value>> _values;
	vector<vector<Key>> _keys;
	mutable size_t _current1, _current2;
	const size_t _id;
	static size_t _freeID;
};

template<class Key, class Value> size_t HashMap<Key, Value>::_freeID(0);

template<class Key, class Value>
HashMap<Key, Value>::HashMap(const size_t alloc)
        : _id(++_freeID), _values(alloc), _keys(alloc), _current1(0), _current2(0)
{
#ifndef NDEBUG
    cout << "Hash Map ID - " << _id << " created" << endl;
#endif
}

template<class Key, class Value>
HashMap<Key, Value>::HashMap(const vector<Key>& keys, const vector<Value>& values, const size_t alloc)
        : _id(++_freeID), _values(alloc), _keys(alloc), _current1(0), _current2(0)
{
    if(keys.size()!=values.size())
        throw BadHashMap("Different sizes of Key's and Value's arrays");
    size_t hashCode(0);
    for(unsigned int i(0);i<keys.size();++i)
    {
        hashCode = getHashCode(keys[i]);
        _values[hashCode].push_back(values[i]);
        _keys[hashCode].pushBack(keys[i]);
    }
#ifndef NDEBUG
    cout << "Hash Map ID - " << _id << " created" << endl;
#endif
}

template<class Key, class Value>
HashMap<Key, Value>::HashMap(const Key keys[], const Value values[], const unsigned int size, const size_t alloc)
        : _id(++_freeID), _values(alloc), _keys(alloc), _current1(0), _current2(0)
{
    size_t hashCode(0);
    for(size_t i(0);i<size;++i)
    {
        hashCode = getHashCode(keys[i]);
        _values[hashCode].push_back(values[i]);
        _keys[hashCode].push_back(keys[i]);
    }
#ifndef NDEBUG
    cout << "Hash Map ID - " << _id << " created" << endl;
#endif
}

template<class Key, class Value>
HashMap<Key, Value>::HashMap(const HashMap<Key, Value>& map)
        : _id(++_freeID), _values(map._values), _keys(map._keys), _current1(0), _current2(0)
{
#ifndef NDEBUG
    cout << "Hash Map ID - " << _id << " copied" << endl;
#endif
}

template<class Key, class Value>
HashMap<Key, Value>& HashMap<Key, Value>::operator=(const HashMap<Key, Value>& map)
{
    if(this!=&map)
    {
        _values = map._values;
        _keys = map._keys;
    }
    return *this;
}

template<class Key, class Value>
HashMap<Key, Value>::~HashMap()
{
#ifndef NDEBUG
    cout << "Hash Map ID - " << _id << " destroyed" << endl;
#endif
}

template<class Key, class Value>
const Value& HashMap<Key, Value>::operator[](const Key& key) const
{
    const size_t hashCode(getHashCode(key));
    if(!hashCode)
        throw BadHashMap("Map is empty");
    const vector<Key>* const region = &_keys[hashCode];
    for(size_t i(0);i<region->size();++i)
    {
        if((*region)[i]==key)
            return _values[hashCode][i];
    }
    throw BadHashMap("No such element " + key);
}

template<class Key, class Value>
HashMap<Key, Value>& HashMap<Key, Value>::add(const Key& key, const Value& value)
{
    const size_t hashCode(getHashCode(key));
    _values[hashCode].push_back(value);
    _keys[hashCode].push_back(key);
    return *this;
}

template<class Key, class Value>
HashMap<Key, Value>& HashMap<Key, Value>::remove(const Key& key)
{
    const size_t hashCode(getHashCode(key));
    const vector<Key>* const region = &_keys[hashCode];
    for(size_t i(0);i<region->size();++i)
    {
        if((*region)[i]==key)
        {
            _values[hashCode].erase(i);
            _keys[hashCode].erase(i);
            return *this;
        }
    }
    throw BadHashMap("No such element " + key);
}

template<class Key, class Value>
const bool HashMap<Key, Value>::isEmpty() const
{
    bool res(!_values.empty());
    for(unsigned int i(0);i<_values.size() && res;++i)
    {
        res = res && _values[i].empty();
    }
    return res;
}

template<class Key, class Value>
const size_t HashMap<Key, Value>::getHashCode(const Key& key) const
{
    return _values.size() ? hash<Key>()(key)%_values.size() : 0;
}


template<class Key, class Value>
const HashMap<Key, Value>& HashMap<Key, Value>::start() const
{
    _current1=_current2=0;
    while(_values[_current1].empty())
        _current1++;
    return *this;
}

template<class Key, class Value>
const HashMap<Key, Value>& HashMap<Key, Value>::next() const
{
    if(isEmpty())
        throw BadHashMap("There are no elements in Hash Map");
    while(_current2<_values[_current1].size())
        _current2++;
    if(_current2==_values[_current1].size())
    {
        _current2 = 0;
        while (_values[_current1].empty())
            _current1++;
    }
    return *this;
}

template<class Key, class Value>
const Value& HashMap<Key, Value>::current() const
{
    if(isEmpty())
        throw BadHashMap("There are no elements in Hash Map");
    if(end())
        throw BadHashMap("There are no more elements in Hash Map");
    return _values[_current1][_current2];
}

template<class Key, class Value>
const bool HashMap<Key, Value>::end() const
{
    bool res(true);
    res = _current2>=_values[_current1].size()-1;
    for(size_t i(_current1+1);i<_values.size() && res;++i)
        res = _values[i].empty();
    return res;
}

template<class Key, class Value>
class HashMap<Key, Value>::BadHashMap
{
public:
    explicit BadHashMap(const string& message="") : _message(message) {}
    BadHashMap(const BadHashMap& exception) : _message(exception._message){}
    ~BadHashMap(){}
    const string& error() const {return _message;}
private:
    BadHashMap& operator=(const BadHashMap&);
    string _message;
};

#endif