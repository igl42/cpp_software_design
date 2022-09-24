/**************************************************************************************************
*
* \file G23_Function.cpp
* \brief Guideline 23: Prefer a Value-Based Implementation of Strategy and Command
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


#include <cstdlib>
#include <functional>
#include <iostream>

void foo( int i )
{
   std::cout << "foo: " << i << '\n';
}

int main()
{
   // Create a default std::function instance. Calling it results
   // in a std::bad_function_call exception
   std::function<void(int)> f{};

   f = []( int i ){  // Assigning a callable to 'f'
      std::cout << "lambda: " << i << '\n';
   };

   f(1);  // Calling 'f' with the integer '1'

   auto g = f;  // Copying 'f' into 'g'

   f = foo;  // Assigning a different callable to 'f'

   f(2);  // Calling 'f' with the integer '2'
   g(3);  // Calling 'g' with the integer '3'

   return EXIT_SUCCESS;
}

