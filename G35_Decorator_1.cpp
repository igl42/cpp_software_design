/**************************************************************************************************
*
* \file G35_Decorator_1.cpp
* \brief Guideline 35: Use Decorators to Add Customization Hierarchically
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


//---- <Item.h> -----------------------------------------------------------------------------------

//#include <Money.h>

class Item
{
 public:
   virtual ~Item() = default;
   virtual Money price() const = 0;
};


//---- <DecoratedItem.h> --------------------------------------------------------------------------

//#include <Item.h>
#include <memory>
#include <stdexcept>
#include <utility>

class DecoratedItem : public Item
{
 public:
   explicit DecoratedItem( std::unique_ptr<Item> item )
      : item_( std::move(item) )
   {
      if( !item_ ) {
         throw std::invalid_argument( "Invalid item" );
      }
   }

 protected:
   Item&       item()       { return *item_; }
   Item const& item() const { return *item_; }

 private:
   std::unique_ptr<Item> item_;
};


//---- <CppBook.h> --------------------------------------------------------------------------------

//#include <Item.h>
#include <string>
#include <utility>

class CppBook : public Item
{
 public:
   CppBook( std::string title, Money price )
      : title_{ std::move(title) }
      , price_{ price }
   {}

   std::string const& title() const { return title_; }
   Money price() const override { return price_; }

 private:
   std::string title_{};
   Money price_{};
};


//---- <ConferenceTicket.h> -----------------------------------------------------------------------

//#include <Item.h>
#include <string>
#include <utility>

class ConferenceTicket : public Item
{
 public:
   ConferenceTicket( std::string name, Money price )
      : name_{ std::move(name) }
      , price_{ price }
   {}

   std::string const& name() const { return name_; }
   Money price() const override { return price_; }

 private:
   std::string name_{};
   Money price_{};
};


//---- <Discounted.h> -----------------------------------------------------------------------------

//#include <DecoratedItem.h>

class Discounted : public DecoratedItem
{
 public:
   Discounted( double discount, std::unique_ptr<Item> item )
      : DecoratedItem( std::move(item) )
      , factor_( 1.0 - discount )
   {
      if( !std::isfinite(discount) || discount < 0.0 || discount > 1.0 ) {
         throw std::invalid_argument( "Invalid discount" );
      }
   }

   Money price() const override
   {
      return item().price() * factor_;
   }

 private:
   double factor_;
};


//---- <Taxed.h> ----------------------------------------------------------------------------------

//#include <DecoratedItem.h>

class Taxed : public DecoratedItem
{
 public:
   Taxed( double taxRate, std::unique_ptr<Item> item )
      : DecoratedItem( std::move(item) )
      , factor_( 1.0 + taxRate )
   {
      if( !std::isfinite(taxRate) || taxRate < 0.0 ) {
         throw std::invalid_argument( "Invalid tax" );
      }
   }

   Money price() const override
   {
      return item().price() * factor_;
   }

 private:
   double factor_;
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <ConferenceTicket.h>
//#include <CppBook.h>
//#include <Discounted.h>
//#include <Taxed.h>
#include <cstdlib>
#include <memory>

int main()
{
   // 7% tax: 19*1.07 = 20.33
   std::unique_ptr<Item> item1(
      std::make_unique<Taxed>( 0.07,
         std::make_unique<CppBook>( "Effective C++", Money{19} ) ) );

   // 20% discount, 19% tax: (999*0.8)*1.19 = 951.05
   std::unique_ptr<Item> item2(
      std::make_unique<Taxed>( 0.19,
         std::make_unique<Discounted>( 0.2,
            std::make_unique<ConferenceTicket>( "CppCon", Money{999} ) ) ) );

   Money const totalPrice1 = item1->price();  // Results in 20.33
   Money const totalPrice2 = item2->price();  // Results in 951.05

   // ...

   return EXIT_SUCCESS;
}

