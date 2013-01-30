/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration 
 *
 *  Copyright (C) 2002,03,04,05  Rudolf Schuerer <rudolf.schuerer@sbg.ac.at>
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
 *  Digital Net 2
 *  Digital Net 2 <T>
 *
 *  Constracts digital nets (and sequences) in base 2 based on
 *  GeneratorMatrix2 GF2VectorSpace
 */

#ifndef HINTLIB_DIGITAL_NET_2_H
#define HINTLIB_DIGITAL_NET_2_H 1

#include <HIntLib/defaults.h>

#ifdef HINTLIB_USE_INTERFACE_IMPLEMENTATION
#pragma interface
#endif

#include <HIntLib/generatormatrix2.h>
#include <HIntLib/qrnsequencebase.h>
#include <HIntLib/shiftscale.h>
#include <HIntLib/pointset.h>
#include <HIntLib/qmcintegration.h>
#include <HIntLib/gf2vectorspace.h>

namespace HIntLib
{

/*****************************************************************************/
/*****        Digital Net 2                                                ***/
/*****************************************************************************/

#ifdef HINTLIB_IEEE_MAGIC_WORKS
   template<class X> struct FloatType {};
   template<> struct FloatType<u32> {  typedef float  floatType; };
#ifdef HINTLIB_U32_NOT_EQUAL_U64
   template<> struct FloatType<u64> {  typedef double floatType; };
#endif
#endif


/**
 *  Digital Net 2
 *
 *  A Digital Net in base 2
 *
 *  T .. unsigned integer type at least  _prec_  bits
 *         usually either "u32" or "u64"
 */

template<class T>
class DigitalNet2: public QRNSequenceBase, public DigitalNet
{
public:
   const GeneratorMatrix2<T> & getGeneratorMatrix() const  { return c; }
   Index getOptimalNumber(Index max) const;

   void setCube (const Hypercube &);
   void randomize (PRNG &);
   void setDigitalShift (const T*);

protected:
   typedef GF2VectorSpace<T> A;
   typedef typename A::scalar_algebra SA;

   const int prec;
   A alg;
   SA scalAlg;
   GeneratorMatrix2<T> c;
   Array<T> x;      // current vector (size dim)
   Array<T> xStart; // Inital values for x (size dim)
   const Truncation trunc;
   const real trivialScale;
   ShiftScale ss;
#ifdef HINTLIB_IEEE_MAGIC_WORKS
   ShiftScale ssMagic;
   enum MODE {STANDARD, DIRECT} mode;
   typedef typename FloatType<T>::floatType floatType;
   enum { floatBits = std::numeric_limits<floatType>::digits - 1 };
#endif

   DigitalNet2
      (const GeneratorMatrix2<T> &, const Hypercube &,
       int m, Index index, bool equi, Truncation);

   void copyXtoP          (real*);
   void copyXtoPDontScale (real*);

   void resetX          (Index n, real*p) { resetX(n); copyXtoP         (p); }
   void resetXDontScale (Index n, real*p) { resetX(n); copyXtoPDontScale(p); }

private:
   void resetX (Index);
};


/**
 *  Digital Net 2 Gray
 *
 *  Traverses the net in gray-code order
 */

template<class T>
class DigitalNet2Gray : public DigitalNet2<T>
{
public:
   DigitalNet2Gray
      (const GeneratorMatrix2<T> & gm, const Hypercube& _h,
       int m, Index i, bool equi, DigitalNet::Truncation t,
       bool correct = true)
      : DigitalNet2<T> (gm, _h, m, i, equi, t)
      { if (correct)  this->c.prepareForGrayCode(); }

   DigitalNet2Gray
      (const GeneratorMatrix2<T> &gm, const Hypercube& _h,
       bool correct = true)
      : DigitalNet2<T> (gm, _h, gm.getM(), 0, false, this->FULL)
      { if (correct)  this->c.prepareForGrayCode(); }

   void first          (real*p, Index _n = 0)
      { this->resetX          (grayCode(this->n = _n), p); }
   void firstDontScale (real*p, Index _n = 0)
      { this->resetXDontScale (grayCode(this->n = _n), p); }

   void next          (real*);
   void nextDontScale (real*);
};

}  // namespace HIntLib


/**
 *  copy X to P
 */

