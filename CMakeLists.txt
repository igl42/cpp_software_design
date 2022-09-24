#==================================================================================================
#
#  CMakeLists for the code examples of the book "C++ Software Design"
#
#  Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
#
#  This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
#  (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
#
#==================================================================================================

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CppSoftwareDesign CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(G15_Procedural_Solution
   G15_Procedural_Solution.cpp
   )

add_executable(G15_Object_Oriented_Solution
   G15_Object_Oriented_Solution.cpp
   )

add_executable(G16_Cyclic_Visitor
   G16_Cyclic_Visitor.cpp
   )

add_executable(G17_Variant
   G17_Variant.cpp
   )

add_executable(G17_Visitor
   G17_Visitor.cpp
   )

add_executable(G18_Acyclic_Visitor
   G18_Acyclic_Visitor.cpp
   )

add_executable(G19_Extensive_Hierarchy
   G19_Extensive_Hierarchy.cpp
   )

add_executable(G19_Strategy
   G19_Strategy.cpp
   )

add_executable(G21_Command
   G21_Command.cpp
   )

add_executable(G22_Example_1
   G22_Example_1.cpp
   )

add_executable(G22_Example_2
   G22_Example_2.cpp
   )

add_executable(G23_Function
   G23_Function.cpp
   )

add_executable(G23_Strategy
   G23_Strategy.cpp
   )

add_executable(G25_Classic_Observer
   G25_Classic_Observer.cpp
   )

add_executable(G25_Modern_Observer
   G25_Modern_Observer.cpp
   )

add_executable(G26_CRTP_1
   G26_CRTP_1.cpp
   )

add_executable(G26_CRTP_2
   G26_CRTP_2.cpp
   )

add_executable(G27_StrongType
   G27_StrongType.cpp
   )

add_executable(G28_Bridge
   G28_Bridge.cpp
   )

add_executable(G28_Pimpl
   G28_Pimpl.cpp
   )

add_executable(G30_Prototype
   G30_Prototype.cpp
   )

add_executable(G31_External_Polymorphism
   G31_External_Polymorphism.cpp
   )

add_executable(G32_Type_Erasure
   G32_Type_Erasure.cpp
   )

add_executable(G33_Small_Buffer_Optimization
   G33_Small_Buffer_Optimization.cpp
   )

add_executable(G33_Manual_Virtual_Dispatch
   G33_Manual_Virtual_Dispatch.cpp
   )

add_executable(G34_Non_Owning_Type_Erasure_1
   G34_Non_Owning_Type_Erasure_1.cpp
   )

add_executable(G34_Non_Owning_Type_Erasure_2
   G34_Non_Owning_Type_Erasure_2.cpp
   )

add_executable(G35_Decorator_1
   G35_Decorator_1.cpp
   )

add_executable(G35_Decorator_2
   G35_Decorator_2.cpp
   )

add_executable(G36_Compile_Time_Decorator
   G36_Compile_Time_Decorator.cpp
   )

add_executable(G36_Runtime_Decorator
   G36_Runtime_Decorator.cpp
   )

add_executable(G38_Singleton
   G38_Singleton.cpp
   )
