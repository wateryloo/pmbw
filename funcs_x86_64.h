/******************************************************************************
 * funcs_x86_64.h
 *
 * All Test Functions in 64-bit assembly code: they are codenamed as
 * Scan/Perm Read/Write 32/64/128/256 Ptr/Index Simple/Unroll Loop.
 *
 * Scan = consecutive scanning, Perm = walk permutation cycle.
 * Read/Write = obvious
 * 32/64/128/256 = size of access
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
// 64-bit Operations
// ----------------------------------------------------------------------------
// ****************************************************************************

// 64-bit writer in a simple loop (C version)
void cScanWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint64_t* begin = (uint64_t*)memarea;
    uint64_t* end = begin + size / sizeof(uint64_t);
    uint64_t value = 0xC0FFEEEEBABE0000;

    do {
        uint64_t* p = begin;
        do {
            *p++ = value;
        }
        while (p < end);
    }
    while (--repeats != 0);
}

//REGISTER(cScanWrite64PtrSimpleLoop, 8, 8);

// 64-bit writer in a simple loop (Assembler version)
void ScanWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n" // rax = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n" // start of write loop
        "mov    %%rax, (%%rcx) \n"
        "add    $8, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rcx", "rsi", "rdi");
}

REGISTER(ScanWrite64PtrSimpleLoop, 8, 8);

// 64-bit writer in an unrolled loop (Assembler version)
void ScanWrite64PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n"    // rax = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "mov    %%rax, 0*8(%%rcx) \n"
        "mov    %%rax, 1*8(%%rcx) \n"
        "mov    %%rax, 2*8(%%rcx) \n"
        "mov    %%rax, 3*8(%%rcx) \n"
        "mov    %%rax, 4*8(%%rcx) \n"
        "mov    %%rax, 5*8(%%rcx) \n"
        "mov    %%rax, 6*8(%%rcx) \n"
        "mov    %%rax, 7*8(%%rcx) \n"
        "mov    %%rax, 8*8(%%rcx) \n"
        "mov    %%rax, 9*8(%%rcx) \n"
        "mov    %%rax, 10*8(%%rcx) \n"
        "mov    %%rax, 11*8(%%rcx) \n"
        "mov    %%rax, 12*8(%%rcx) \n"
        "mov    %%rax, 13*8(%%rcx) \n"
        "mov    %%rax, 14*8(%%rcx) \n"
        "mov    %%rax, 15*8(%%rcx) \n"
        "add    $16*8, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rcx", "rsi", "rdi");
}

REGISTER(ScanWrite64PtrUnrollLoop, 8, 8);

// 64-bit reader in a simple loop (Assembler version)
void ScanRead64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "mov    (%%rcx), %%rax \n"
        "add    $8, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rcx", "rsi", "rdi");
}

REGISTER(ScanRead64PtrSimpleLoop, 8, 8);

// 64-bit reader in an unrolled loop (Assembler version)
void ScanRead64PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "mov    0*8(%%rcx), %%rax \n"
        "mov    1*8(%%rcx), %%rax \n"
        "mov    2*8(%%rcx), %%rax \n"
        "mov    3*8(%%rcx), %%rax \n"
        "mov    4*8(%%rcx), %%rax \n"
        "mov    5*8(%%rcx), %%rax \n"
        "mov    6*8(%%rcx), %%rax \n"
        "mov    7*8(%%rcx), %%rax \n"
        "mov    8*8(%%rcx), %%rax \n"
        "mov    9*8(%%rcx), %%rax \n"
        "mov    10*8(%%rcx), %%rax \n"
        "mov    11*8(%%rcx), %%rax \n"
        "mov    12*8(%%rcx), %%rax \n"
        "mov    13*8(%%rcx), %%rax \n"
        "mov    14*8(%%rcx), %%rax \n"
        "mov    15*8(%%rcx), %%rax \n"
        "add    $16*8, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rcx", "rsi", "rdi");
}

REGISTER(ScanRead64PtrUnrollLoop, 8, 8);

// -----------------------------------------------------------------------------

// 64-bit writer in an indexed loop (C version)
void cScanWrite64IndexSimpleLoop(void* _memarea, size_t _size, size_t repeats)
{
    uint64_t* memarea = (uint64_t*)_memarea;
    uint64_t size = _size / sizeof(uint64_t);
    uint64_t value = 0xC0FFEEEEBABE0000;

    do {
        for (size_t i = 0; i < size; ++i)
            memarea[i] = value;
    }
    while (--repeats != 0);
}

//REGISTER(cScanWrite64IndexSimpleLoop, 8, 8);

// 64-bit writer in an indexed loop (Assembler version)
void ScanWrite64IndexSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n" // rax = test value
        "1: \n" // start of repeat loop
        "xor    %%rcx, %%rcx \n"        // rcx = reset index
        "2: \n" // start of write loop
        "mov    %%rax, (%[memarea],%%rcx) \n"
        "add    $8, %%rcx \n"
        // test write loop condition
        "cmp    %[size], %%rcx \n"      // compare to total size
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "rax", "rcx");
}

REGISTER(ScanWrite64IndexSimpleLoop, 8, 8);

// 64-bit writer in an indexed unrolled loop (Assembler version)
void ScanWrite64IndexUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n" // rax = test value
        "1: \n" // start of repeat loop
        "xor    %%rcx, %%rcx \n"        // rcx = reset index
        "2: \n" // start of write loop
        "mov    %%rax, 0*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 1*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 2*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 3*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 4*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 5*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 6*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 7*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 8*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 9*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 10*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 11*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 12*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 13*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 14*8(%[memarea],%%rcx) \n"
        "mov    %%rax, 15*8(%[memarea],%%rcx) \n"
        "add    $16*8, %%rcx \n"
        // test write loop condition
        "cmp    %[size], %%rcx \n"      // compare to total size
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "rax", "rcx");
}

REGISTER(ScanWrite64IndexUnrollLoop, 8, 8);

// 64-bit reader in an indexed loop (Assembler version)
void ScanRead64IndexSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "xor    %%rcx, %%rcx \n"        // rcx = reset index
        "2: \n" // start of write loop
        "mov    (%[memarea],%%rcx), %%rax \n"
        "add    $8, %%rcx \n"
        // test read loop condition
        "cmp    %[size], %%rcx \n"      // compare to total size
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "rax", "rcx");
}

REGISTER(ScanRead64IndexSimpleLoop, 8, 8);

// 64-bit reader in an indexed unrolled loop (Assembler version)
void ScanRead64IndexUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "xor    %%rcx, %%rcx \n"        // rcx = reset index
        "2: \n" // start of write loop
        "mov    0*8(%[memarea],%%rcx), %%rax \n"
        "mov    1*8(%[memarea],%%rcx), %%rax \n"
        "mov    2*8(%[memarea],%%rcx), %%rax \n"
        "mov    3*8(%[memarea],%%rcx), %%rax \n"
        "mov    4*8(%[memarea],%%rcx), %%rax \n"
        "mov    5*8(%[memarea],%%rcx), %%rax \n"
        "mov    6*8(%[memarea],%%rcx), %%rax \n"
        "mov    7*8(%[memarea],%%rcx), %%rax \n"
        "mov    8*8(%[memarea],%%rcx), %%rax \n"
        "mov    9*8(%[memarea],%%rcx), %%rax \n"
        "mov    10*8(%[memarea],%%rcx), %%rax \n"
        "mov    11*8(%[memarea],%%rcx), %%rax \n"
        "mov    12*8(%[memarea],%%rcx), %%rax \n"
        "mov    13*8(%[memarea],%%rcx), %%rax \n"
        "mov    14*8(%[memarea],%%rcx), %%rax \n"
        "mov    15*8(%[memarea],%%rcx), %%rax \n"
        "add    $16*8, %%rcx \n"
        // test read loop condition
        "cmp    %[size], %%rcx \n"      // compare to total size
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [size] "r" (size), [repeats] "r" (repeats)
        : "rax", "rcx");
}

REGISTER(ScanRead64IndexUnrollLoop, 8, 8);

// -----------------------------------------------------------------------------

#if 0
static const int skiplen64 = 64; // one item per cache line

// 64-bit skipping writer in a simple loop (C version)
void cSkipWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    uint64_t* begin = (uint64_t*)memarea;
    uint64_t* end = begin + size / sizeof(uint64_t);
    uint64_t value = 0xC0FFEEEEBABE0000;

    do {
        uint64_t* p = begin;
        do {
            *p++ = value;
            p += skiplen64 / sizeof(uint64_t);
        }
        while (p < end);
    }
    while (--repeats != 0);
}

REGISTER(cSkipWrite64PtrSimpleLoop, 8, 8);

// 64-bit skipping writer in a simple loop (Assembler version)
void SkipWrite64PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n" // rax = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n" // start of write loop
        "mov    %%rax, (%%rcx) \n"
        "add    $8, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rcx", "rsi", "rdi");
}

REGISTER(SkipWrite64PtrSimpleLoop, 8, 8);
#endif

// ****************************************************************************
// ----------------------------------------------------------------------------
// 128-bit Operations
// ----------------------------------------------------------------------------
// ****************************************************************************

// 128-bit writer in a simple loop (C version)
void cScanWrite128PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    typedef std::pair<uint64_t,uint64_t> uint128;

    uint128* begin = (uint128*)memarea;
    uint128* end = begin + size / sizeof(uint128);
    uint64_t val64 = 0xC0FFEEEEBABE0000;
    uint128 value = uint128(val64,val64);

    do {
        uint128* p = begin;
        do {
            *p++ = value;
        }
        while(p < end);
    }
    while (--repeats != 0);
}

//REGISTER(cScanWrite128PtrSimpleLoop, 16, 16);

// 128-bit writer in a simple loop (Assembler version)
void ScanWrite128PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n"
        "movq   %%rax, %%xmm0 \n"
        "movq   %%rax, %%xmm1 \n"
        "movlhps %%xmm0, %%xmm1 \n"     // xmm0 = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rax \n"        // rax = reset loop iterator
        "2: \n" // start of write loop
        "movdqa %%xmm0, (%%rax) \n"
        "add    $16, %%rax \n"
        // test write loop condition
        "cmp    %%rdi, %%rax \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rsi", "rdi", "xmm0", "xmm1");
}

REGISTER(ScanWrite128PtrSimpleLoop, 16, 16);

// 128-bit writer in an unrolled loop (Assembler version)
void ScanWrite128PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n"
        "movq   %%rax, %%xmm0 \n"
        "movq   %%rax, %%xmm1 \n"
        "movlhps %%xmm0, %%xmm1 \n"     // xmm0 = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rax \n"        // rax = reset loop iterator
        "2: \n" // start of write loop
        "movdqa %%xmm0, 0*16(%%rax) \n"
        "movdqa %%xmm0, 1*16(%%rax) \n"
        "movdqa %%xmm0, 2*16(%%rax) \n"
        "movdqa %%xmm0, 3*16(%%rax) \n"
        "movdqa %%xmm0, 4*16(%%rax) \n"
        "movdqa %%xmm0, 5*16(%%rax) \n"
        "movdqa %%xmm0, 6*16(%%rax) \n"
        "movdqa %%xmm0, 7*16(%%rax) \n"
        "movdqa %%xmm0, 8*16(%%rax) \n"
        "movdqa %%xmm0, 9*16(%%rax) \n"
        "movdqa %%xmm0, 10*16(%%rax) \n"
        "movdqa %%xmm0, 11*16(%%rax) \n"
        "movdqa %%xmm0, 12*16(%%rax) \n"
        "movdqa %%xmm0, 13*16(%%rax) \n"
        "movdqa %%xmm0, 14*16(%%rax) \n"
        "movdqa %%xmm0, 15*16(%%rax) \n"
        "add    $16*16, %%rax \n"
        // test write loop condition
        "cmp    %%rdi, %%rax \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rsi", "rdi", "xmm0", "xmm1");
}

REGISTER(ScanWrite128PtrUnrollLoop, 16, 16);

// 128-bit reader in a simple loop (Assembler version)
void ScanRead128PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n"
        "movq   %%rax, %%xmm0 \n"
        "movq   %%rax, %%xmm1 \n"
        "movlhps %%xmm0, %%xmm1 \n"     // xmm0 = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rax \n"        // rax = reset loop iterator
        "2: \n" // start of read loop
        "movdqa (%%rax), %%xmm0 \n"
        "add    $16, %%rax \n"
        // test read loop condition
        "cmp    %%rdi, %%rax \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rsi", "rdi", "xmm0", "xmm1");
}

REGISTER(ScanRead128PtrSimpleLoop, 16, 16);

// 128-bit reader in an unrolled loop (Assembler version)
void ScanRead128PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEEBABE0000, %%rax \n"
        "movq   %%rax, %%xmm0 \n"
        "movq   %%rax, %%xmm1 \n"
        "movlhps %%xmm0, %%xmm1 \n"     // xmm0 = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rax \n"        // rax = reset loop iterator
        "2: \n" // start of read loop
        "movdqa 0*16(%%rax), %%xmm0 \n"
        "movdqa 1*16(%%rax), %%xmm0 \n"
        "movdqa 2*16(%%rax), %%xmm0 \n"
        "movdqa 3*16(%%rax), %%xmm0 \n"
        "movdqa 4*16(%%rax), %%xmm0 \n"
        "movdqa 5*16(%%rax), %%xmm0 \n"
        "movdqa 6*16(%%rax), %%xmm0 \n"
        "movdqa 7*16(%%rax), %%xmm0 \n"
        "movdqa 8*16(%%rax), %%xmm0 \n"
        "movdqa 9*16(%%rax), %%xmm0 \n"
        "movdqa 10*16(%%rax), %%xmm0 \n"
        "movdqa 11*16(%%rax), %%xmm0 \n"
        "movdqa 12*16(%%rax), %%xmm0 \n"
        "movdqa 13*16(%%rax), %%xmm0 \n"
        "movdqa 14*16(%%rax), %%xmm0 \n"
        "movdqa 15*16(%%rax), %%xmm0 \n"
        "add    $16*16, %%rax \n"
        // test read loop condition
        "cmp    %%rdi, %%rax \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "rax", "rsi", "rdi", "xmm0", "xmm1");
}

REGISTER(ScanRead128PtrUnrollLoop, 16, 16);

// ****************************************************************************
// ----------------------------------------------------------------------------
// 32-bit Operations
// ----------------------------------------------------------------------------
// ****************************************************************************

// 32-bit writer in a simple loop (Assembler version)
void ScanWrite32PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEE, %%eax \n"  // eax = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n" // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n" // start of write loop
        "movl   %%eax, (%%rcx) \n"
        "add    $4, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "eax", "rcx", "rsi", "rdi");
}

REGISTER(ScanWrite32PtrSimpleLoop, 4, 4);

// 32-bit writer in an unrolled loop (Assembler version)
void ScanWrite32PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    $0xC0FFEEEE, %%eax \n"  // eax = test value
        "mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "movl   %%eax, 0*4(%%rcx) \n"
        "movl   %%eax, 1*4(%%rcx) \n"
        "movl   %%eax, 2*4(%%rcx) \n"
        "movl   %%eax, 3*4(%%rcx) \n"
        "movl   %%eax, 4*4(%%rcx) \n"
        "movl   %%eax, 5*4(%%rcx) \n"
        "movl   %%eax, 6*4(%%rcx) \n"
        "movl   %%eax, 7*4(%%rcx) \n"
        "movl   %%eax, 8*4(%%rcx) \n"
        "movl   %%eax, 9*4(%%rcx) \n"
        "movl   %%eax, 10*4(%%rcx) \n"
        "movl   %%eax, 11*4(%%rcx) \n"
        "movl   %%eax, 12*4(%%rcx) \n"
        "movl   %%eax, 13*4(%%rcx) \n"
        "movl   %%eax, 14*4(%%rcx) \n"
        "movl   %%eax, 15*4(%%rcx) \n"
        "add    $16*4, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "eax", "rcx", "rsi", "rdi");
}

REGISTER(ScanWrite32PtrUnrollLoop, 4, 4);

// 32-bit reader in a simple loop (Assembler version)
void ScanRead32PtrSimpleLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "movl   (%%rcx), %%eax \n"
        "add    $4, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "eax", "rcx", "rsi", "rdi");
}

REGISTER(ScanRead32PtrSimpleLoop, 4, 4);

// 32-bit reader in an unrolled loop (Assembler version)
void ScanRead32PtrUnrollLoop(void* memarea, size_t size, size_t repeats)
{
    asm("mov    %[memarea], %%rsi \n"   // rsi = memarea
        "mov    %[size], %%rdi \n"
        "add    %%rsi, %%rdi \n"        // rdi = memarea+size
        "1: \n"                         // start of repeat loop
        "mov    %%rsi, %%rcx \n"        // rcx = reset loop iterator
        "2: \n"                         // start of write loop
        "movl   0*4(%%rcx), %%eax \n"
        "movl   1*4(%%rcx), %%eax \n"
        "movl   2*4(%%rcx), %%eax \n"
        "movl   3*4(%%rcx), %%eax \n"
        "movl   4*4(%%rcx), %%eax \n"
        "movl   5*4(%%rcx), %%eax \n"
        "movl   6*4(%%rcx), %%eax \n"
        "movl   7*4(%%rcx), %%eax \n"
        "movl   8*4(%%rcx), %%eax \n"
        "movl   9*4(%%rcx), %%eax \n"
        "movl   10*4(%%rcx), %%eax \n"
        "movl   11*4(%%rcx), %%eax \n"
        "movl   12*4(%%rcx), %%eax \n"
        "movl   13*4(%%rcx), %%eax \n"
        "movl   14*4(%%rcx), %%eax \n"
        "movl   15*4(%%rcx), %%eax \n"
        "add    $16*4, %%rcx \n"
        // test write loop condition
        "cmp    %%rdi, %%rcx \n"        // compare to end iterator
        "jb     2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "g" (memarea), [size] "g" (size), [repeats] "r" (repeats)
        : "eax", "rcx", "rsi", "rdi");
}

REGISTER(ScanRead32PtrUnrollLoop, 4, 4);

// ****************************************************************************
// ----------------------------------------------------------------------------
// Permutation Walking
// ----------------------------------------------------------------------------
// ****************************************************************************

// follow 64-bit permutation in a simple loop (C version)
void cPermRead64SimpleLoop(void* memarea, size_t, size_t repeats)
{
    uint64_t* begin = (uint64_t*)memarea;

    do {
        uint64_t* p = begin;
        do {
            p = (uint64_t*)*p;
        }
        while (p != begin);
    }
    while (--repeats != 0);
}

//REGISTER_PERM(cPermRead64SimpleLoop, 8);

// follow 64-bit permutation in a simple loop (Assembler version)
void PermRead64SimpleLoop(void* memarea, size_t, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "mov    %[memarea], %%rax \n"   // rax = reset iterator
        "2: \n" // start of write loop
        "mov    (%%rax), %%rax \n"
        // test write loop condition
        "cmp    %%rax, %[memarea] \n"   // compare to first iterator
        "jne    2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [repeats] "r" (repeats)
        : "rax");
}

REGISTER_PERM(PermRead64SimpleLoop, 8);

// follow 64-bit permutation in an unrolled loop (Assembler version)
void PermRead64UnrollLoop(void* memarea, size_t, size_t repeats)
{
    asm("1: \n" // start of repeat loop
        "mov    %[memarea], %%rax \n"   // rax = reset iterator
        "2: \n" // start of write loop
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"

        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"

        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"

        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        "mov    (%%rax), %%rax \n"
        // test write loop condition
        "cmp    %%rax, %[memarea] \n"   // compare to first iterator
        "jne    2b \n"
        // test repeat loop condition
        "dec    %[repeats] \n"          // until repeats = 0
        "jnz    1b \n"
        :
        : [memarea] "r" (memarea), [repeats] "r" (repeats)
        : "rax");
}

REGISTER_PERM(PermRead64UnrollLoop, 8);

// -----------------------------------------------------------------------------
