//Created by Volodymyr Fomin on 20/02/2018

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <vector>

using namespace std;

template <class Key, class Value> 
class HashMap
{
public:
	class BadHashMap
	{
	public:
		BadHashMap(const string& message="") : _message(message) {}
		BadHashMap(const BadHashMap& exception) : _message(exception._message){}
		~BadHashMap(){}
		const string& error() const {return _message;}
	private:
		BadHashMap& operator=(const BadHashMap&);
		string _message;
	};

	HashMap(const unsigned int alloc=100)
		: _id(++_freeID), _values(alloc), _keys(alloc)
	{
	#ifndef NDEBUG
		cout << "Hash Map ID - " << _id << " created" << endl;
	#endif
	}
	HashMap(const vector<Key>& keys, const vector<Value>& values, const unsigned int alloc=100)
		: _id(++_freeID), _values(alloc), _keys(alloc)
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
	HashMap(const Key keys[], const Value values[], const unsigned int size, const unsigned int alloc=100)
		: _id(++_freeID), _values(alloc), _keys(alloc)
	{
		size_t hashCode(0);
		for(unsigned int i(0);i<size;++i)
		{
			hashCode = getHashCode(keys[i]);
			_values[hashCode].push_back(values[i]);
			_keys[hashCode].push_back(keys[i]);
		}
	#ifndef NDEBUG
		cout << "Hash Map ID - " << _id << " created" << endl;
	#endif
	}
	HashMap(const HashMap<Key, Value>& map)
		: _id(++_freeID), _values(map._values), _keys(map._keys)
	{
	#ifndef NDEBUG
		cout << "Hash Map ID - " << _id << " copied" << endl;
	#endif
	}
	HashMap<Key, Value>& operator=(const HashMap<Key, Value>& map)
	{
		if(this!=&map)
		{
			_values = map._values;
			_keys = map._keys;
		}
		return *this;
	}
	~HashMap()
	{
	#ifndef NDEBUG
		cout << "Hash Map ID - " << _id << " destroyed" << endl;
	#endif
	}

	const Value& operator[](const Key& key) const
	{
		const size_t hashCode(getHashCode(key));
		if(!hashCode)
			throw BadHashMap("Map is empty");
		const vector<Key>* const region = &_keys[hashCode];
		for(unsigned int i(0);i<region->size();++i)
		{
			if((*region)[i]==key)
				return _values[hashCode][i];
		}
		throw BadHashMap("No such element " + key);
	}
	HashMap<Key, Value>& add(const Key& key, const Value& value)
	{
		const size_t hashCode(getHashCode(key));
		_values[hashCode].push_back(value);
		_keys[hashCode].push_back(key);
		return *this;
	}
	HashMap<Key, Value>& remove(const Key& key)
	{
		const unsigned int hashCode(getHashCode(key));
		for(unsigned int i(0);i<region->size();++i)
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
	const bool isEmpty() const
	{
		bool res(!_values.empty());
		for(unsigned int i(0);i<_values.size() && res;++i)
		{
			res = res && _values[i].empty();
		}
		return res;
	}
private:
	const size_t getHashCode(const Key& key) const
	{
		return _values.size() ? hash<Key>()(key)%_values.size() : 0;
	}

	vector<vector<Value>> _values;
	vector<vector<Key>> _keys;
	const unsigned int _id;
	static unsigned int _freeID;
};

template<class Key, class Value> unsigned int HashMap<Key, Value>::_freeID = 0;

#endif