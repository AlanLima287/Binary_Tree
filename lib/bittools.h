#pragma once

/*
* Author
*    Alan Lima (https://github.com/AlanLima287/)
*
* BitTools
*    It's a not precompiled C++ library with wrapper function
*    for especific assembly instructions (BT, BTC, BTR, BTS)
*    or its behavior
*/

#ifndef __BIT_TOOLS_
#define __BIT_TOOLS_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <new>

#if __has_include("intrin.h") && (defined(_WIN32) || defined(_WIN64))
#include <intrin.h>
#endif

namespace BitTools {

   typedef unsigned long mword;

   const mword SHIFT = sizeof(mword) >= 4 ? (sizeof(mword) == 4 ? 5 : 6) : (sizeof(mword) + 2);
   const mword MASK = (1 << SHIFT) - 1;

   mword* construct(uint64_t, mword = 0);
   inline void destruct(mword*);

   // BT (Bit Test)
   inline bool getbit(mword*, uint64_t);

   // BTC (Bit Test and Complement)
   inline bool flipbit(mword*, uint64_t);

   // BTR (Bit Test and Reset)
   inline bool setbit_0(mword*, uint64_t);

   // BTD (Bit Test and Set)
   inline bool setbit_1(mword*, uint64_t);

   // Fills the boolean array with a pattern
   mword* initialize(mword*, uint64_t, mword = 0);
   uint64_t size(uint64_t size) { return (size + MASK) >> SHIFT; }

   // Print the bits
   void print_bits(mword*, uint64_t);
};

#include "bittools.cpp"

#endif /* __BIT_TOOLS_ */