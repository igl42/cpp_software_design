/**************************************************************************************************
*
* \file G19_Extensive_Hierarchy.cpp
* \brief Guideline 19: Use Strategy to Isolate How Things are Done
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Point.h> ----------------------------------------------------------------------------------

struct Point
{
   double x;
   double y;
};


//---- <Shape.h> ----------------------------------------------------------------------------------

class Shape
{
 public:
   virtual ~Shape() = default;

   virtual void draw( /*some arguments*/ ) const = 0;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Point.h>
//#include <Shape.h>

class Circle : public Shape
{
 public:
   explicit Circle( double radius )
      : radius_( radius )
   {
      /* Checking that the given radius is valid */
   }

   double radius() const { return radius_; }
   Point  center() const { return center_; }

   // ... No implementation of the draw() member function anymore

 private:
   double radius_;
   Point center_{};
};


//---- <OpenGLCircle.h> ---------------------------------------------------------------------------

//#include <Circle.h>

class OpenGLCircle : public Circle
{
 public:
   explicit OpenGLCircle( double radius )
      : Circle( radius )
   {}

   void draw( /*some arguments*/ ) const override;
};


//---- <OpenGLCircle.cpp> ----------------

//#include <OpenGLCircle.h>
//#include /* OpenGL graphics library headers */

void OpenGLCircle::draw( /*some arguments*/ ) const
{
   // ... Implementing the logic for drawing a circle by means of OpenGL
}


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Point.h>
//#include <Shape.h>

class Square : public Shape
{
 public:
   explicit Square( double side )
      : side_( side )
   {
      /* Checking that the given side length is valid */
   }

   double side  () const { return side_; }
   Point  center() const { return center_; }

   // ... No implementation of the draw() member function anymore

 private:
   double side_;
   Point center_{};
};


//---- <OpenGLSquare.h> ----------------

//#include <Square.h>

class OpenGLSquare : public Square
{
 public:
   explicit OpenGLSquare( double side )
      : Square( side )
   {}

   void draw( /*some arguments*/ ) const override;
};


//---- <OpenGLSquare.cpp> ----------------

//#include <OpenGLSquare.h>
//#include /* OpenGL graphics library headers */

void OpenGLSquare::draw( /*some arguments*/ ) const
{
   // ... Implementing the logic for drawing a square by means of OpenGL
}


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


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <OpenGLCircle.h>
//#include <OpenGLSquare.h>
//#include <DrawAllShapes.h>
#include <cstdlib>
#include <memory>
#include <vector>

int main()
{
   using Shapes = std::vector< std::unique_ptr<Shape> >;

   Shapes shapes{};

   // Creating some shapes
   shapes.emplace_back( std::make_unique<OpenGLCircle>( 2.3 ) );
   shapes.emplace_back( std::make_unique<OpenGLSquare>( 1.2 ) );
   shapes.emplace_back( std::make_unique<OpenGLCircle>( 4.1 ) );

   // Drawing all shapes
   drawAllShapes( shapes );

   return EXIT_SUCCESS;
}

