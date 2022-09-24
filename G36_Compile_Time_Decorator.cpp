/**************************************************************************************************
*
* \file G36_Compile_Time_Decorator.cpp
* \brief Guideline 36: Understand the Trade-off Between Runtime and Compile Time Abstraction
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Money.h> --------------------------------------------------------------------------------

#include <cmath>
#include <concepts>
#include <cstdint>
#include <ostream>

struct Money
{
   uint64_t value{};
};

template< typename T >
   requires std::is_arithmetic_v<T>
Money operator*( Money money, T factor )
{
   return Money{ static_cast<uint64_t>( money.value * factor ) };
}

constexpr Money operator+( Money lhs, Money rhs ) noexcept
{
   return Money{ lhs.value + rhs.value };
}

std::ostream& operator<<( std::ostream& os, Money money )
{
   return os << money.value;
}


//---- <ConferenceTicket.h> -----------------------------------------------------------------------

//#include <Money.h>
#include <string>
#include <utility>

class ConferenceTicket
{
 public:
   ConferenceTicket( std::string name, Money price )
      : name_{ std::move(name) }
      , price_{ price }
   {}

   std::string const& name() const { return name_; }
   Money price() const { return price_; }

 private:
   std::string name_;
   Money price_;
};


//---- <PricedItem.h> -----------------------------------------------------------------------------

//#include <Money.h>

template< typename T >
concept PricedItem =
   requires ( T item ) {
      { item.price() } -> std::same_as<Money>;
   };


//---- <Discounted.h> -----------------------------------------------------------------------------

//#include <Money.h>
//#include <PricedItem.h>
#include <utility>

template< double discount, PricedItem Item >
class Discounted  // Using composition
{
 public:
   template< typename... Args >
   explicit Discounted( Args&&... args )
      : item_{ std::forward<Args>(args)... }
   {}

   Money price() const {
      return item_.price() * ( 1.0 - discount );
   }

 private:
   Item item_;
};


//---- <Taxed.h> ----------------------------------------------------------------------------------

//#include <Money.h>
//#include <PricedItem.h>
#include <utility>

template< double taxRate, PricedItem Item >
class Taxed : private Item  // Using inheritance
{
 public:
   template< typename... Args >
   explicit Taxed( Args&&... args )
      : Item{ std::forward<Args>(args)... }
   {}

   Money price() const {
      return Item::price() * ( 1.0 + taxRate );
   }
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <ConferenceTicket.h>
//#include <Discounted.h>
//#include <Taxed.h>
#include <cstdlib>

int main()
{
   // 20% discount, 15% tax: (499*0.8)*1.15 = 459.08
   Taxed<0.15,Discounted<0.2,ConferenceTicket>> item{ "Core C++", Money{499} };

   Money const totalPrice = item.price();  // Results in 459.08

   // ...

   return EXIT_SUCCESS;
}

