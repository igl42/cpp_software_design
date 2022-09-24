/**************************************************************************************************
*
* \file G15_Procedural_Solution.cpp
* \brief Guideline 15: Design for the Addition of Types or Operations
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

enum ShapeType
{
   circle,
   square
};

class Shape
{
 protected:
   explicit Shape( ShapeType type )
      : type_( type )
   {}

 public:
   virtual ~Shape() = default;

   ShapeType getType() const { return type_; }

 private:
   ShapeType type_;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Point.h>
//#include <Shape.h>

class Circle : public Shape
{
 public:
   explicit Circle( double radius )
      : Shape( circle )
      , radius_( radius )
   {
      /* Checking that the given radius is valid */
   }

   double radius() const { return radius_; }
   Point  center() const { return center_; }

 private:
   double radius_;
   Point center_{};
};


//---- <DrawCircle.h> -----------------------------------------------------------------------------

class Circle;

void draw( Circle const& circle );


//---- <DrawCircle.cpp> ---------------------------------------------------------------------------

//#include <Circle.h>
//#include <DrawCircle.h>
//#include /* some graphics library */

void draw( Circle const& circle )
{
   // ... Implementing the logic for drawing a circle
}


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
//#include <Point.h>

class Square : public Shape
{
 public:
   explicit Square( double side )
      : Shape( square )
      , side_( side )
   {
      /* Checking that the given side length is valid */
   }

   double side  () const { return side_; }
   Point  center() const { return center_; }

 private:
   double side_;
   Point center_{};
};


//---- <DrawSquare.h> -----------------------------------------------------------------------------

class Square;

void draw( Square const& square );


//---- <DrawSquare.cpp> ---------------------------------------------------------------------------

//#include <DrawSquare.h>
//#include <Square.h>
//#include /* some graphics library */

void draw( Square const& square )
{
   // ... Implementing the logic for drawing a square
}


//---- <DrawAllShapes.h> --------------------------------------------------------------------------

#include <memory>
#include <vector>
class Shape;

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes );


//---- <DrawAllShapes.cpp> ------------------------------------------------------------------------

//#include <DrawAllShapes.h>
//#include <Circle.h>
//#include <Square.h>

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes )
{
   for( auto const& shape : shapes )
   {
      switch( shape->getType() )
      {
         case circle:
            draw( static_cast<Circle const&>( *shape ) );
            break;
         case square:
            draw( static_cast<Square const&>( *shape ) );
            break;
      }
   }
}


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Square.h>
//#include <DrawAllShapes.h>
#include <cstdlib>
#include <memory>
#include <vector>

int main()
{
   using Shapes = std::vector<std::unique_ptr<Shape>>;

   // Creating some shapes
   Shapes shapes{};
   shapes.emplace_back( std::make_unique<Circle>( 2.3 ) );
   shapes.emplace_back( std::make_unique<Square>( 1.2 ) );
   shapes.emplace_back( std::make_unique<Circle>( 4.1 ) );

   // Drawing all shapes
   drawAllShapes( shapes );

   return EXIT_SUCCESS;
}

