/**************************************************************************************************
*
* \file G32_Type_Erasure.cpp
* \brief Guideline 32: Consider Replacing Inheritance Hierarchies with Type Erasure
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

#include <memory>
#include <utility>

namespace detail {

class ShapeConcept  // The External Polymorphism design pattern
{
 public:
   virtual ~ShapeConcept() = default;
   virtual void draw() const = 0;
   virtual std::unique_ptr<ShapeConcept> clone() const = 0;  // The Prototype design pattern
};

template< typename ShapeT
        , typename DrawStrategy >
class OwningShapeModel : public ShapeConcept
{
 public:
   explicit OwningShapeModel( ShapeT shape, DrawStrategy drawer )
      : shape_{ std::move(shape) }
      , drawer_{ std::move(drawer) }
   {}

   void draw() const override { drawer_(shape_); }

   std::unique_ptr<ShapeConcept> clone() const override  // The Prototype design pattern
   {
      return std::make_unique<OwningShapeModel>( *this );
   }

 private:
   ShapeT shape_;
   DrawStrategy drawer_;
};

} // namespace detail


class Shape
{
 public:
   template< typename ShapeT
           , typename DrawStrategy >
   Shape( ShapeT shape, DrawStrategy drawer )
   {
      using Model = detail::OwningShapeModel<ShapeT,DrawStrategy>;
      pimpl_ = std::make_unique<Model>( std::move(shape)
                                      , std::move(drawer) );
   }

   Shape( Shape const& other )
      : pimpl_( other.pimpl_->clone() )
   {}

   Shape& operator=( Shape const& other )
   {
      // Copy-and-Swap Idiom
      Shape copy( other );
      pimpl_.swap( copy.pimpl_ );
      return *this;
   }

   ~Shape() = default;
   Shape( Shape&& ) = default;
   Shape& operator=( Shape&& ) = default;

 private:
   friend void draw( Shape const& shape )
   {
      shape.pimpl_->draw();
   }

   std::unique_ptr<detail::ShapeConcept> pimpl_;  // The Bridge design pattern
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

