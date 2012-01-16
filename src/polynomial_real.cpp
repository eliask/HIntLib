/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration
 *
 *  Copyright (C) 2002,03,04,05  Rudolf Sch�rer <rudolf.schuerer@sbg.ac.at>
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

#define HINTLIB_LIBRARY_OBJECT

#include <HIntLib/polynomial_real.tcc>

#include <HIntLib/realfield.h>

namespace HIntLib
{
   // Polynomial<>

   HINTLIB_INSTANTIATE_POLYNOMIAL_NO_EQUAL (Real<real>)
   HINTLIB_INSTANTIATE_POLYNOMIAL_NO_EQUAL (Complex<real>)

   // PolynomialRing<>

   HINTLIB_INSTANTIATE_POLYNOMIALRING_REAL (RealField<real>)
   HINTLIB_INSTANTIATE_POLYNOMIALRING_COMPLEX (ComplexField<real>)
}
