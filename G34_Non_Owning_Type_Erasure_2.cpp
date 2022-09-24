/**************************************************************************************************
*
* \file G34_Non_Owning_Type_Erasure_2.cpp
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

#include <array>
#include <cstddef>
#include <memory>
#include <utility>

namespace detail {

class ShapeConcept  // The External Polymorphism design pattern
{
 public:
   virtual ~ShapeConcept() = default;
   virtual void draw() const = 0;
   virtual std::unique_ptr<ShapeConcept> clone() const = 0;  // The Prototype design pattern
   virtual void clone( ShapeConcept* memory ) const = 0;  // The Prototype design pattern
};

template< typename ShapeT, typename DrawStrategy > class NonOwningShapeModel;

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

   void clone( ShapeConcept* memory ) const
   {
      using Model = NonOwningShapeModel<ShapeT const,DrawStrategy const>;

      std::construct_at( static_cast<Model*>(memory), shape_, drawer_ );

      // or:
      // auto* ptr =
      //    const_cast<void*>(static_cast<void const volatile*>(memory));
      // ::new (ptr) Model( shape_, drawer_ );
   }

 private:
   ShapeT shape_;
   DrawStrategy drawer_;
};

template< typename ShapeT
        , typename DrawStrategy >
class NonOwningShapeModel : public ShapeConcept
{
 public:
   NonOwningShapeModel( ShapeT& shape, DrawStrategy& drawer )
      : shape_{ std::addressof(shape) }
      , drawer_{ std::addressof(drawer) }
   {}

   void draw() const override { (*drawer_)(*shape_); }

   std::unique_ptr<ShapeConcept> clone() const override
   {
      using Model = OwningShapeModel<ShapeT,DrawStrategy>;
      return std::make_unique<Model>( *shape_, *drawer_ );
   }

   void clone( ShapeConcept* memory ) const override
   {
      std::construct_at( static_cast<NonOwningShapeModel*>(memory), *this );

      // or:
      // auto* ptr = const_cast<void*>(static_cast<void const volatile*>(memory));
      // ::new (ptr) NonOwningShapeModel( *this );
   }

 private:
   ShapeT* shape_{ nullptr };
   DrawStrategy* drawer_{ nullptr };
};

} // namespace detail


class Shape;


class ShapeConstRef
{
 public:
   // Type 'ShapeT' and 'DrawStrategy' are possibly cv qualified;
   // lvalue references prevent references to rvalues
   template< typename ShapeT
           , typename DrawStrategy >
   ShapeConstRef( ShapeT& shape
                , DrawStrategy& drawer )
   {
      using Model =
         detail::NonOwningShapeModel<ShapeT const,DrawStrategy const>;
      static_assert( sizeof(Model) == MODEL_SIZE, "Invalid size detected" );
      static_assert( alignof(Model) == alignof(void*), "Misaligned detected" );

      std::construct_at( static_cast<Model*>(pimpl()), shape, drawer );

      // or:
      // auto* ptr =
      //    const_cast<void*>(static_cast<void const volatile*>(pimpl()));
      // ::new (ptr) Model( shape, drawer );
   }

   ShapeConstRef( Shape& other );
   ShapeConstRef( Shape const& other );

   ShapeConstRef( ShapeConstRef const& other )
   {
      other.pimpl()->clone( pimpl() );
   }

   ShapeConstRef& operator=( ShapeConstRef const& other )
   {
      // Copy-and-swap idiom
      ShapeConstRef copy( other );
      raw_.swap( copy.raw_ );
      return *this;
   }

   ~ShapeConstRef()
   {
      std::destroy_at( pimpl() );
      // or: pimpl()->~ShapeConcept();
   }

   // Move operations explicitly not declared

 private:
   friend void draw( ShapeConstRef const& shape )
   {
      shape.pimpl()->draw();
   }

   detail::ShapeConcept* pimpl()  // The Bridge design pattern
   {
      return reinterpret_cast<detail::ShapeConcept*>( raw_.data() );
   }

   detail::ShapeConcept const* pimpl() const
   {
      return reinterpret_cast<detail::ShapeConcept const*>( raw_.data() );
   }

   // Expected size of a model instantiation:
   //     sizeof(ShapeT*) + sizeof(DrawStrategy*) + sizeof(vptr)
   static constexpr size_t MODEL_SIZE = 3U*sizeof(void*);

   alignas(void*) std::array<std::byte,MODEL_SIZE> raw_;

   friend class Shape;
};


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

   Shape( ShapeConstRef const& other )
      : pimpl_{ other.pimpl()->clone() }
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

   friend class ShapeConstRef;
};


ShapeConstRef::ShapeConstRef( Shape& other )
{
   other.pimpl_->clone( pimpl() );
}

ShapeConstRef::ShapeConstRef( Shape const& other )
{
   other.pimpl_->clone( pimpl() );
}


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Circle.h>
//#include <Shape.h>
#include <cstdlib>

int main()
{
   // Create a circle as one representative of a concrete shape type
   Circle circle{ 3.14 };

   // Create a drawing strategy in form of a lambda
   auto drawer = []( Circle const& c ){ /*...*/ };

   // Combine the shape and the drawing strategy in a 'Shape' abstraction
   Shape shape1( circle, drawer );

   // Draw the shape
   draw( shape1 );

   // Create a reference to the shape
   // Works already, but the shape reference will store a pointer
   // to the 'shape1' instance instead of a pointer to the 'circle'.
   ShapeConstRef shaperef( shape1 );

   // Draw via the shape reference, resulting in the same output
   // This works, but only by means of two indirections!
   draw( shaperef );

   // Create a deep copy of the shape via the shape reference
   // This is _not_ possible with the simple non-owning implementation!
   // With the simple implementation, this creates a copy of the 'shaperef'
   // instance. 'shape2' itself would act as a reference and there would be
   // three indirections... sigh.
   Shape shape2( shaperef );

   // Drawing the copy will again result in the same output
   draw( shape2 );

   return EXIT_SUCCESS;
}

