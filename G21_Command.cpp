/**************************************************************************************************
*
* \file G21_Command.cpp
* \brief Guideline 21: Use Commands to Isolate What Things are Done
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <CalculatorCommand.h> ----------------------------------------------------------------------

class CalculatorCommand
{
 public:
   virtual ~CalculatorCommand() = default;

   virtual int execute( int i ) const = 0;
   virtual int undo( int i ) const = 0;
};


//---- <Add.h> ------------------------------------------------------------------------------------

//#include <CalculatorCommand.h>

class Add : public CalculatorCommand
{
 public:
   explicit Add( int operand ) : operand_(operand) {}

   int execute( int i ) const override
   {
      return i + operand_;
   }
   int undo( int i ) const override
   {
      return i - operand_;
   }

 private:
   int operand_{};
};


//---- <Subtract.h> -------------------------------------------------------------------------------

//#include <CalculatorCommand.h>

class Subtract : public CalculatorCommand
{
 public:
   explicit Subtract( int operand ) : operand_(operand) {}

   int execute( int i ) const override
   {
      return i - operand_;
   }
   int undo( int i ) const override
   {
      return i + operand_;
   }

 private:
   int operand_{};
};


//---- <Calculator.h> -----------------------------------------------------------------------------

//#include <CalculatorCommand.h>
#include <memory>
#include <stack>

class Calculator
{
 public:
   void compute( std::unique_ptr<CalculatorCommand> command );
   void undoLast();

   int result() const;
   void clear();

 private:
   using CommandStack = std::stack<std::unique_ptr<CalculatorCommand>>;

   int current_{};
   CommandStack stack_;
};


//---- <Calculator.cpp> ----------------

//#include <Calculator.h>
#include <utility>

void Calculator::compute( std::unique_ptr<CalculatorCommand> command )
{
   current_ = command->execute( current_ );
   stack_.push( std::move(command) );
}

void Calculator::undoLast()
{
   if( stack_.empty() ) return;

   auto command = std::move(stack_.top());
   stack_.pop();

   current_ = command->undo(current_);
}

int Calculator::result() const
{
   return current_;
}

void Calculator::clear()
{
   current_ = 0;
   CommandStack{}.swap( stack_ );  // Clearing the stack
}


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <Calculator.h>
//#include <Add.h>
//#include <Subtract.h>
#include <cstdlib>
#include <memory>
#include <utility>

int main()
{
   Calculator calculator{};

   auto op1 = std::make_unique<Add>( 3 );
   auto op2 = std::make_unique<Add>( 7 );
   auto op3 = std::make_unique<Subtract>( 4 );
   auto op4 = std::make_unique<Subtract>( 2 );

   calculator.compute( std::move(op1) );  // Computes 0 + 3, stores and returns 3
   calculator.compute( std::move(op2) );  // Computes 3 + 7, stores and returns 10
   calculator.compute( std::move(op3) );  // Computes 10 - 4, stores and returns 6
   calculator.compute( std::move(op4) );  // Computes 6 - 2, stores and returns 4

   calculator.undoLast();  // Reverts the last operation,
                           // stores and returns 6

   int const res = calculator.result();  // Get the final result: 6

   // ...

   return EXIT_SUCCESS;
}

