/**************************************************************************************************
*
* \file G33_Manual_Virtual_Dispatch.cpp
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

#include <cstddef>
#include <memory>

class Shape
{
 public:
   template< typename ShapeT
           , typename DrawStrategy >
   Shape( ShapeT shape, DrawStrategy drawer )
      : pimpl_(
            new OwningModel<ShapeT,DrawStrategy>( std::move(shape)
                                                , std::move(drawer) )
          , []( void* shapeBytes ){
               using Model = OwningModel<ShapeT,DrawStrategy>;
               auto* const model = static_cast<Model*>(shapeBytes);
               delete model;
            } )
      , draw_(
            []( void* shapeBytes ){
               using Model = OwningModel<ShapeT,DrawStrategy>;
               auto* const model = static_cast<Model*>(shapeBytes);
               (*model->drawer_)( model->shape_ );
            } )
      , clone_(
            []( void* shapeBytes ) -> void* {
               using Model = OwningModel<ShapeT,DrawStrategy>;
               auto* const model = static_cast<Model*>(shapeBytes);
               return new Model( *model );
            } )
   {}

   Shape( Shape const& other )
      : pimpl_( other.clone_( other.pimpl_.get() ), other.pimpl_.get_deleter() )
      , draw_ ( other.draw_ )
      , clone_( other.clone_ )
   {}

   Shape& operator=( Shape const& other )
   {
      // Copy-and-Swap Idiom
      using std::swap;
      Shape copy( other );
      swap( pimpl_, copy.pimpl_ );
      swap( draw_, copy.draw_ );
      swap( clone_, copy.clone_ );
      return *this;
   }

   ~Shape() = default;
   Shape( Shape&& ) = default;
   Shape& operator=( Shape&& ) = default;

 private:
   friend void draw( Shape const& shape )
   {
      shape.draw_( shape.pimpl_.get() );
   }

   template< typename ShapeT
           , typename DrawStrategy >
   struct OwningModel
   {
      OwningModel( ShapeT value, DrawStrategy drawer )
         : shape_( std::move(value) )
         , drawer_( std::move(drawer) )
      {}

      ShapeT shape_;
      DrawStrategy drawer_;
   };

   using DestroyOperation = void(void*);
   using DrawOperation    = void(void*);
   using CloneOperation   = void*(void*);

   std::unique_ptr<void,DestroyOperation*> pimpl_;
   DrawOperation*  draw_ { nullptr };
   CloneOperation* clone_{ nullptr };
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

