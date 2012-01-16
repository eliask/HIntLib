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

#ifndef HINTLIB_ONE_DIM_VECTOR_SPACE_H
#define HINTLIB_ONE_DIM_VECTOR_SPACE_H 1

#ifdef __GNUG__
#pragma interface
#endif

#include <HIntLib/defaults.h>

#ifdef HINTLIB_HAVE_OSTREAM
  #include <ostream>
#else
  #include <iostream>
#endif

#ifdef HINTLIB_HAVE_SSTREAM
  #include <sstream>
#else
  #include <HIntLib/fallback_sstream.h>
#endif

#include <HIntLib/algebra.h>

namespace HIntLib
{
   template<typename A>
   class OneDimVectorSpace : private A
   {
   public:
      typedef vectorspace_tag algebra_category;
      typedef nopolynomial_tag polynomial_category;

      typedef typename A::type type;
      typedef typename A::type scalar_type;
      typedef A scalar_algebra;
      typedef type& scalar_reference;

      OneDimVectorSpace (const A& a) : A (a) {}

      A getScalarAlgebra() const  { return *this; }

      unsigned dimension() const  { return 1; }

      using A::size;
      using A::element;
      using A::index;
      using A::printSuffix;
      using A::is0;
      using A::add;
      using A::addTo;
      using A::sub;
      using A::subFrom;
      using A::neg;
      using A::negate;
      using A::times;
      using A::mul;
      using A::additiveOrder;

      void printShort (std::ostream&, const type& x) const;
      void print      (std::ostream&, const type& x) const;

      scalar_type coord (const type& x, unsigned) const { return x; }
      scalar_reference coord  (type& x, unsigned) const { return x; }

      template<typename I> void toCoord (const type& x, I p) const  {  *p = x; }
      template<typename I> void fromCoord (type& x, I p) const  { x = *p; }

      type& scale (type& x, const scalar_type& a) const { return mulBy (x, a); }
   };

   template<typename A>
   void OneDimVectorSpace<A>::print (std::ostream &o, const type& x) const
   {
      std::ostringstream ss;
      ss.flags (o.flags());
      ss.precision (o.precision());
#ifdef HINTLIB_STREAMS_SUPPORT_LOCAL
      ss.imbue (o.getloc());
#endif

      ss << '(';
      A::print (ss, x);
      ss << ')';

      o << ss.str().c_str();
   }

   template<typename A>
   void OneDimVectorSpace<A>::printShort (std::ostream &o, const type& x) const
   {
      std::ostringstream ss;
      ss.flags (o.flags());
      ss.precision (o.precision());
#ifdef HINTLIB_STREAMS_SUPPORT_LOCAL
      ss.imbue (o.getloc());
#endif

      ss << '(';
      A::printShort (ss, x);
      ss << ')';

      o << ss.str().c_str();
   }

   template<typename A>
   std::ostream& operator<< (std::ostream &o, const OneDimVectorSpace<A> &v)
   {
      std::ostringstream ss;
      ss.flags (o.flags());
      ss.precision (o.precision());
#ifdef HINTLIB_STREAMS_SUPPORT_LOCAL
      ss.imbue (o.getloc());
#endif

      ss << v.getScalarAlgebra() << "^1";
      return o << ss.str().c_str();
   } 

} // namespace HIntLib

#endif
