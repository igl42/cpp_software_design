/**************************************************************************************************
*
* \file G17_Variant.cpp
* \brief Guideline 17: Consider std::variant for Implementing Visitors
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <variant>


struct Print
{
   void operator()( int value ) const
      { std::cout << "int: " << value << '\n'; }
   void operator()( double value ) const
      { std::cout << "double: " << value << '\n'; }
   void operator()( std::string const& value ) const
      { std::cout << "string: " << value << '\n'; }
};


int main()
{
   // Creates a default variant that contains an 'int' initialized to 0
   std::variant<int,double,std::string> v{};

   v = 42;        // Assigns the 'int' 42 to the variant
   v = 3.14;      // Assigns the 'double' 3.14 to the variant
   v = 2.71F;     // Assigns a 'float', which is promoted to 'double'
   v = "Bjarne";  // Assigns the string literal 'Bjarne' to the variant
   v = 43;        // Assigns the 'int' 43 to the variant

   int const i = std::get<int>(v);  // Direct access to the value

   int* const pi = std::get_if<int>(&v);  // Direct access to the value

   std::visit( Print{}, v );  // Applying the Print visitor

   return EXIT_SUCCESS;
}

