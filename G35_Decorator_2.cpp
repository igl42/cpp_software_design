/**************************************************************************************************
*
* \file G35_Decorator_2.cpp
* \brief Guideline 35: Use Decorators to Add Customization Hierarchically
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <CustomAllocator.h> ----------------

#include <cstdlib>
#include <memory_resource>

class CustomAllocator : public std::pmr::memory_resource
{
 public:
   CustomAllocator( std::pmr::memory_resource* upstream )
      : upstream_{ upstream }
   {}

 private:
   void* do_allocate( size_t bytes, size_t alignment ) override
   {
      return malloc( bytes );
   }

   void do_deallocate( void* ptr, [[maybe_unused]] size_t bytes,
                       [[maybe_unused]] size_t alignment ) override
   {
      free( ptr );
   }

   bool do_is_equal(
      std::pmr::memory_resource const& other ) const noexcept override
   {
      return ( this == &other ) ||
             ( dynamic_cast<const CustomAllocator*>( &other ) != nullptr );
   }

   std::pmr::memory_resource* upstream_{};
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

#include <array>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

int main()
{
   /*
   std::array<std::byte,1000> raw;  // Note: not initialized!
   std::pmr::monotonic_buffer_resource buffer{ raw.data(), raw.size(), std::pmr::null_memory_resource() };
   */

   CustomAllocator custom_allocator{ std::pmr::new_delete_resource() };
   std::pmr::monotonic_buffer_resource buffer{ &custom_allocator };

   std::pmr::vector<std::pmr::string> strings{ &buffer };

   strings.emplace_back( "String longer than what SSO can handle" );
   strings.emplace_back( "Another long string that goes beyond SSO" );
   strings.emplace_back( "A third long string that cannot be handled by SSO" );

   for( const auto& s : strings ) {
      std::cout << std::quoted(s) << '\n';
   }

   return EXIT_SUCCESS;
}

