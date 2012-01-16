/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration 
 *
 *  Copyright (C) 2002  Rudolf Sch�rer <rudolf.schuerer@sbg.ac.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifdef __GNUG__
#pragma implementation

#ifndef HINTLIB_HAVE_LIMITS
#pragma implementation "fallback_limits.h"
#endif
#endif

#include <HIntLib/mymath.h>

#include <HIntLib/polynomial2.h>
#include <HIntLib/modulararithmetic.h>
#include <HIntLib/polynomial.h>

namespace L = HIntLib;

/**
 *  powInt()
 *
 *  Arbitrary powers to an integer exponent
 *
 *  Works for
 *     - integers
 *     - floats
 *     - Polynomial2
 */

template<class T>
T L::powInt (T x, unsigned exponent)
{
   T result(1);

   for(;;)
   {
      if (exponent & 1)  result *= x;
      if ((exponent >>= 1) == 0)  return result;
      x *= x;
   }
}


namespace HIntLib
{
#define HINTLIB_INSTANTIATE(X) template X powInt (X, unsigned);

   HINTLIB_INSTANTIATE (double)
#if HINTLIB_REAL != 2
   HINTLIB_INSTANTIATE (real)
#endif
   HINTLIB_INSTANTIATE (int)
   HINTLIB_INSTANTIATE (unsigned)
   HINTLIB_INSTANTIATE (unsigned long)
#ifdef HINTLIB_HAVE_UNSIGNED_LONG_LONG_INT
   HINTLIB_INSTANTIATE (unsigned long long)
#endif
   HINTLIB_INSTANTIATE (Polynomial2<u32>)
#ifdef HINTLIB_U32_NOT_EQUAL_U64
   HINTLIB_INSTANTIATE (Polynomial2<u64>)
#endif
#undef HINTLIB_INSTANTIATE
}


/**
 *  powerMod()
 */

template<class T>
T L::powerMod (T x, unsigned exponent, T p)
{
   T result (1);

   for (;;)
   {
      if (exponent & 1)  result = (result * x) % p;
      if ((exponent >>= 1) == 0)  return result;
      x = (x*x) % p;
   }
}

namespace HIntLib
{
#define HINTLIB_INSTANTIATE(X) template X powerMod (X, unsigned, X);

   HINTLIB_INSTANTIATE (unsigned)
   HINTLIB_INSTANTIATE (unsigned long)
   HINTLIB_INSTANTIATE (Polynomial2<u32>)
#ifdef HINTLIB_U32_NOT_EQUAL_U64
   HINTLIB_INSTANTIATE (Polynomial2<u64>)
#endif
#undef HINTLIB_INSTANTIATE
}


/**
 *  logInt()
 *
 *  Determines the base-_base_ logarithm of _x_, truncated to an int
 *
 *  logInt(0, base)  := -1
 */

template<class T> int L::logInt (T x, T base)
{
   if (base < 2)  throw InvalidLogBase (base);
   if (x == 0)  return -1;
   
   unsigned result = 0;
   x /= base;

   while (x)
   {
      x /= base;
      ++ result;
   }

   return result;
}

namespace HIntLib
{
#define HINTLIB_INSTANTIATE(X) template int logInt (X, X);

   HINTLIB_INSTANTIATE (unsigned)
   HINTLIB_INSTANTIATE (unsigned long)
#ifdef HINTLIB_HAVE_UNSIGNED_LONG_LONG_INT
   HINTLIB_INSTANTIATE (unsigned long long)
#endif
#undef HINTLIB_INSTANTIATE
}


