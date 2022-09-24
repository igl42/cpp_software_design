/**************************************************************************************************
*
* \file G26_CRTP_2.cpp
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
#include <concepts>
#include <numeric>
#include <ostream>

struct DenseVectorTag {};

template< typename T >
struct IsDenseVector
   : public std::is_base_of<DenseVectorTag,T>
{};

template< typename T >
constexpr bool IsDenseVector_v = IsDenseVector<T>::value;

template< typename T >
concept DenseVector =
   requires ( T t, size_t index ) {
      t.size();
      t[index];
      { t.begin() } -> std::same_as<typename T::iterator>;
      { t.end() } -> std::same_as<typename T::iterator>;
   } &&
   requires ( T const t, size_t index ) {
      t[index];
      { t.begin() } -> std::same_as<typename T::const_iterator>;
      { t.end() } -> std::same_as<typename T::const_iterator>;
   } &&
   IsDenseVector_v<T>;

template< DenseVector VectorT >
std::ostream& operator<<( std::ostream& os, VectorT const& vector )
{
   size_t const size( vector.size() );

   os << "(";
   for( size_t i=0UL; i<size; ++i ) {
      os << " " << vector[i];
   }
   os << " )";

   return os;
}

template< DenseVector VectorT >
decltype(auto) l2norm( VectorT const& vector )
{
   using T = typename VectorT::value_type;
   return std::sqrt( std::inner_product( std::begin(vector), std::end(vector)
                                       , std::begin(vector), T{} ) );
}


//---- <DynamicVector.h> --------------------------------------------------------------------------

//#include <DenseVector.h>
#include <vector>
#include <initializer_list>

template< typename T >
class DynamicVector : private DenseVectorTag
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

template< typename T, size_t Size >
struct IsDenseVector< StaticVector<T,Size> >
   : public std::true_type
{};


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

