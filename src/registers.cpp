#include <registers.h>
#include <magic_enum.hpp>
#include <stdexcept>

RegisterSet::RegisterSet ( uint16_t _AF, uint16_t _BC, uint16_t _DE, uint16_t _HL, uint16_t _SP, uint16_t _PC )
    :
    SP ( _SP ),
    PC ( _PC )
{
    r ( R16::AF, _AF );
    r ( R16::BC, _BC );
    r ( R16::DE, _DE );
    r ( R16::HL, _HL );
}

uint8_t RegisterSet::r ( Register8 rg ) const
{
    return r8bit[magic_enum::enum_integer ( rg )];
}

void RegisterSet::r ( Register8 rg, uint8_t _v )
{
    r8bit[magic_enum::enum_integer ( rg )] = _v;
}

uint16_t RegisterSet::r ( Register16 rg ) const
{
    switch ( rg ) {
    case Register16::AF:
        return ( r ( R8::A ) << 8 ) | r ( R8::F );
        break;
    case Register16::BC:
        return ( r ( R8::B ) << 8 ) | r ( R8::C );
        break;
    case Register16::DE:
        return ( r ( R8::D ) << 8 ) | r ( R8::E );
        break;
    case Register16::HL:
        return ( r ( R8::H ) << 8 ) | r ( R8::L );
        break;
    case Register16::SP:
        return SP;
    case Register16::PC:
        return PC;
    default:
        throw std::runtime_error{ "Invalid register" };
    }
}

void RegisterSet::r ( Register16 rg, uint16_t _v )
{
    switch ( rg ) {
    case R16::AF:
        r ( R8::F, _v & 0xFF );
        r ( R8::A, ( _v >> 8 ) & 0xFF );
        break;
    case R16::BC:
        r ( R8::C, _v & 0xFF );
        r ( R8::B, ( _v >> 8 ) & 0xFF );
        break;
    case R16::DE:
        r ( R8::E, _v & 0xFF );
        r ( R8::D, ( _v >> 8 ) & 0xFF );
        break;
    case R16::HL:
        r ( R8::L,_v & 0xFF );
        r ( R8::H, ( _v >> 8 ) & 0xFF );
        break;
    case Register16::SP:
        SP = _v;
        break;
    case Register16::PC:
        PC = _v;
        break;
    default:
        throw std::runtime_error{ "Invalid register" };
    }
}
