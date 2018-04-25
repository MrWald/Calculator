//*******************************************
//    Control Work
//	  Tasks 1 and 7-10
//    Developed by Volodymyr Fomin
//    on 4/17/2018
//    Version 1.0
//*******************************************

#include "Exception.h"
#include <string>

Exception::Exception(const string& reason)
        : _reason(reason)
{}

Exception::Exception(const Exception& exception)
        : _reason(exception._reason)
{}

Exception::~Exception()
{}

ostream& Exception::diagnose(ostream& out)const
{
    return out << _reason;
}

ostream& operator<<(ostream& out, const Exception& exception)
{
    return exception.diagnose(out);
}