template<class T>
inline
void HIntLib::DigitalNet2<T>::copyXtoP (real* point)
{
#ifdef HINTLIB_IEEE_MAGIC_WORKS
   if (      std::numeric_limits<floatType>::digits
          >= std::numeric_limits<real>::digits
       || mode == DIRECT)
   {
      floatType *p = reinterpret_cast<floatType*>(&x[0]);
      for (int d = 0; d < getDimension(); ++d)
      {
         point[d] = ssMagic[d] (p[d]);
      }
   }
   else
#endif
   for (int d = 0; d < getDimension(); ++d)  point[d] = ss[d] (x[d]);
}


/**
 *  copy X to P Dont Scale ()
 */

template<class T>
inline
void HIntLib::DigitalNet2<T>::copyXtoPDontScale (real* point)
{
#ifdef HINTLIB_IEEE_MAGIC_WORKS
   if (      std::numeric_limits<floatType>::digits
          >= std::numeric_limits<real>::digits
       || mode == DIRECT)
   {
      floatType *p = reinterpret_cast<floatType*>(&x[0]);
      for (int d = 0; d < getDimension(); ++d)  point[d] = p[d] + -1.0;
   }
   else
#endif
   for (int d = 0; d < getDimension(); ++d) point[d] = x[d] * trivialScale;
}


/**
 *  Gray :: next()
 *
 *  The next method updates the array x depending on n and the vectors in v
 *  Once x is updated, these values are converted to real numbers and returned.
 */

template<class T>
inline
void HIntLib::DigitalNet2Gray<T>::next (real* point)
{
   const T *vp = this->c (ls0 (this->n++));  // determine digit that changed
   const int DIM = this->getDimension();

   for (int d = 0; d != DIM; ++d)  this->alg.addTo (this->x[d], vp[d]);

   this->copyXtoP (point);
} 

template<class T>
inline
void HIntLib::DigitalNet2Gray<T>::nextDontScale (real* point)
{
   const T *vp = this->c (ls0 (this->n++)); // determine digit that changed
   const int DIM = this->getDimension();

   for (int d = 0; d != DIM; ++d)  this->alg.addTo (this->x[d], vp[d]);

   this->copyXtoPDontScale (point);
} 


/*****************************************************************************/
/*****        Digital Net  Point Set                                       ***/
/*****************************************************************************/

namespace HIntLib
{

/**
 *  Digital 2 Point Set
 */

class Digital2PointSet : public PartitionablePointSet
{
private:
   bool isRandomized;
   unsigned seed;

public:
#ifdef HINTLIB_IEEE_MAGIC_WORKS
   typedef u64 BaseType;
#else
   typedef Index BaseType;
#endif

   Digital2PointSet & enableRandomize () { isRandomized = true; return *this;}
   virtual void randomize (unsigned _seed) { seed = _seed; }

protected:
   Digital2PointSet () : isRandomized (false), seed (0) {}

   void performRandomization (DigitalNet2Gray<BaseType> &);

private:
   Digital2PointSet (const Digital2PointSet &);
   const Digital2PointSet & operator= (const Digital2PointSet &);
};


/**
 *  Digital Net 2 Point Set
 */

// Non-template base

class DigitalNet2PointSetBase : public Digital2PointSet
{
public:
   typedef DigitalNet::Truncation Truncation;

protected:
   const GeneratorMatrix2<BaseType> gm;
   bool copy;
   bool equi;
   Truncation trunc;
   Index index;
   const Hypercube *h;

   int calculateM (Index) const;

   DigitalNet2PointSetBase (
      const GeneratorMatrix& _gm, bool _equi, Truncation t, Index i)
      : gm(_gm), equi(_equi), trunc(t), index (i), h (0) {}

public:
   virtual void setCube (const Hypercube*);

   virtual bool doJobRep (real *, ReportingJob &, Index);
   virtual void doJobPartition (real *, Job &, Index, Index, Index);

   virtual Index getOptimalNumber (Index max, const Hypercube &);
};

// Template

template<class Sum>
class DigitalNet2PointSet: public DigitalNet2PointSetBase
{
public:
   DigitalNet2PointSet (
      const GeneratorMatrix& _gm,
      bool e = true, DigitalNet::Truncation t = DigitalNet::TRUNCATE,
      Index i = 0)
   : DigitalNet2PointSetBase(_gm, e, t, i) {}

