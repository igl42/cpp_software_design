/**************************************************************************************************
*
* \file G16_Cylic_Visitor.cpp
* \brief Guideline 16: Use Visitors to Extend Operations
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <ShapeVisitor.h> ---------------------------------------------------------------------------

class Circle;
class Square;

class ShapeVisitor
{
 public:
   virtual ~ShapeVisitor() = default;

   virtual void visit( Circle const& /*, ...*/ ) const = 0;
   virtual void visit( Square const& /*, ...*/ ) const = 0;
   // Possibly more visit() functions, one for each concrete shape
};


//---- <Shape.h> ----------------------------------------------------------------------------------

//#include <ShapeVisitor.h>

class Shape
{
 public:
   virtual ~Shape() = default;
   virtual void accept( ShapeVisitor const& v ) = 0;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Shape.h>

class Circle : public Shape
{
 public:
   explicit Circle( double radius )
      : radius_( radius )
   {
      /* Checking that the given radius is valid */
   }

   void accept( ShapeVisitor const& v ) override { v.visit(*this); }

   double radius() const { return radius_; }

 private:
   double radius_;
};


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Shape.h>

class Square : public Shape
{
 public:
   explicit Square( double side )
      : side_( side )
   {
      /* Checking that the given side length is valid */
   }

   void accept( ShapeVisitor const& v ) override { v.visit(*this); }

   double side() const { return side_; }

 private:
   double side_;
};


//---- <Draw.h> -----------------------------------------------------------------------------------

//#include <Circle.h>
//#include <ShapeVisitor.h>
//#include <Square.h>

class Draw : public ShapeVisitor
{
 public:
   void visit( Circle const& c /*, ...*/ ) const override
   {
      // ... Implementing the logic for drawing a circle
   }

   void visit( Square const& s /*, ...*/ ) const override
   {
      // ... Implementing the logic for drawing a square
   }

   // Possibly more visit() functions, one for each concrete shape
};


//---- <DrawAllShapes.h> --------------------------------------------------------------------------

#include <memory>
#include <vector>
class Shape;

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes );


//---- <DrawAllShapes.cpp> ------------------------------------------------------------------------

//#include <DrawAllShapes.h>
//#include <Draw.h>
//#include <Shape.h>

void drawAllShapes( std::vector<std::unique_ptr<Shape>> const& shapes )
{
   for( auto const& shape : shapes )
   {
      shape->accept( Draw{} );
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
   using Shapes = std::vector< std::unique_ptr<Shape> >;

   Shapes shapes{};

   // Creating some shapes
   shapes.emplace_back( std::make_unique<Circle>( 2.3 ) );
   shapes.emplace_back( std::make_unique<Square>( 1.2 ) );
   shapes.emplace_back( std::make_unique<Circle>( 4.1 ) );

   // Drawing all shapes
   drawAllShapes( shapes );

   return EXIT_SUCCESS;
}

