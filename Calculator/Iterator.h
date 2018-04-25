//*******************************************
//    Control Work
//	  Tasks 1 and 7-10
//    Developed by Volodymyr Fomin
//    on 4/18/2018
//    Version 1.0
//*******************************************
#ifndef _ITERATOR_H
#define _ITERATOR_H

#include <ostream>
using namespace std;

template<class Container, class Elem>
class Iterator
{
public:
	virtual const Container& start() const=0;
	virtual const Container& next() const=0;
	virtual const Elem& current() const=0;
	virtual const bool end() const=0;
	virtual ~Iterator() {};
};

//Console Output
template<class Containter, class Elem>
ostream& operator<<(ostream& out, const Iterator<Containter, Elem>& iterator)
{
	out << "{ ";
	iterator.start();
	while(!iterator.end())
	{
		out << iterator.current();
		out << (iterator.next().end()?" ":", ");
	}
	iterator.start();
	out << '}';
	return out;
}

#endif