/**************************************************************************************************
*
* \file G18_Acyclic_Visitor.cpp
* \brief Guideline 18: Beware the Performance of Acyclic Visitors
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <AbstractVisitor.h> ------------------------------------------------------------------------

class AbstractVisitor
{
 public:
   virtual ~AbstractVisitor() = default;
};


//---- <Visitor.h> --------------------------------------------------------------------------------

template< typename T >
class Visitor
{
 protected:
   virtual ~Visitor() = default;

 public:
   virtual void visit( T const& ) const = 0;
};


//---- <Shape.h> ----------------------------------------------------------------------------------

//#include <AbstractVisitor.h>

class Shape
{
 public:
   virtual ~Shape() = default;

   virtual void accept( AbstractVisitor const& v ) = 0;
};


//---- <Circle.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
//#include <Visitor.h>

class Circle : public Shape
{
 public:
   explicit Circle( double radius )
      : radius_( radius )
   {
      /* Checking that the given radius is valid */
   }

   void accept( AbstractVisitor const& v ) override {
      if( auto const* cv = dynamic_cast<Visitor<Circle> const*>(&v) ) {
         cv->visit(*this);
      }
   }

   double radius() const { return radius_; }

 private:
   double radius_;
};


//---- <Square.h> ---------------------------------------------------------------------------------

//#include <Shape.h>
//#include <Visitor.h>

class Square : public Shape
{
 public:
   explicit Square( double side )
      : side_( side )
   {
      /* Checking that the given side length is valid */
   }

   void accept( AbstractVisitor const& v ) override {
      if( auto const* sv = dynamic_cast<Visitor<Square> const*>(&v) ) {
         sv->visit(*this);
      }
   }

   double side() const { return side_; }

 private:
   double side_;
};


//---- <Draw.h> -----------------------------------------------------------------------------------

//#include <AbstractVisitor.h>
//#include <Visitor.h>
//#include <Circle.h>
//#include <Square.h>

class Draw : public AbstractVisitor
           , public Visitor<Circle>
           , public Visitor<Square>
{
 public:
   void visit( Circle const& c ) const override
      { /* ... Implementing the logic for drawing a circle ... */ }
   void visit( Square const& s ) const override
      { /* ... Implementing the logic for drawing a square ... */ }
};


//---- <DrawAllShapes.h> --------------------------------------------------------------------------

#include <memory>
#include <vector>
class Shape;

void drawAllShapes( std::vector< std::unique_ptr<Shape> > const& shapes );


//---- <DrawAllShapes.cpp> ------------------------------------------------------------------------

//#include <DrawAllShapes.h>
//#include <Draw.h>
//#include <Shape.h>

void drawAllShapes( std::vector< std::unique_ptr<Shape> > const& shapes )
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

   shapes.emplace_back( std::make_unique<Circle>( 2.3 ) );
   shapes.emplace_back( std::make_unique<Square>( 1.2 ) );
   shapes.emplace_back( std::make_unique<Circle>( 4.1 ) );

   drawAllShapes( shapes );

   return EXIT_SUCCESS;
}

