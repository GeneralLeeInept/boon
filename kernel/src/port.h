#include <stdint.h>

inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port) );
}

inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm ("inb %1, %0" : "=a"(value) : "Nd"(port) );
    return value;
}

inline void outw(uint16_t port, uint16_t value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port) );
}

inline uint16_t inw(uint16_t port)
{
    uint16_t value;
    asm ("inw %0, %1" : "=a"(value) : "Nd"(port) );
    return value;
}

inline void io_wait()
{
    outb(0x80, 0);
}
