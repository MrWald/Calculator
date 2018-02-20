
#pragma once
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <vector>

template <class Key, class Value> class HashMap
{
public:
	HashMap();
	HashMap(const vector<Key>&, const vector<Value>&);
	~HashMap();
	Value& operator[](const Key&);
	const Value& operator[](const Key&) const;
	void add(const Key&, const Value&);
	const Value& remove(const Key&);
	const bool isEmpty() const;
private:
	HashMap(const HashMap&);
	HashMap& operator=(const HashMap&);
	vector<vector<Value>> _values;
	const unsigned int _id;
	static unsigned int _freeID;
};

#endif