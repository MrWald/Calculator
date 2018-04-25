//
// Created by Vladimir Fomin on 4/25/18.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "AComplex.h"
#include "Evaluator.h"

size_t AComplex::_freeId(0);

const double AComplex::parseDouble(const int& startPos, int& endPos, const Evaluator<AComplex, Function>& ev, const Evaluator<AComplex, Function>& (Evaluator<AComplex, Function>::*move)()const)
{
    if ((ev.getExpression()[startPos] == 'P' && ev.getExpression()[startPos+1] == 'I')
        || (ev.getExpression()[startPos] == 'p' && ev.getExpression()[startPos+1] == 'i'))
    {
        (ev.*move)();
        (ev.*move)();
        return M_PI;
    }
    else if (ev.getExpression()[startPos] == 'e' || ev.getExpression()[startPos] == 'E')
    {
        (ev.*move)();
        return M_E;
    }
    else
    {
        while ((ev.getExpression()[endPos] >= '0' && ev.getExpression()[endPos] <= '9') || ev.getExpression()[endPos] == '.')
        {
            ++endPos;
            (ev.*move)();
        }
        //Parsing read real/im number part
        return atof((ev.getExpression().substr(startPos, endPos-startPos)).c_str());
    }
}

AComplex::AComplex (const double re, const double im)
        : _id(++_freeId), _re(re), _im(im)
{}

AComplex::AComplex (const AComplex& aComplex)
        : _id(++_freeId), _re(aComplex._re), _im(aComplex._im)
{}

AComplex::AComplex (const TComplex& tComplex) : _id(++_freeId), _re(tComplex.re()), _im(tComplex.im()) {}

AComplex::~AComplex()
{}

AComplex& AComplex::operator= (const AComplex& complex)
{
    if(this != &complex)
    {
        _re=complex._re;
        _im=complex._im;
    }
    return *this;
}

AComplex::TComplex::TComplex(const double ro, const double phi)
        : _ro(ro), _phi(limitPhi(phi))
{}

AComplex::TComplex::TComplex(const AComplex& a)
        : _ro(a.mod()), _phi(a.arg())
{}

const double AComplex::TComplex::re() const
{
    double res(_ro*cos(_phi));
    if(abs(res - static_cast<int>(res))<=1E-9)
        res=static_cast<int>(res);
    else if(abs(res - static_cast<int>(res) -1 )<=1E-9)
        res=static_cast<int>(res) + 1;
    return res;
}

const double AComplex::TComplex::im() const
{
    double res(_ro*sin(_phi));
    if(abs(res - static_cast<int>(res))<=1E-9)
        res=static_cast<int>(res);
    else if(abs(res - static_cast<int>(res) -1 )<=1E-9)
        res=static_cast<int>(res) + 1;
    return res;
}

const double AComplex::TComplex::limitPhi(const double phi)
{
    double posPhi(phi);
    const double _2_PI(2*M_PI);
    if(posPhi<0)
        posPhi += static_cast<int>(1 + (posPhi/(-_2_PI)))*_2_PI;
    else if(posPhi >= _2_PI)
        posPhi -= static_cast<int>(posPhi/_2_PI)*_2_PI;
    return posPhi;
}
const AComplex::TComplex AComplex::TComplex::power(const TComplex& c, const double n)
{
    return TComplex( pow(c._ro, static_cast<int>(n)), n*c._phi );
}

AComplex::operator const double() const
{
    return _re;
}
double& AComplex::re()
{
    return _re;
}
double& AComplex::im()
{
    return _im;
}

const double AComplex::re() const
{
    return _re;
}
const double AComplex::im() const
{
    return _im;
}

const double AComplex::mod() const
{
    return sqrt(_re*_re + _im*_im);
}
const double AComplex::arg() const
{
    double res(0);
    if(!_re)
    {
        if(_im<0)
        {
            res=3*M_PI_2;
        }
        else
        {
            res=M_PI_2;
        }
    }
    else
    {
        if(!_im)
        {
            res=(_re>0?0:M_PI);
        }
        else
        {
            res=atan(_im/_re);
            if(_re<0)
            {
                if(_im>=0)
                    res+=M_PI;
                else
                    res-=M_PI;
            }
        }
    }
    res=TComplex::limitPhi(res);
    if(abs(res - static_cast<int>(res))<=1E-9)
        res=static_cast<int>(res);
    else if(abs(res - static_cast<int>(res) -1 )<=1E-9)
        res=static_cast<int>(res) + 1;
    return res;
}

