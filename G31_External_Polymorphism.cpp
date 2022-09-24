/**************************************************************************************************
*
* \file G31_External_Polymorphism.cpp
* \brief Guideline 31: Use External Polymorphism for Nonintrusive Runtime Polymorphism
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
   {
      /* Checking that the given radius is valid */
   }

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
   {
      /* Checking that the given side length is valid */
   }

   double side() const { return side_; }
   /* Several more getters and square-specific utility functions */

 private:
   double side_;
   /* Several more data members */
};


//---- <Shape.h> ----------------------------------------------------------------------------------

#include <functional>
#include <stdexcept>
#include <utility>

class ShapeConcept
{
 public:
   virtual ~ShapeConcept() = default;

   virtual void draw() const = 0;

   // ... Potentially more polymorphic operations
};


template< typename ShapeT
        , typename DrawStrategy >
class ShapeModel : public ShapeConcept
{
 public:
   explicit ShapeModel( ShapeT shape, DrawStrategy drawer )
      : shape_{ std::move(shape) }
      , drawer_{ std::move(drawer) }
   {}

   void draw() const override { drawer_(shape_); }

 private:
   ShapeT shape_;
   DrawStrategy drawer_;
};


//---- <OpenGLDrawStrategy.h> ---------------------------------------------------------------------

//#include <Circle.h>
//#include <Square.h>
//#include /* OpenGL graphics library headers */

class OpenGLDrawStrategy
{
 public:
   explicit OpenGLDrawStrategy( /* Drawing related arguments */ )
   {}

   void operator()( Circle const& circle ) const
   {
      // ... Implementing the logic for drawing a circle by means of OpenGL
   }
   void operator()( Square const& square ) const
   {
      // ... Implementing the logic for drawing a square by means of OpenGL
   }

 private:
   /* Drawing related data members, e.g., colors, textures, ... */
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Square.h>
//#include <Shape.h>
//#include <OpenGLDrawStrategy.h>
#include <memory>
#include <vector>

int main()
{
   using Shapes = std::vector<std::unique_ptr<ShapeConcept>>;

   using CircleModel = ShapeModel<Circle,OpenGLDrawStrategy>;
   using SquareModel = ShapeModel<Square,OpenGLDrawStrategy>;

   Shapes shapes{};

   // Creating some shapes, each one
   //   equipped with an OpenGL drawing strategy
   shapes.emplace_back(
      std::make_unique<CircleModel>(
         Circle{2.3}, OpenGLDrawStrategy(/*...red...*/) ) );
   shapes.emplace_back(
      std::make_unique<SquareModel>(
         Square{1.2}, OpenGLDrawStrategy(/*...green...*/) ) );
   shapes.emplace_back(
      std::make_unique<CircleModel>(
         Circle{4.1}, OpenGLDrawStrategy(/*...blue...*/) ) );

   // Drawing all shapes
   for( auto const& shape : shapes )
   {
      shape->draw();
   }

   return EXIT_SUCCESS;
}

