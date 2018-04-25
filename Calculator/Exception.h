//*******************************************
//    Control Work
//	  Tasks 1 and 7-10
//    Developed by Volodymyr Fomin
//    on 4/18/2018
//    Version 1.0
//*******************************************
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <ostream>
using namespace std;

class Exception
{
private:
   const string _reason;
   Exception& operator=(const Exception&);
public:
	explicit Exception(const string& = "");

	Exception(const Exception&);

	virtual ~Exception() = 0;

	virtual ostream& diagnose(ostream& out)const;
};

ostream& operator<<(ostream&, const Exception&);

#endif