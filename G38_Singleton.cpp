/**************************************************************************************************
*
* \file G38_Singleton.cpp
* \brief Guideline 38: Design Singletons for Change and Testability
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
**************************************************************************************************/


//---- <PersistenceInterface.h> -------------------------------------------------------------------

class PersistenceInterface
{
 public:
   virtual ~PersistenceInterface() = default;

   bool read( /*some arguments*/ ) const
   {
      return do_read( /*...*/ );
   }
   bool write( /*some arguments*/ )
   {
      return do_write( /*...*/ );
   }

   // ... More database specific functionality

 private:
   virtual bool do_read( /*some arguments*/ ) const = 0;
   virtual bool do_write( /*some arguments*/ ) = 0;
};

PersistenceInterface* get_persistence_interface();
void set_persistence_interface( PersistenceInterface* persistence );

// Declaration of the one 'instance' variable
extern PersistenceInterface* instance;


//---- <Database.h> -------------------------------------------------------------------------------

class Database : public PersistenceInterface
{
 public:
   Database() = default;

   // ... Potentially access to data members

   // Make the class immobile by deleting the copy and move operations
   Database( Database const& ) = delete;
   Database& operator=( Database const& ) = delete;
   Database( Database&& ) = delete;
   Database& operator=( Database&& ) = delete;

 private:
   bool do_read( /*some arguments*/ ) const override
   {
      /* Reading from the database */
      return true;
   }

   bool do_write( /*some arguments*/ ) override
   {
      /* Writing to the database */
      return true;
   }

   // ... More database-specific functionality

   // ... Potentially some data members
};


//---- <PersistenceInterface.cpp> -----------------------------------------------------------------

//#include <Database.h>

// Definition of the one 'instance' variable
PersistenceInterface* instance = nullptr;

PersistenceInterface* get_persistence_interface()
{
   // Local object, initialized by an 'Immediately Invoked Lambda Expression (IILE)'
   static bool init = [](){
      if( !instance ) {
         static Database db;
         instance = &db;
      }
      return true;  // or false, as the actual value does not matter.
   }();  // Note the '()' after the lambda expression. This invokes the lambda.

   return instance;
}

void set_persistence_interface( PersistenceInterface* persistence )
{
   instance = persistence;
}


//---- <Widget.h> ---------------------------------------------------------------------------------

//#include <PersistenceInterface.h>

class Widget
{
 public:
   Widget( PersistenceInterface* persistence )  // Dependency injection
      : persistence_(persistence)
   {}

   void doSomething( /*some arguments*/ )
   {
      doSomething( get_persistence_interface() /*, some arguments*/ );
   }

   void doSomething( PersistenceInterface* persistence /*, some arguments*/ )
   {
      // ...
      persistence->read( /*some arguments*/ );
      // ...
   }

 private:
   PersistenceInterface* persistence_{};
};


//---- <CustomPersistence.h> ----------------------------------------------------------------------

//#include <PersistenceInterface.h>
#include <iostream>

class CustomPersistence : public PersistenceInterface
{
 public:
   CustomPersistence() = default;
   CustomPersistence( CustomPersistence const& db ) = default;

   bool do_read( /* some arguments */ ) const override
   {
      /* Reading from the custom persistence system */
      return true;
   }

   bool do_write( /* some arguments */ ) override
   {
      /* Writing to the custom persistence system */
      return true;
   }

   // ... More database specific functionality

 private:
   // ... Potential implementation details and data members
};


//---- <Main.cpp> ---------------------------------------------------------------------------------

//#include <PersistenceInterface.h>
#include <cstdlib>

int main()
{
   CustomPersistence persistence;
   set_persistence_interface( &persistence );

   Widget widget{ get_persistence_interface() };
   widget.doSomething();

   // ...

   return EXIT_SUCCESS;
}