const size_t AComplex::getId() const
{
    return _id;
}
const size_t AComplex::amount()
{
    return _freeId;
}

//	For parsing with Evaluator
const AComplex AComplex::read(const Evaluator<AComplex, Function>& ev, const int startPos, const Evaluator<AComplex, Function>& (Evaluator<AComplex, Function>::*move)()const)
{
    AComplex res;
    int endPos(startPos);
    const double readD1(parseDouble(startPos, endPos, ev, move));
    if(ev.getExpression()[endPos]=='i')
    {
        res.im() = readD1 + (endPos==startPos);
        (ev.*move)();
    }
    else
        res.re() = readD1;
    return res;
}

//	Determining whether read by Evaluator expression is of this type
const bool AComplex::isElement(const string& expr, const int currentPos)
{
    const char currentChar(expr[currentPos]);
    return (currentChar >= '0' && currentChar <= '9') || currentChar == '.' || currentChar == 'i' ;

}
//	Applying function of specified signature, because Evaluator doesn't know how many arguments are passed to function of our signature
const AComplex AComplex::apply(const Evaluator<AComplex, Function>& ev, const AComplex (Evaluator<AComplex, Function>::*parseExpression)()const, const AComplex& current, const Function& currentF)
{
    return currentF(current);
}

const AComplex AComplex::conj(const AComplex& ac)
{
    return AComplex(ac.re(), -ac.im());
}

const AComplex AComplex::power(const AComplex& ac, const AComplex& n)
{
    return TComplex::power(ac, n);
}

AComplex& operator+=(AComplex& c1, const AComplex& c2)
{
    c1.re()+=c2.re();
    c1.im()+=c2.im();
    return c1;
}
AComplex& operator-=(AComplex& c1, const AComplex& c2)
{
    c1.re()-=c2.re();
    c1.im()-=c2.im();
    return c1;
}
AComplex& operator*=(AComplex& c1, const AComplex& c2)
{
    const double c1ReCopy(c1.re()), c1ImCopy(c1.im());
    c1.re()=c1ReCopy*c2.re() - c1ImCopy*c2.im();
    c1.im()=c1ReCopy*c2.im() + c1ImCopy*c2.re();
    return c1;
}
AComplex& operator/=(AComplex& c1, const AComplex& c2)
{
    if(!c2.re() && !c2.im())
        throw invalid_argument("Can't divide by zero");
    const double c1ReCopy(c1.re()), c1ImCopy(c1.im());
    c1.re() = (c1ReCopy * c2.re() + c1ImCopy * c2.im()) / (c2.re() * c2.re() + c2.im() * c2.im());
    c1.im() = (c2.re() * c1ImCopy - c1ReCopy * c2.im()) / (c2.re() * c2.re() + c2.im() * c2.im());
    return c1;
}

const AComplex operator+ (const AComplex& c1, const AComplex& c2)
{
    return AComplex(c1.re() + c2.re(), c1.im() + c2.im());
}
const AComplex operator- (const AComplex& c1, const AComplex& c2)
{
    return AComplex(c1.re() - c2.re(), c1.im() - c2.im());
}

const AComplex operator+ (const AComplex& ac)
{
    return ac;
}
const AComplex operator- (AComplex ac)
{
    ac.re()=-ac.re();
    ac.im()=-ac.im();
    return ac;
}

const bool operator== (const AComplex& c1, const AComplex& c2)
{
    return abs(c1.re()-c2.re())<=1E-9 && abs(c1.im()-c2.im())<=1E-9;
}
const bool operator!= (const AComplex& c1, const AComplex& c2)
{
    return !(c1==c2);
}

ostream& operator<<(ostream& out, const AComplex& c)
{
    //out << "(z" << c.getId() << " = ";
    out << '(';
    const double re(c.re());
    double im(c.im());
    if(re)
    {
        out << re;
        if(im)
        {
            out << ' ' << (im > 0 ? '+' : '-') << ' ';
            im=abs(im);
        }
    }
    if(im)
    {
        if(im!=1)
        {
            if(im==-1)
                out << '-';
            else
                out << im << '*';
        }
        out << 'i';
    }
    else if(!re)
        out << 0;
    out << ')';
    return out;
}

istream& operator>>(istream& in, AComplex& c)
{
    cout << "a(z" << c.getId() << ") = ";
    in >> c.re();
    cout << "b(z" << c.getId() << ") = ";
    in >> c.im();
    return in;
}