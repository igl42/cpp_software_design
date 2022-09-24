/**************************************************************************************************
*
* \file G27_StrongType.cpp
* \brief Guideline 27: Use CRTP for Static Mixin Classes
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <utility>


//---- <Addable.h> --------------------------------------------------------------------------------

template< typename Derived >
struct Addable
{
   friend Derived& operator+=( Derived& lhs, Derived const& rhs ) {
      lhs.get() += rhs.get();
      return lhs;
   }

   friend Derived operator+( Derived const& lhs, Derived const& rhs ) {
      return Derived{ lhs.get() + rhs.get() };
   }
};


//---- <Subtractable.h> ---------------------------------------------------------------------------

template< typename Derived >
struct Subtractable
{
   friend Derived& operator-=( Derived& lhs, Derived const& rhs ) {
      lhs.get() -= rhs.get();
      return lhs;
   }

   friend Derived operator-( Derived const& lhs, Derived const& rhs ) {
      return Derived{ lhs.get() - rhs.get() };
   }
};


//---- <IntegralArithmetic.h> ---------------------------------------------------------------------

//#include <Addable.h>
//#include <Subtractable.h>

template< typename Derived >
struct IntegralArithmetic
   : public Addable<Derived>
   , public Subtractable<Derived>
{};


//---- <Printable.h> ------------------------------------------------------------------------------

#include <iosfwd>

template< typename Derived >
struct Printable
{
   friend std::ostream& operator<<( std::ostream& os, Derived const& d )
   {
      os << d.get();
      return os;
   }
};


//---- <Swappable.h> ------------------------------------------------------------------------------

#include <utility>

template< typename Derived >
struct Swappable
{
   friend void swap( Derived& lhs, Derived& rhs )
   {
      using std::swap;  // Enable ADL
      swap( lhs.get(), rhs.get() );
   }
};


//---- <StrongType.h> -----------------------------------------------------------------------------

template< typename T, typename Tag, template<typename> class... Skills >
struct StrongType
   : public Skills< StrongType<T,Tag,Skills...> >...
{
 public:
   using value_type = T;

   explicit constexpr StrongType( T const& value ) : value_( value ) {}

   constexpr T&       get()       noexcept { return value_; }
   constexpr T const& get() const noexcept { return value_; }

   void swap( StrongType& other ) {
      using std::swap;
      swap( value_, other.value_ );
   }

 private:
   T value_;
};

template< typename T, typename Tag, template<typename> class... Skills >
std::ostream& operator<<( std::ostream& os, StrongType<T,Tag,Skills...> const& nt )
{
   os << nt.get();
   return os;
}

template< typename T, typename Tag, template<typename> class... Skills >
void swap( StrongType<T,Tag,Skills...>& a, StrongType<T,Tag,Skills...>& b )
{
   a.swap( b );
}


//---- <Distances.h> ------------------------------------------------------------------------------

//#include <StrongType.h>

template< typename T >
using Meter = StrongType<T,struct MeterTag,IntegralArithmetic,Printable,Swappable>;

template< typename T >
using Kilometer = StrongType<T,struct KilometerTag,IntegralArithmetic,Printable,Swappable>;


//---- <Person.h> ---------------------------------------------------------------------------------

//#include <StrongType.h>

using Surname = StrongType<std::string,struct SurnameTag,Printable,Swappable>;


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Distances.h>
#include <iostream>

int main()
{
   auto const m1 = Meter<long>{ 100 };
   auto const m2 = Meter<long>{  50 };

   auto const m3 = m1 + m2;  // Compiles and results in 150 meters

   std::cout << "\n m3  = " << m3 << "m\n\n";

   return EXIT_SUCCESS;
}
