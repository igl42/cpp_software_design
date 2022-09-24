/**************************************************************************************************
*
* \file G30_Prototype.cpp
* \brief Guideline 30: Apply Prototype for Abstract Copy Operations
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <Animal.h> ---------------------------------------------------------------------------------

#include <memory>

class Animal
{
 public:
   virtual ~Animal() = default;
   virtual void makeSound() const = 0;
   virtual std::unique_ptr<Animal> clone() const = 0; // Prototype design pattern
};


//---- <Sheep.h> ----------------------------------------------------------------------------------

//#include <Animal.h>
#include <string>

class Sheep : public Animal
{
 public:
   explicit Sheep( std::string name ) : name_{ std::move(name) } {}

   void makeSound() const override;
   std::unique_ptr<Animal> clone() const override;  // Prototype design pattern

 private:
   std::string name_;
};


//---- <Sheep.cpp> --------------------------------------------------------------------------------

//#include <Sheep.h>
#include <iostream>

void Sheep::makeSound() const
{
   std::cout << "baa\n";
}

std::unique_ptr<Animal> Sheep::clone() const
{
   return std::make_unique<Sheep>(*this);  // Copy-construct a sheep
}


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Sheep.h>
#include <cstdlib>
#include <memory>

int main()
{
   std::unique_ptr<Animal> dolly = std::make_unique<Sheep>( "Dolly" );
   std::unique_ptr<Animal> dollyClone = dolly->clone();

   dolly->makeSound();       // Triggers the first Dolly's beastly sound
   dollyClone->makeSound();  // The clone sounds just like Dolly

   return EXIT_SUCCESS;
}
