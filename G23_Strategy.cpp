/**************************************************************************************************
*
* \file G23_Strategy.cpp
* \brief Guideline 23: Prefer a Value-Based Implementation of Strategy and Command
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Shape.h> ----------------------------------------------------------------------------------

class Shape
{
 public:
   virtual ~Shape() = default;
   virtual void draw( /*some arguments*/ ) const = 0;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
#include <functional>
#include <utility>

class Circle : public Shape
{
 public:
   using DrawStrategy = std::function<void(Circle const& /*, ...*/)>;

   explicit Circle( double radius, DrawStrategy drawer )
      : radius_( radius )
      , drawer_( std::move(drawer) )
   {
      /* Checking that the given radius is valid and that
         the given 'std::function' instance is not empty */
   }

   void draw( /*some arguments*/ ) const override
   {
      drawer_( *this /*, some arguments*/ );
   }

   double radius() const { return radius_; }

 private:
   double radius_;
   DrawStrategy drawer_;
};


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
#include <functional>
#include <utility>

class Square : public Shape
{
 public:
   using DrawStrategy = std::function<void(Square const& /*, ...*/)>;

   explicit Square( double side, DrawStrategy drawer )
      : side_( side )
      , drawer_( std::move(drawer) )
   {
      /* Checking that the given side length is valid and that
         the given 'std::function' instance is not empty */
   }

   void draw( /*some arguments*/ ) const override
   {
      drawer_( *this /*, some arguments*/ );
   }

   double side() const { return side_; }

 private:
   double side_;
   DrawStrategy drawer_;
};


//---- <OpenGLCircleStrategy.h> -------------------------------------------------------------------

//#include <Circle.h>
//#include /* OpenGL graphics library */

class OpenGLCircleStrategy
{
 public:
   explicit OpenGLCircleStrategy( /* Drawing related arguments */ )
   {}

   void operator()( Circle const& circle /*, ...*/ ) const
   {
      // ... Implementing the logic for drawing a circle by means of OpenGL
   }

 private:
   /* Drawing related data members, e.g. colors, textures, ... */
};


//---- <OpenGLSquareStrategy.h> -------------------------------------------------------------------

//#include <Square.h>
//#include /* OpenGL graphics library */

class OpenGLSquareStrategy
{
 public:
   explicit OpenGLSquareStrategy( /* Drawing related arguments */ )
   {}

   void operator()( Square const& square /*, ...*/ ) const
   {
      // ... Implementing the logic for drawing a square by means of OpenGL
   }

 private:
   /* Drawing related data members, e.g. colors, textures, ... */
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Square.h>
//#include <OpenGLCircleStrategy.h>
//#include <OpenGLSquareStrategy.h>
#include <memory>
#include <vector>

int main()
{
   using Shapes = std::vector<std::unique_ptr<Shape>>;

   Shapes shapes{};

   // Creating some shapes, each one
   //   equipped with the according OpenGL drawing strategy
   shapes.emplace_back(
      std::make_unique<Circle>( 2.3, OpenGLCircleStrategy(/*...red...*/) ) );
   shapes.emplace_back(
      std::make_unique<Square>( 1.2, OpenGLSquareStrategy(/*...green...*/) ) );
   shapes.emplace_back(
      std::make_unique<Circle>( 4.1, OpenGLCircleStrategy(/*...blue...*/) ) );

   // Drawing all shapes
   for( auto const& shape : shapes )
   {
      shape->draw();
   }

   return EXIT_SUCCESS;
}

