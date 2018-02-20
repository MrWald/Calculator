
#pragma once
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <vector>
using namespace std;
template <class Key, class Value> 
class HashMap
{
public:
	HashMap()
		: _id(++_freeID), _values(100), _keys(100)
	{}
	HashMap(const vector<Key>& keys, const vector<Value>& values)
		: _id(++_freeID), _values(100), _keys(100)
	{
		size_t hashCode(0);
		for(unsigned int i(0);i<keys.size();++i)
		{
			hashCode = hash<Key>()(keys[i])%_values.size();
			_values[hashCode].push_back(values[i]);
			_keys[hashCode].pushBack(keys[i]);
		}
	}
	HashMap(const Key keys[], const Value values[], unsigned int size)
		: _id(++_freeID), _values(100), _keys(100)
	{
		size_t hashCode(0);
		for(unsigned int i(0);i<size;++i)
		{
			hashCode = hash<Key>()(keys[i])%_values.size();
			_values[hashCode].push_back(values[i]);
			_keys[hashCode].push_back(keys[i]);
		}
	}
	HashMap(const HashMap& map)
		: _id(++_freeID), _values(map._values), _keys(map._keys)
	{

	}
	~HashMap(){}
	const Value& operator[](const Key& key) const
		{
			const size_t hashCode(hash<Key>()(key)%_values.size());
			const vector<Key>* const region = &_keys[hashCode];
			for(unsigned int i(0);i<region->size();++i)
			{
				if((*region)[i]==key)
					return _values[hashCode][i];
			}
			throw invalid_argument("No such element");
		}
	void add(const Key& key, const Value& value)
		{
			const size_t hashCode(hash<Key>()(key)%_values.size());
			_values[hashCode].push_back(value);
			_keys[hashCode].push_back(key);
		}
	const Value& remove(const Key& key)
		{
			const unsigned int hashCode(hash<Key>()(key)%_values.size());
			const vector<Key>* const region = &_keys[hashCode];
			Value res;
			for(unsigned int i(0);i<region->size();++i)
			{
				if((*region)[i]==key)
				{
					res = _values[hashCode][i];
					_values[hashCode].erase(i);
					_keys[hashCode].erase(i);
					return res;
				}
			}
			throw invalid_argument("No such element");
		}
	const bool isEmpty() const
	{
		bool res(false);
		for(unsigned int i(0);i<_values.size() && !res;++i)
		{
			res = res || _values[i].empty();
		}
		return res;
	}
private:
	HashMap& operator=(const HashMap&){}
	vector<vector<Value>> _values;
	vector<vector<Key>> _keys;
	const unsigned int _id;
	static unsigned int _freeID;
};

#endif