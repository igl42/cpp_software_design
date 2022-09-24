/**************************************************************************************************
*
* \file G25_Classic_Observer.cpp
* \brief Guideline 25: Apply Observers as an Abstract Notification Mechanism
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Observer.h> -------------------------------------------------------------------------------

template< typename Subject, typename StateTag >
class Observer
{
 public:
   virtual ~Observer() = default;

   virtual void update( Subject const& subject, StateTag property ) = 0;
};


//---- <Person.h> ---------------------------------------------------------------------------------

//#include <Observer.h>
#include <string>
#include <set>

class Person
{
 public:
   enum StateChange
   {
      forenameChanged,
      surnameChanged,
      addressChanged
   };

   using PersonObserver = Observer<Person,StateChange>;

   explicit Person( std::string forename, std::string surname )
      : forename_{ std::move(forename) }
      , surname_{ std::move(surname) }
   {}

   bool attach( PersonObserver* observer );
   bool detach( PersonObserver* observer );

   void notify( StateChange property );

   void forename( std::string newForename );
   void surname ( std::string newSurname );
   void address ( std::string newAddress );

   std::string const& forename() const { return forename_; }
   std::string const& surname () const { return surname_; }
   std::string const& address () const { return address_; }

 private:
   std::string forename_;
   std::string surname_;
   std::string address_;

   std::set<PersonObserver*> observers_;
};


//---- <Person.cpp> -------------------------------------------------------------------------------

//#include <Person.h>

void Person::forename( std::string newForename )
{
   forename_ = std::move(newForename);
   notify( forenameChanged );
}

void Person::surname( std::string newSurname )
{
   surname_ = std::move(newSurname);
   notify( surnameChanged );
}

void Person::address( std::string newAddress )
{
   address_ = std::move(newAddress);
   notify( addressChanged );
}

bool Person::attach( PersonObserver* observer )
{
   auto [pos,success] = observers_.insert( observer );
   return success;
}

bool Person::detach( PersonObserver* observer )
{
   return ( observers_.erase( observer ) > 0U );
}

void Person::notify( StateChange property )
{
   // This formulation makes sure detach() operations
   // can be detected during the iteration
   for( auto iter=begin(observers_); iter!=end(observers_); )
   {
      auto const pos = iter++;
      (*pos)->update(*this,property);
   }
}


//---- <NameObserver.h> ---------------------------------------------------------------------------

//#include <Observer.h>
//#include <Person.h>

class NameObserver : public Observer<Person,Person::StateChange>
{
 public:
   void update( Person const& person, Person::StateChange property ) override;
};


//---- <NameObserver.cpp> ----------------

//#include <NameObserver.h>

void NameObserver::update( Person const& person, Person::StateChange property )
{
   if( property == Person::forenameChanged ||
       property == Person::surnameChanged )
   {
      // ... Respond to changed name
   }
}


//---- <AddressObserver.h> ------------------------------------------------------------------------

//#include <Observer.h>
//#include <Person.h>

class AddressObserver : public Observer<Person,Person::StateChange>
{
 public:
   void update( Person const& person, Person::StateChange property ) override;
};

//---- <AddressObserver.cpp> ----------------------------------------------------------------------

//#include <AddressObserver.h>

void AddressObserver::update( Person const& person, Person::StateChange property )
{
   if( property == Person::addressChanged ) {
      // ... Respond to changed address
   }
}


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <AddressObserver.h>
//#include <NameObserver.h>
//#include <Person.h>
#include <cstdlib>

int main()
{
   NameObserver nameObserver;
   AddressObserver addressObserver;

   Person homer( "Homer"     , "Simpson" );
   Person marge( "Marge"     , "Simpson" );
   Person monty( "Montgomery", "Burns"   );

   // Attaching observers
   homer.attach( &nameObserver );
   marge.attach( &addressObserver );
   monty.attach( &addressObserver );

   // Updating information on Homer Simpson
   homer.forename( "Homer Jay" );  // Adding his middle name

   // Updating information on Marge Simpson
   marge.address( "712 Red Bark Lane, Henderson, Clark County, Nevada 89011" );

   // Updating information on Montgomery Burns
   monty.address( "Springfield Nuclear Power Plant" );

   // Detaching observers
   homer.detach( &nameObserver );

   return EXIT_SUCCESS;
}

