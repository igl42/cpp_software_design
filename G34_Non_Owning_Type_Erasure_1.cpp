/**************************************************************************************************
*
* \file G34_Non_Owning_Type_Erasure_1.cpp
* \brief Guideline 34: Be Aware of the Setup Costs of Owning Type Erasure Wrappers
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

class ShapeConstRef
{
 public:
   template< typename ShapeT, typename DrawStrategy >
   ShapeConstRef( ShapeT& shape, DrawStrategy& drawer )
      : shape_{ std::addressof(shape) }
      , drawer_{ std::addressof(drawer) }
      , draw_{ []( void const* shapeBytes, void const* drawerBytes ){
           auto const* shape = static_cast<ShapeT const*>(shapeBytes);
           auto const* drawer = static_cast<DrawStrategy const*>(drawerBytes);
           (*drawer)( *shape );
        } }
   {}

 private:
   friend void draw( ShapeConstRef const& shape )
   {
      shape.draw_( shape.shape_, shape.drawer_ );
   }

   using DrawOperation = void( void const*,void const* );

   void const* shape_{ nullptr };
   void const* drawer_{ nullptr };
   DrawOperation* draw_{ nullptr };
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Shape.h>
#include <cstdlib>

void useShapeConstRef( ShapeConstRef shape )
{
   draw( shape );
}

int main()
{
   // Create a circle as one representative of a concrete shape type
   Circle circle{ 3.14 };

   // Create a drawing strategy in form of a lambda
   auto drawer = []( Circle const& c ){ /*...*/ };

   // Draw the circle directly via the 'ShapeConstRef' abstraction
   useShapeConstRef( { circle, drawer } );

   return EXIT_SUCCESS;
}

