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

/**
 *  rule3gauss.cpp
 *
 *  Cubature rule of degree 3 with 2^n points.
 *  All points are inside the hypercube.
 *
 *  It is also presented in
 *     A. H. Stoud. Approximate Calculation of Multiple Integrals (1971)
 *  as formula Cn: 3-4
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <HIntLib/rule3gauss.h>

#include <HIntLib/defaultcubaturerulefactory.h>
#include <HIntLib/mymath.h>
#include <HIntLib/exception.h>


namespace L = HIntLib;

namespace
{
#if HINTLIB_STATIC_WORKS == 1
   const L::real r = 1.0 / sqrt(3.0);
#else
   L::real r;
#endif
}


/**
 *  The constructor is used primarily to initialize all the dimension dependent
 *  constatns and to allocate (dimension dependent) memory
 */

L::Rule3Gauss::Rule3Gauss (unsigned dim)
: OrbitRule (dim), a (dim), oneDivTwoPowDim (1.0 / (Index(1) << dim))
{
   checkDimensionNotZero (dim);
   checkDimensionLeq<std::numeric_limits<Index>::digits - 1> (dim);

   #if HINTLIB_STATIC_WORKS == 0
      r = 1.0 / sqrt(3.0);
   #endif
}


/**
 *  Do the actual function evaluation
 */

L::real L::Rule3Gauss::eval (Function &f, const Hypercube &h)
{
   const real * width = h.getWidth();
   
   for (unsigned i = 0; i < dim; ++i)  a [i] = width [i] * r;

   return h.getVolume() * evalR_Rfs (f, h.getCenter(), a) * oneDivTwoPowDim;
}

L::CubatureRuleFactory* L::Rule3Gauss::getFactory()
{
   return new DefaultCubatureRuleFactory<L::Rule3Gauss> ();
}
