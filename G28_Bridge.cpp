/**************************************************************************************************
*
* \file G28_Bridge.cpp
* \brief Guideline 28: Build Bridges to Remove Physical Dependencies
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Engine.h> ---------------------------------------------------------------------------------

class Engine
{
 public:
   virtual ~Engine() = default;
   virtual void start() = 0;
   virtual void stop() = 0;
   // ... more engine-specific functions

 private:
   // ...
};


//---- <Car.h> ------------------------------------------------------------------------------------

//#include <Engine.h>
#include <memory>
#include <utility>

class Car
{
 protected:
   explicit Car( std::unique_ptr<Engine> engine )
      : pimpl_{ std::move(engine) }
   {}

 public:
   virtual ~Car() = default;
   virtual void drive() = 0;
   // ... more car-specific functions

 protected:
   Engine*       getEngine()       { return pimpl_.get(); }
   Engine const* getEngine() const { return pimpl_.get(); }

 private:
   std::unique_ptr<Engine> pimpl_;  // Pointer-to-implementation (pimpl)

   // ... more car-specific data members (wheels, drivetrain, ...)
};


//---- <ElectricEngine.h> -------------------------------------------------------------------------

//#include <Engine.h>

class ElectricEngine : public Engine
{
 public:
   void start() override;
   void stop() override;

 private:
   // ...
};


//---- <ElectricEngine.cpp> -----------------------------------------------------------------------

//#include <ElectricEngine.h>
#include <iostream>

void ElectricEngine::start()
{
   std::cout << "Starting the 'ElectricEngine'...\n";
}

void ElectricEngine::stop()
{
   std::cout << "Stopping the 'ElectricEngine'...\n";
}


//---- <ElectricCar.h> ----------------------------------------------------------------------------

//#include <Car.h>

class ElectricCar : public Car
{
 public:
   ElectricCar();

   void drive();
   // ...

 private:
   // ...
};


//---- <ElectricCar.cpp> --------------------------------------------------------------------------

//#include <ElectricCar.h>
//#include <ElectricEngine.h>
#include <iostream>

ElectricCar::ElectricCar()
   : Car{ std::make_unique<ElectricEngine>( /*engine arguments*/ ) }
   // ... Initialization of the other data members
{}

void ElectricCar::drive()
{
   getEngine()->start();
   std::cout << "Driving the 'ElectricCar'...\n";
   getEngine()->stop();
}

// ... Other 'ElectricCar' member functions, primarily using the 'Engine'
//     abstraction, but potentially also explicitly dealing with an
//     'ElectricEngine'.


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <ElectricCar.h>
#include <cstdlib>

int main()
{
   ElectricCar ecar{};
   ecar.drive();

   return EXIT_SUCCESS;
}