   void integratePartition (real *, Integrand &, Index, Index, Index, Stat&);
   void integratePartition (real *, Integrand &, Index, Index, Index, StatVar&);
};

// Specializatioin for Sum=real

template<>
class DigitalNet2PointSet<real>: public DigitalNet2PointSetBase
{
public:
   DigitalNet2PointSet<real> (
      const GeneratorMatrix& gm, bool e = true, DigitalNet::Truncation t = DigitalNet::TRUNCATE, Index i = 0)
   : DigitalNet2PointSetBase(gm, e, t, i) {}

   void integratePartition (real *, Integrand &, Index, Index, Index, Stat&);
   void integratePartition (real *, Integrand &, Index, Index, Index, StatVar&);
};

} // namespcae HIntLib


/**
 *  DigitalNet2PointSet :: integrate()
 */

template<class Sum>
void HIntLib::DigitalNet2PointSet<Sum>::integratePartition
   (real* point, Integrand &f, Index num, Index begin, Index end, Stat& stat)
{
   DigitalNet2Gray<BaseType> net
      (gm, *h, calculateM (num), index, equi, trunc);
   performRandomization (net);

   Statistic<real, Sum> s;
   qmcIntegration (point, net, f, begin, end, s);
   stat = s;
}

template<class Sum>
void HIntLib::DigitalNet2PointSet<Sum>::integratePartition
   (real* point, Integrand &f, Index num, Index begin, Index end, StatVar& stat)
{
   DigitalNet2Gray<BaseType> net
      (gm, *h, calculateM (num), index, equi, trunc);
   performRandomization (net);

   StatisticVar<real, Sum> s;
   qmcIntegration (point, net, f, begin, end, s);
   stat = s;
}


/*****************************************************************************/
/*****        Digital Seq  Point Set                                       ***/
/*****************************************************************************/

namespace HIntLib
{

/**
 *  Digital Sequence 2 Point Set
 */

// Non-template base

class DigitalSeq2PointSetBase : public Digital2PointSet
{
public:
   void init()  { delete net; net = 0; offset = 0; }

protected:
   GeneratorMatrix2<BaseType> gm;
   DigitalNet2Gray<BaseType>* net;
   Index offset;
   const bool reset;

   void checkSize (const DigitalNet2<BaseType> &, Index) const;

   DigitalSeq2PointSetBase (const GeneratorMatrix& _gm, bool _reset)
      : gm (_gm), net (0), offset (0), reset (_reset) {}
   ~DigitalSeq2PointSetBase ();

public:
   virtual Index getOptimalNumber (Index max, const Hypercube &);
   virtual void setCube (const Hypercube *);

   virtual bool doJobRep       (real *, ReportingJob &, Index);
   virtual void doJobPartition (real *, Job &, Index, Index, Index);

   virtual void randomize (unsigned _seed);
};

// Template

template<class Sum>
class DigitalSeq2PointSet: public DigitalSeq2PointSetBase
{
public:
   DigitalSeq2PointSet (const GeneratorMatrix& gm, bool reset)
      : DigitalSeq2PointSetBase(gm, reset) {}

   void integratePartition (real *, Integrand &, Index, Index, Index, Stat&);
   void integratePartition (real *, Integrand &, Index, Index, Index, StatVar&);
};

// Specializatioin for Sum=real

template<>
class DigitalSeq2PointSet<real>: public DigitalSeq2PointSetBase
{
public:
   DigitalSeq2PointSet<real> (const GeneratorMatrix& gm, bool reset)
      : DigitalSeq2PointSetBase(gm, reset) {}

   void integratePartition (real *, Integrand &, Index, Index, Index, Stat&);
   void integratePartition (real *, Integrand &, Index, Index, Index, StatVar&);
};

} // namespace HIntLib


/**
 *  DigitalSeq2PointSet :: integrate()
 */

template<class Sum>
void HIntLib::DigitalSeq2PointSet<Sum>::integratePartition
   (real* point, Integrand &f, Index num, Index begin, Index end, Stat& stat)
{
   if (! reset)
   {
      begin  += offset;
      end    += offset;
      offset += num;
   }
   checkSize (*net, end);

   Statistic<real, Sum> s;
   qmcIntegration (point, *net, f, begin, end, s);
   stat = s;
}

template<class Sum>
void HIntLib::DigitalSeq2PointSet<Sum>::integratePartition
   (real* point, Integrand &f, Index num, Index begin, Index end, StatVar& stat)
{
   if (! reset)
   {
      begin  += offset;
      end    += offset;
      offset += num;
   }
   checkSize (*net, end);

   StatisticVar<real, Sum> s;
   qmcIntegration (point, *net, f, begin, end, s);
   stat = s;
}


#endif

