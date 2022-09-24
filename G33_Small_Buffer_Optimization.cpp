/**************************************************************************************************
*
* \file G33_Small_Buffer_Optimization.cpp
* \brief Guideline 33: Be Aware of the Optimization Potential of Type Erasure
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Circle.h> ---------------------------------------------------------------------------------

class Circle
{
 public:
   explicit Circle( double radius )
      : radius_( radius )
   {}

   double radius() const { return radius_; }
   /* Several more getters and circle-specific utility functions */

 private:
   double radius_;
   /* Several more data members */
};


//---- <Square.h> ---------------------------------------------------------------------------------

class Square
{
 public:
   explicit Square( double side )
      : side_( side )
   {}

   double side() const { return side_; }
   /* Several more getters and square-specific utility functions */

 private:
   double side_;
   /* Several more data members */
};


//---- <Shape.h> ----------------------------------------------------------------------------------

#include <array>
#include <cstdlib>
#include <memory>
#include <utility>


template< size_t Capacity = 32U, size_t Alignment = alignof(void*) >
class Shape
{
 public:
   template< typename ShapeT, typename DrawStrategy >
   Shape( ShapeT shape, DrawStrategy drawer )
   {
      using Model = OwningModel<ShapeT,DrawStrategy>;

      static_assert( sizeof(Model) <= Capacity, "Given type is too large" );
      static_assert( alignof(Model) <= Alignment, "Given type is misaligned" );

      std::construct_at( static_cast<Model*>(pimpl())
                       , std::move(shape), std::move(drawer) );
      // or:
      // auto* ptr =
      //    const_cast<void*>(static_cast<void const volatile*>(pimpl()));
      // ::new (ptr) Model( std::move(shape), std::move(drawer) );
   }

   Shape( Shape const& other )
   {
      other.pimpl()->clone( pimpl() );
   }

   Shape& operator=( Shape const& other )
   {
      // Copy-and-Swap Idiom
      Shape copy( other );
      buffer_.swap( copy.buffer_ );
      return *this;
   }

   Shape( Shape&& other ) noexcept
   {
      other.pimpl()->move( pimpl() );
   }

   Shape& operator=( Shape&& other ) noexcept
   {
      // Copy-and-Swap Idiom
      Shape copy( std::move(other) );
      buffer_.swap( copy.buffer_ );
      return *this;
   }

   ~Shape()
   {
      std::destroy_at( pimpl() );
      // or: pimpl()->~Concept();
   }

 private:
   friend void draw( Shape const& shape )
   {
      shape.pimpl()->draw();
   }

   struct Concept  // The External Polymorphism design pattern
   {
      virtual ~Concept() = default;
      virtual void draw() const = 0;
      virtual void clone( Concept* memory ) const = 0;  // The Prototype design pattern
      virtual void move( Concept* memory ) = 0;
   };

   template< typename ShapeT, typename DrawStrategy >
   struct OwningModel : public Concept
   {
      OwningModel( ShapeT shape, DrawStrategy drawer )
         : shape_( std::move(shape) )
         , drawer_( std::move(drawer) )
      {}

      void draw() const override
      {
         drawer_( shape_ );
      }

      void clone( Concept* memory ) const override
      {
         std::construct_at( static_cast<OwningModel*>(memory), *this );

         // or:
         // auto* ptr = const_cast<void*>(static_cast<void const volatile*>(memory));
         // ::new (ptr) OwningModel( *this );
      }

      void move( Concept* memory ) override
      {
         std::construct_at( static_cast<OwningModel*>(memory), std::move(*this) );

         // or:
         // auto* ptr = const_cast<void*>(static_cast<void const volatile*>(memory));
         // ::new (ptr) OwningModel( std::move(*this) );
      }

      ShapeT shape_;
      DrawStrategy drawer_;
   };

   Concept* pimpl()  // The Bridge design pattern
   {
      return reinterpret_cast<Concept*>( buffer_.data() );
   }

   Concept const* pimpl() const
   {
      return reinterpret_cast<Concept const*>( buffer_.data() );
   }

   alignas(Alignment) std::array<std::byte,Capacity> buffer_;
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Square.h>
//#include <Shape.h>
#include <cstdlib>

int main()
{
   // Create a circle as one representative of a concrete shape type
   Circle circle{ 3.14 };

   // Create a drawing strategy in form of a lambda
   auto drawer = []( Circle const& c ){ /*...*/ };

   // Combine the shape and the drawing strategy in a 'Shape' abstraction
   // This constructor call will instantiate a 'detail::OwningShapeModel' for
   // the given 'Circle' and lambda types
   Shape shape1( circle, drawer );

   // Draw the shape
   draw( shape1 );

   // Create a copy of the shape by means of the copy constructor
   Shape shape2( shape1 );

   // Drawing the copy will result in the same output
   draw( shape2 );

   return EXIT_SUCCESS;
}

