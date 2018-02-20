#include "HashMap.h"
#include <iostream>
using namespace std;
template<class Key, class Value> unsigned int HashMap::freeID(0);

template<class Key, class Value> HashMap::HashMap()
	: _id(++_freeID), _values(100)
{
}
template<class Key, class Value> HashMap::HashMap(const vector<Key>& keys, const vector<Value>& values)
	: _id(++_freeID), _values(100)
{
	for(unsigned int i(0);i<keys.size();++i)
	{
		_values[hash<Key>(keys[i])%_values.size()].push_back(values[i]);
	}
}

template<class Key, class Value> HashMap::~HashMap{}

template<class Key, class Value> const HashMap::Value& HashMap::operator[](const Key& key) const
{
	const vector<Value>* const region = &_values[hash<Key>(key)%_values.size()];
	for(unsigned int i(0);i<region->size();++i)
	{
		if((*region)[i]==key)
			return (*region)[i];
	}
	throw invalid_argument("No such element");
}
template<class Key, class Value> void HashMap::add(const Key& key, const Value& value)
{
	_values[hash<Key>(key)%_values.size()].push_back(value);
}
template<class Key, class Value> const Value& HashMap<Key, Value>::remove(const Key& key)
{
	const vector<Value>* const region = &_values[hash<Key>(key)%_values.size()];
	for(unsigned int i(0);i<region->size();++i)
	{
		if((*region)[i]==key)
			return (*region)[i];
	}
	throw invalid_argument("No such element");
}
template<class Key, class Value> const bool HashMap::isEmpty() const
{
	bool res(false);
	for(unsigned int i(0);i<_values.size() && !res;++i)
	{
		res = res || _values[i].empty();
	}
	return res;
}