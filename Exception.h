/*
 * Copyright © 2013 by Jonathan Doman <jonathan.doman@gmail.com>
 *
 * See COPYING for license terms
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <sstream>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// This macro allows you to easily define new exception classes to throw
//
// RUNTIME_EXCEPTION( MyException );
// ...
// throw MyException( 25 );
// ...
//
// You can pass any argument to the constructor that can be used in 
// a string stream.
// Alternatively, pass a format string and corresponding parameters
// to the variadic constructor.
//
// LOGIC_EXCEPTION( MyException );
// ...
// throw MyException( "Looped %d times instead of %d", i, 10 );
// ...

#define BASE_EXCEPTION(eName,eBase,constructArg)\
class eName : public eBase\
{\
public:\
   eName() : eBase(constructArg), _msg(#eName) {}\
   template<typename T>\
   eName( const T& a ) : eBase(constructArg), _msg(_errorStr(a)) {}\
   eName( const char* f, ... ) : eBase(constructArg) {\
      if( f == NULL )\
         f = "";\
      size_t len = strlen(f) + strlen(#eName) + 3;\
      char* fmt = new char[len];\
      snprintf( fmt, len, #eName": %s", f );\
      size_t size(50), n;\
      char* msg = NULL;\
      while( true ) {\
         va_list ap;\
         va_start( ap, f );\
         msg = static_cast<char*>(realloc( msg, size ));\
         n = vsnprintf( msg, size, fmt, ap );\
         va_end( ap );\
         msg = static_cast<char*>(realloc( msg, n+1 ));\
         if( n < size )\
            break;\
         size = n + 1;\
      }\
      delete [] fmt;\
      _msg = msg;\
      free( msg );\
   }\
   ~eName() throw() {} \
   const char* what() const throw() { return _msg.c_str(); }\
private:\
   template<typename T>\
   static std::string _errorStr( const T& a ) {\
      std::ostringstream error;\
      error << #eName": " << a;\
      return error.str();\
   }\
private:\
   std::string _msg;\
}

#define EXCEPTION(x)         BASE_EXCEPTION(x,std::exception,)
#define RUNTIME_EXCEPTION(x) BASE_EXCEPTION(x,std::runtime_error,"")
#define LOGIC_EXCEPTION(x)   BASE_EXCEPTION(x,std::logic_error,"")

// Some example ones that might be useful
RUNTIME_EXCEPTION(FileAccessException);
RUNTIME_EXCEPTION(InvalidArgumentException);
LOGIC_EXCEPTION(NotImplementedException);

#endif // !EXCEPTION_H
