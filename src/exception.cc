#include "exception.h"


using namespace std;
using namespace quby;

Exception::Exception () {}

Exception::Exception( const Exception &rhs )
{
    msg = rhs.msg;
}

Exception::~Exception () throw() {

}

const char * Exception::what() const throw()
{
    return msg.c_str();
}

