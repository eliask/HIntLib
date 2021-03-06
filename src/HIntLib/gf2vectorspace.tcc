/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration
 *
 *  Copyright (C) 2002  Rudolf Schuerer <rudolf.schuerer@sbg.ac.at>
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

#ifndef HINTLIB_GF_2_VECTOR_SPACE_TCC
#define HINTLIB_GF_2_VECTOR_SPACE_TCC 1

#include <HIntLib/gf2vectorspace.h>

#include <HIntLib/output.h>


/******************************  GF2 Vector Space  ***************************/

/**
 * printShort()
 */

template<typename T>
void
HIntLib::GF2VectorSpace<T>::printShort (std::ostream& o, const type& v) const
{
   Private::Printer ss (o);

   ss << '(';
   for (int i = 0; i < dim; ++i)
   {
      if (i > 0)  ss << ',';
      ss << unsigned (coord(v, i));
   }
   ss << ')';
}

#ifdef HINTLIB_BUILD_WCHAR
template<typename T>
void
HIntLib::GF2VectorSpace<T>::printShort (std::wostream& o, const type& v) const
{
   Private::WPrinter ss (o);

   ss << L'(';
   for (int i = 0; i < dim; ++i)
   {
      if (i > 0)  ss << L',';
      ss << unsigned (coord(v, i));
   }
   ss << L')';
}
#endif


/**
 *  print()
 */

template<typename T>
void
HIntLib::GF2VectorSpace<T>::print (std::ostream& o, const type& v) const
{
   Private::Printer ss (o);

   printShort (ss, v);
   ss << " (2)";
}

#ifdef HINTLIB_BUILD_WCHAR
template<typename T>
void
HIntLib::GF2VectorSpace<T>::print (std::wostream& o, const type& v) const
{
   Private::WPrinter ss (o);

   printShort (ss, v);
   ss << L" (2)";
}
#endif


/********************  Instantiations  ***************************************/

#ifdef HINTLIB_BUILD_WCHAR
#define HINTLIB_INSTANTIATE_GF2VECTORSPACE_W(X) \
   template void GF2VectorSpace<X >::print(std::wostream&, const type&) const; \
   template void GF2VectorSpace<X >::printShort \
      (std::wostream&, const type&) const;
#else
#define HINTLIB_INSTANTIATE_GF2VECTORSPACE_W(X)
#endif

#define HINTLIB_INSTANTIATE_GF2VECTORSPACE(X) \
   HINTLIB_INSTANTIATE_GF2VECTORSPACE_W(X) \
   template void GF2VectorSpace<X >::print (std::ostream&, const type&) const; \
   template void GF2VectorSpace<X >::printShort \
      (std::ostream&, const type&) const;

#endif

