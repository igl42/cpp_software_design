/**************************************************************************************************
*
* \file G26_CRTP_1.cpp
* \brief Guideline 26: Use CRTP to Introduce Static Type Categories
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <DenseVector.h> ----------------------------------------------------------------------------

#include <cmath>
#include <numeric>
#include <ostream>

template< typename Derived >
struct DenseVector
{
   constexpr Derived&       derived()       noexcept { return static_cast<Derived&>(*this); }
   constexpr Derived const& derived() const noexcept { return static_cast<Derived const&>(*this); }

   constexpr size_t size() const noexcept { return derived().size(); }

   decltype(auto) operator[]( size_t index )       noexcept { return derived()[index]; }
   decltype(auto) operator[]( size_t index ) const noexcept { return derived()[index]; }

   decltype(auto) begin()       noexcept { return derived().begin(); }
   decltype(auto) begin() const noexcept { return derived().begin(); }
   decltype(auto) end()         noexcept { return derived().end(); }
   decltype(auto) end()   const noexcept { return derived().end(); }
};

template< typename Derived >
std::ostream& operator<<( std::ostream& os, DenseVector<Derived> const& vector )
{
   size_t const size( vector.size() );

   os << "(";
   for( size_t i=0UL; i<size; ++i ) {
      os << " " << vector[i];
   }
   os << " )";

   return os;
}

template< typename Derived >
decltype(auto) l2norm( DenseVector<Derived> const& vector )
{
   using T = typename Derived::value_type;
   return std::sqrt( std::inner_product( std::begin(vector), std::end(vector)
                                       , std::begin(vector), T{} ) );
}


//---- <DynamicVector.h> --------------------------------------------------------------------------

//#include <DenseVector.h>
#include <vector>
#include <initializer_list>

template< typename T >
class DynamicVector
   : public DenseVector< DynamicVector<T> >
{
 public:
   using value_type     = T;
   using iterator       = typename std::vector<T>::iterator;
   using const_iterator = typename std::vector<T>::const_iterator;

   DynamicVector() = default;
   DynamicVector( std::initializer_list<T> init )
      : values_( std::begin(init), std::end(init) )
   {}

   size_t size() const noexcept { return values_.size(); }

   T&       operator[]( size_t index )       noexcept { return values_[index]; }
   T const& operator[]( size_t index ) const noexcept { return values_[index]; }

   iterator       begin()       noexcept { return values_.begin(); }
   const_iterator begin() const noexcept { return values_.begin(); }
   iterator       end()         noexcept { return values_.end(); }
   const_iterator end()   const noexcept { return values_.end(); }

   // ... Many numeric functions

 private:
   std::vector<T> values_;
};


//---- <StaticVector.h> ---------------------------------------------------------------------------

//#include <DenseVector.h>
#include <array>
#include <initializer_list>

template< typename T, size_t Size >
class StaticVector
   : public DenseVector< StaticVector<T,Size> >
{
 public:
   using value_type     = T;
   using iterator       = typename std::array<T,Size>::iterator;
   using const_iterator = typename std::array<T,Size>::const_iterator;

   StaticVector() = default;
   StaticVector( std::initializer_list<T> init )
   {
      std::copy( std::begin(init), std::end(init), std::begin(values_) );
   }

   size_t size() const noexcept { return values_.size(); }

   T&       operator[]( size_t index )       noexcept { return values_[index]; }
   T const& operator[]( size_t index ) const noexcept { return values_[index]; }

   iterator       begin()       noexcept { return values_.begin(); }
   const_iterator begin() const noexcept { return values_.begin(); }
   iterator       end()         noexcept { return values_.end(); }
   const_iterator end()   const noexcept { return values_.end(); }

   // ... Many numeric functions

 private:
   std::array<T,Size> values_;
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <DynamicVector.h>
//#include <StaticVector.h>
#include <cstdlib>
#include <iostream>

int main()
{
   DynamicVector<int> const a{ 1, 2, 3 };
   StaticVector<int,4UL> const b{ 4, 5, 6, 7 };

   std::cout << "\n"
             << " a = " << a << ", L2-norm = " << l2norm(a) << "\n"
             << " b = " << b << ", L2-norm = " << l2norm(b) << "\n"
             << "\n";

   return EXIT_SUCCESS;
}

