/******************************************************************************
 * funcs_arm.h
 *
 * All Test Functions in 32-bit ARM assembly code: they are codenamed as
 * Scan/Perm Read/Write 32 Ptr/Index Simple/Unroll Loop.
 *
 * Scan = consecutive scanning, Perm = walk permutation cycle.
 * Read/Write = obvious
 * 32 = size of access
 * Ptr = with pointer, Index = access as array[i]
 * Simple/Unroll = 1 or 16 operations per loop
 *
 ******************************************************************************
 * Copyright (C) 2013 Timo Bingmann <tb@panthema.net>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

// ****************************************************************************
// ----------------------------------------------------------------------------
// 32-bit Operations
// ----------------------------------------------------------------------------
// ****************************************************************************

// 32-bit writer in a simple loop (C version)
void cScanWrite32PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t* begin = (uint32_t*)memarea;
    uint32_t* end = begin + size / sizeof(uint32_t);
    uint32_t value = 0xC0FFEEEE;

    do {
        uint32_t* p = begin;
        do {
            *p++ = value;
        }
        while (p < end);
    }
    while (--repeats != 0);
}

//REGISTER(cScanWrite32PtrSimpleLoop, 4, 4);

// 32-bit writer in a simple loop (Assembler version)
void ScanWrite32PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "str    %[value], [ip], #4 \n"  // store and advance 4
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3");
}

REGISTER(ScanWrite32PtrSimpleLoop, 4, 4);

// 32-bit writer in an unrolled loop (Assembler version)
void ScanWrite32PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "str    %[value], [ip,#0*4] \n" // store
        "str    %[value], [ip,#1*4] \n" // store
        "str    %[value], [ip,#2*4] \n" // store
        "str    %[value], [ip,#3*4] \n" // store

        "str    %[value], [ip,#4*4] \n" // store
        "str    %[value], [ip,#5*4] \n" // store
        "str    %[value], [ip,#6*4] \n" // store
        "str    %[value], [ip,#7*4] \n" // store

        "str    %[value], [ip,#8*4] \n" // store
        "str    %[value], [ip,#9*4] \n" // store
        "str    %[value], [ip,#10*4] \n" // store
        "str    %[value], [ip,#11*4] \n" // store

        "str    %[value], [ip,#12*4] \n" // store
        "str    %[value], [ip,#13*4] \n" // store
        "str    %[value], [ip,#14*4] \n" // store
        "str    %[value], [ip,#15*4] \n" // store
        
        "add    ip, ip, #16*4 \n"       // add offset
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3");
}

REGISTER(ScanWrite32PtrUnrollLoop, 4, 4);

// 32-bit read in a simple loop (Assembler version)
void ScanRead32PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "ldr    %[value], [ip], #4 \n"  // retieve and advance 4
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3");
}

REGISTER(ScanRead32PtrSimpleLoop, 4, 4);

// 32-bit reader in an unrolled loop (Assembler version)
void ScanRead32PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "ldr    %[value], [ip,#0*4] \n" // retrieve
        "ldr    %[value], [ip,#1*4] \n" // retrieve
        "ldr    %[value], [ip,#2*4] \n" // retrieve
        "ldr    %[value], [ip,#3*4] \n" // retrieve

        "ldr    %[value], [ip,#4*4] \n" // retrieve
        "ldr    %[value], [ip,#5*4] \n" // retrieve
        "ldr    %[value], [ip,#6*4] \n" // retrieve
        "ldr    %[value], [ip,#7*4] \n" // retrieve

        "ldr    %[value], [ip,#8*4] \n" // retrieve
        "ldr    %[value], [ip,#9*4] \n" // retrieve
        "ldr    %[value], [ip,#10*4] \n" // retrieve
        "ldr    %[value], [ip,#11*4] \n" // retrieve

        "ldr    %[value], [ip,#12*4] \n" // retrieve
        "ldr    %[value], [ip,#13*4] \n" // retrieve
        "ldr    %[value], [ip,#14*4] \n" // retrieve
        "ldr    %[value], [ip,#15*4] \n" // retrieve
        
        "add    ip, ip, #16*4 \n"       // add offset
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3");
}

REGISTER(ScanRead32PtrUnrollLoop, 4, 4);

// -----------------------------------------------------------------------------

// 32-bit writer in an indexed loop (C version)
void cScanWrite32IndexSimpleLoop(void* _memarea, size_t _size, size_t repeats)
{
    uint32_t* memarea = (uint32_t*)_memarea;
    uint32_t size = _size / sizeof(uint32_t);
    uint32_t value = 0xC0FFEEEE;

    do {
        for (size_t i = 0; i < size; ++i)
            memarea[i] = value;
    }
    while (--repeats != 0);
}

REGISTER(cScanWrite32IndexSimpleLoop, 4, 4);

// 32-bit writer in an indexed loop (Assembler version)
void ScanWrite32IndexSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("1: \n" // start of repeat loop
        "mov    ip, #0 \n"              // ip = reset index
        "2: \n" // start of write loop
        "str    %[value], [%[memarea], ip] \n"  // store and advance 4
        "add    ip, #4 \n"
        // test write loop condition
        "cmp    ip, %[size] \n"         // compare to total size
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip");
}

REGISTER(ScanWrite32IndexSimpleLoop, 4, 4);

// 32-bit reader in an indexed loop (Assembler version)
void ScanRead32IndexSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("1: \n" // start of repeat loop
        "mov    ip, #0 \n"              // ip = reset index
        "2: \n" // start of write loop
        "ldr    %[value], [%[memarea], ip] \n"  // store and advance 4
        "add    ip, #4 \n"
        // test write loop condition
        "cmp    ip, %[size] \n"         // compare to total size
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip");
}

REGISTER(ScanRead32IndexSimpleLoop, 4, 4);

// ****************************************************************************
// ----------------------------------------------------------------------------
// 64-bit Operations
// ----------------------------------------------------------------------------
// ****************************************************************************

// 64-bit writer in a simple loop (C version)
void cScanWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    typedef std::pair<uint32_t,uint32_t> uint64;

    uint64* begin = (uint64*)memarea;
    uint64* end = begin + size / sizeof(uint64);
    uint32_t val32 = 0xC0FFEEEE;
    uint64 value = uint64(val32,val32);

    do {
        uint64* p = begin;
        do {
            *p++ = value;
        }
        while(p < end);
    }
    while (--repeats != 0);
}

//REGISTER(cScanWrite64PtrSimpleLoop, 8, 8);

// 64-bit writer in a simple loop (Assembler version)
void ScanWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "mov    r4, %[value] \n"            // r4+r5 = 64-bit value
        "mov    r5, %[value] \n"
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "strd   r4, [ip], #8 \n"        // store and advance 8
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3", "r4", "r5");
}

REGISTER(ScanWrite64PtrSimpleLoop, 8, 8);

// 64-bit writer in an unrolled loop (Assembler version)
void ScanWrite64PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "mov    r4, %[value] \n"            // r4+r5 = 64-bit value
        "mov    r5, %[value] \n"
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "strd   r4, [ip,#0*8] \n"       // store
        "strd   r4, [ip,#1*8] \n"       // store
        "strd   r4, [ip,#2*8] \n"       // store
        "strd   r4, [ip,#3*8] \n"       // store

        "strd   r4, [ip,#4*8] \n"       // store
        "strd   r4, [ip,#5*8] \n"       // store
        "strd   r4, [ip,#6*8] \n"       // store
        "strd   r4, [ip,#7*8] \n"       // store

        "strd   r4, [ip,#8*8] \n"       // store
        "strd   r4, [ip,#9*8] \n"       // store
        "strd   r4, [ip,#10*8] \n"      // store
        "strd   r4, [ip,#11*8] \n"      // store

        "strd   r4, [ip,#12*8] \n"      // store
        "strd   r4, [ip,#13*8] \n"      // store
        "strd   r4, [ip,#14*8] \n"      // store
        "strd   r4, [ip,#15*8] \n"      // store

        "add    ip, #16*8 \n"
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3", "r4", "r5");
}

REGISTER(ScanWrite64PtrUnrollLoop, 8, 8);

// 64-bit reader in a simple loop (Assembler version)
void ScanRead64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "mov    r4, %[value] \n"            // r4+r5 = 64-bit value
        "mov    r5, %[value] \n"
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "ldrd   r4, [ip], #8 \n"        // retrieve and advance 8
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3", "r4", "r5");
}

REGISTER(ScanRead64PtrSimpleLoop, 8, 8);

// 64-bit reader in an unrolled loop (Assembler version)
void ScanRead64PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    uint32_t value = 0xC0FFEEEE;

    asm("add    r3, %[memarea], %[size] \n" // r3 = memarea+size
        "mov    r4, %[value] \n"            // r4+r5 = 64-bit value
        "mov    r5, %[value] \n"
        "1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset loop iterator
        "2: \n" // start of write loop
        "ldrd   r4, [ip,#0*8] \n"       // retrieve
        "ldrd   r4, [ip,#1*8] \n"       // retrieve
        "ldrd   r4, [ip,#2*8] \n"       // retrieve
        "ldrd   r4, [ip,#3*8] \n"       // retrieve

        "ldrd   r4, [ip,#4*8] \n"       // retrieve
        "ldrd   r4, [ip,#5*8] \n"       // retrieve
        "ldrd   r4, [ip,#6*8] \n"       // retrieve
        "ldrd   r4, [ip,#7*8] \n"       // retrieve

        "ldrd   r4, [ip,#8*8] \n"       // retrieve
        "ldrd   r4, [ip,#9*8] \n"       // retrieve
        "ldrd   r4, [ip,#10*8] \n"      // retrieve
        "ldrd   r4, [ip,#11*8] \n"      // retrieve

        "ldrd   r4, [ip,#12*8] \n"      // retrieve
        "ldrd   r4, [ip,#13*8] \n"      // retrieve
        "ldrd   r4, [ip,#14*8] \n"      // retrieve
        "ldrd   r4, [ip,#15*8] \n"      // retrieve

        "add    ip, #16*8 \n" 
        // test write loop condition
        "cmp    ip, r3 \n"              // compare to end iterator
        "blo    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [value] "r" (value), [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "ip", "r3", "r4", "r5");
}

REGISTER(ScanRead64PtrUnrollLoop, 8, 8);

// ****************************************************************************
// ----------------------------------------------------------------------------
// Permutation Walking
// ----------------------------------------------------------------------------
// ****************************************************************************

// follow 32-bit permutation in a simple loop (C version)
void cPermRead32SimpleLoop(void* memarea, size_t, size_t repeats)
{
    uint32_t* begin = (uint32_t*)memarea;

    do {
        uint32_t* p = begin;
        do {
            p = (uint32_t*)*p;
        }
        while (p != begin);
    }
    while (--repeats != 0);
}

REGISTER_PERM(cPermRead32SimpleLoop, 4);

// follow 32-bit permutation in a simple loop (Assembler version)
void PermRead32SimpleLoop(void* memarea, size_t, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset iterator
        "2: \n" // start of loop
        "ldr    ip, [ip] \n"
        // test loop condition
        "cmp    ip, %[memarea] \n"      // compare to end iterator
        "bne    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [memarea] "r" (memarea), [repeats] "r" (repeats)
        : "ip");
}

REGISTER_PERM(PermRead32SimpleLoop, 4);

// follow 32-bit permutation in an unrolled loop (Assembler version)
void PermRead32UnrollLoop(void* memarea, size_t, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "mov    ip, %[memarea] \n"      // ip = reset iterator
        "2: \n" // start of loop
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"

        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"

        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"

        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        "ldr    ip, [ip] \n"
        // test loop condition
        "cmp    ip, %[memarea] \n"      // compare to end iterator
        "bne    2b \n"
        // test repeat loop condition
        "subs   %[repeats], %[repeats], #1 \n" // until repeats = 0
        "bne    1b \n"
        :
        : [memarea] "r" (memarea), [repeats] "r" (repeats)
        : "ip");
}

REGISTER_PERM(PermRead32UnrollLoop, 4);

// -----------------------------------------------------------------------------
