#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "platform.h"

namespace quby {
    
    
    class Exception : public std::exception
    {
    private:
        std::string msg;
    public :
        Exception();
        Exception( const Exception &rhs );
        virtual ~Exception() throw();
        virtual const char * what() const throw();
        template<typename T>
        inline Exception& operator<<( const T& t )
        {
            std::stringstream ss;
            ss << t;
            msg +=ss.str();
            return *this;
        }
    };
}

#endif
