/**************************************************************************************************
*
* \file G19_Strategy.cpp
* \brief Guideline 19: Use Strategy to Isolate How Things are Done
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


//---- <DrawStrategy.h> ---------------------------------------------------------------------------

template< typename T >
class DrawStrategy
{
 public:
   virtual ~DrawStrategy() = default;
   virtual void draw( T const& ) const = 0;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
//#include <DrawStrategy.h>
#include <memory>
#include <utility>

class Circle : public Shape
{
 public:
   using DrawCircleStrategy = DrawStrategy<Circle>;

   explicit Circle( double radius, std::unique_ptr<DrawCircleStrategy> drawer )
      : radius_( radius )
      , drawer_( std::move(drawer) )
   {
      /* Checking that the given radius is valid and that
         the given 'std::unique_ptr' is not a nullptr */
   }

   void draw( /*some arguments*/ ) const override
   {
      drawer_->draw( *this /*, some arguments*/ );
   }

   double radius() const { return radius_; }

 private:
   double radius_;
   std::unique_ptr<DrawCircleStrategy> drawer_;
};


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
//#include <DrawStrategy.h>
#include <memory>
#include <utility>

class Square : public Shape
{
 public:
   using DrawSquareStrategy = DrawStrategy<Square>;

   explicit Square( double side, std::unique_ptr<DrawSquareStrategy> drawer )
      : side_( side )
      , drawer_( std::move(drawer) )
   {
      /* Checking that the given side length is valid and that
         the given 'std::unique_ptr' is not a nullptr */
   }

   void draw( /*some arguments*/ ) const override
   {
      drawer_->draw( *this /*, some arguments*/ );
   }

   double side() const { return side_; }

 private:
   double side_;
   std::unique_ptr<DrawSquareStrategy> drawer_;
};


//---- <DrawAllShapes.h> --------------------------------------------------------------------------

#include <memory>
#include <vector>
class Shape;

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes );


//---- <DrawAllShapes.cpp> ------------------------------------------------------------------------

//#include <DrawAllShapes.h>
//#include <Shape.h>

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes )
{
   for( auto const& shape : shapes )
   {
      shape->draw( /*some arguments*/ );
   }
}


//---- <OpenGLCircleStrategy.h> ----------------

//#include <Circle.h>
//#include <DrawStrategy.h>
//#include /* OpenGL graphics library */

class OpenGLCircleStrategy : public DrawStrategy<Circle>
{
 public:
   explicit OpenGLCircleStrategy( /* Drawing related arguments */ )
   {}

   void draw( Circle const& circle /*, ...*/ ) const override
   {
      // ... Implementing the logic for drawing a circle by means of OpenGL
   }

 private:
   /* Drawing related data members, e.g. colors, textures, ... */
};


//---- <OpenGLSquareStrategy.h> ----------------

//#include <Square.h>
//#include <DrawStrategy.h>
//#include /* OpenGL graphics library */

class OpenGLSquareStrategy : public DrawStrategy<Square>
{
 public:
   explicit OpenGLSquareStrategy( /* Drawing related arguments */ )
   {}

   void draw( Square const& square /*, ...*/ ) const override
   {
      // ... Implementing the logic for drawing a square by means of OpenGL
   }

 private:
   /* Drawing related data members, e.g. colors, textures, ... */
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <DrawAllShapes.h>
//#include <Square.h>
//#include <OpenGLCircleStrategy.h>
//#include <OpenGLSquareStrategy.h>
#include <memory>
#include <vector>
#include <cstdlib>

int main()
{
   using Shapes = std::vector<std::unique_ptr<Shape>>;

   Shapes shapes{};

   // Creating some shapes, each one
   //   equipped with the according OpenGL drawing strategy
   shapes.emplace_back(
      std::make_unique<Circle>(
         2.3, std::make_unique<OpenGLCircleStrategy>(/*...red...*/) ) );
   shapes.emplace_back(
      std::make_unique<Square>(
         1.2, std::make_unique<OpenGLSquareStrategy>(/*...green...*/) ) );
   shapes.emplace_back(
      std::make_unique<Circle>(
         4.1, std::make_unique<OpenGLCircleStrategy>(/*...blue...*/) ) );

   drawAllShapes(shapes);

   return EXIT_SUCCESS;
}

