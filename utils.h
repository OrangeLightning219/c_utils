#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define global_variable               static
#define local_persist                 static
#define internal                      static
#define pi32                          3.14159265359f
#define DEGREES_TO_RADIANS( degrees ) degrees *pi32 / 180.0f
#define RADIANS_TO_DEGREES( radians ) radians * 180.0f / pi32

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef float float32;
typedef double float64;

#if SLOW
    #define Assert( expression )    \
        if ( !( expression ) )      \
        {                           \
            int *volatile null = 0; \
            *null = 0;              \
        }
#else
    #define Assert( expression )
#endif

#define Kilobytes( value ) ( ( value ) *1024LL )
#define Megabytes( value ) ( Kilobytes( value ) * 1024LL )
#define Gigabytes( value ) ( Megabytes( value ) * 1024LL )
#define Terabytes( value ) ( Gigabytes( value ) * 1024LL )

#define ArrayCount( array ) ( ( int ) ( sizeof( array ) / sizeof( ( array )[ 0 ] ) ) )

#define CONCAT_INTERNAL( x, y ) x##y
#define CONCAT( x, y )          CONCAT_INTERNAL( x, y )

inline u32 SafeTruncateU64( u64 value )
{
    Assert( value <= 0xFFFFFFFF );
    return ( u32 ) value;
}

void ConcatenateStrings( char *sourceA, int countA, char *sourceB, int countB, //NOLINT
                         char *destination )
{
    for ( int index = 0; index < countA; ++index )
    {
        *destination++ = *sourceA++;
    }
    for ( int index = 0; index < countB; ++index )
    {
        *destination++ = *sourceB++;
    }
    *destination++ = '\0';
}

int StringLength( char *string ) //NOLINT
{
    int length = 0;
    while ( *string++ )
    {
        ++length;
    }
    return length;
}

bool IsWhitespace( char a ) //NOLINT
{
    return a == ' ' || a == '\t' || a == '\v' || a == '\f' || a == '\n' || a == '\r';
}

bool StringsAreEqual( char *a, u64 aLength, char *b ) //NOLINT
{
    char *at = b;
    for ( u64 index = 0; index < aLength; ++index, ++at )
    {
        if ( *at == 0 || a[ index ] != *at )
        {
            return false;
        }
    }
    return *at == 0;
}

#ifdef __cplusplus

template< typename T >
struct ExitScope
{
    T lambda;
    ExitScope( T lambda ) :
        lambda( lambda ) { }
    ~ExitScope() { lambda(); }
    ExitScope( const ExitScope & );

private:
    ExitScope &operator=( const ExitScope & );
};

class ExitScopeHelp
{
public:
    template< typename T >
    ExitScope< T > operator+( T t )
    {
        return t;
    }
};

    #define defer const auto &CONCAT( defer__, __LINE__ ) = ExitScopeHelp() + [ & ]()
#endif

#endif
