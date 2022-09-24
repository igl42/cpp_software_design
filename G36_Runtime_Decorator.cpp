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


//---- <Item.h> ----------------

//#include <Money.h>
#include <memory>
#include <utility>

class Item
{
 public:
   template< typename T >
   Item( T item )
      : pimpl_( std::make_unique<Model<T>>( std::move(item) ) )
   {}

   Item( Item const& item ) : pimpl_( item.pimpl_->clone() ) {}

   Item& operator=( Item const& item )
   {
      pimpl_ = item.pimpl_->clone();
      return *this;
   }

   ~Item() = default;
   Item( Item&& ) = default;
   Item& operator=( Item&& item ) = default;

   Money price() const { return pimpl_->price(); }

 private:
   struct Concept
   {
      virtual ~Concept() = default;
      virtual Money price() const = 0;
      virtual std::unique_ptr<Concept> clone() const = 0;
   };

   template< typename T >
   struct Model : public Concept
   {
      explicit Model( T const& item ) : item_( item ) {}
      explicit Model( T&& item ) : item_( std::move(item) ) {}

      Money price() const override
      {
         return item_.price();
      }

      std::unique_ptr<Concept> clone() const override
      {
         return std::make_unique<Model<T>>(*this);
      }

      T item_;
   };

   std::unique_ptr<Concept> pimpl_;
};


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


//---- <Discounted.h> -----------------------------------------------------------------------------

//#include <Item.h>
#include <utility>

class Discounted
{
 public:
   Discounted( double discount, Item item )
      : item_( std::move(item) )
      , factor_( 1.0 - discount )
   {}

   Money price() const
   {
      return item_.price() * factor_;
   }

 private:
   Item item_;
   double factor_;
};


//---- <Taxed.h> ----------------------------------------------------------------------------------

//#include <Item.h>
#include <utility>

class Taxed
{
 public:
   Taxed( double taxRate, Item item )
      : item_( std::move(item) )
      , factor_( 1.0 + taxRate )
   {}

   Money price() const
   {
      return item_.price() * factor_;
   }

 private:
   Item item_;
   double factor_;
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <ConferenceTicket.h>
//#include <Discounted.h>
//#include <Taxed.h>
#include <cstdlib>

int main()
{
   // 20% discount, 15% tax: (499*0.8)*1.15 = 459.08
   Item item( Taxed( 0.15, Discounted(0.2, ConferenceTicket{ "Core C++", Money{499} } ) ) );

   Money const totalPrice = item.price();

   // ...

   return EXIT_SUCCESS;
}

