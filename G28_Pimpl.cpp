/**************************************************************************************************
*
* \file G28_Pimpl.cpp
* \brief Guideline 28: Build Bridges to Remove Physical Dependencies
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Person.h> ---------------------------------------------------------------------------------

#include <memory>

class Person
{
 public:
   // ...
   Person();
   ~Person();

   Person( Person const& other );
   Person& operator=( Person const& other );

   Person( Person&& other );
   Person& operator=( Person&& other );

   int year_of_birth() const;
   // ... Many more access functions

 private:
   struct Impl;
   std::unique_ptr<Impl> const pimpl_;
};


//---- <Person.cpp> -------------------------------------------------------------------------------

//#include <Person.h>
#include <string>

struct Person::Impl
{
   std::string forename;
   std::string surname;
   std::string address;
   std::string city;
   std::string country;
   std::string zip;
   int year_of_birth;
   // ... Potentially many more data members
};


Person::Person()
   : pimpl_{ std::make_unique<Impl>() }
{}

Person::~Person() = default;

Person::Person( Person const& other )
   : pimpl_{ std::make_unique<Impl>(*other.pimpl_) }
{}

Person& Person::operator=( Person const& other )
{
   *pimpl_ = *other.pimpl_;
   return *this;
}

Person::Person( Person&& other )
   : pimpl_{ std::make_unique<Impl>(std::move(*other.pimpl_)) }
{}

Person& Person::operator=( Person&& other )
{
   *pimpl_ = std::move(*other.pimpl_);
   return *this;
}

int Person::year_of_birth() const
{
   return pimpl_->year_of_birth;
}

// ... Many more Person member functions


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Person.h>

int main()
{
   Person p1{};

   return EXIT_SUCCESS;
}

