#==================================================================================================
#
#  Makefile for the code examples of the book "C++ Software Design"
#
#  Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
#
#  This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
#  (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
#
#==================================================================================================


# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Werror


# Setting the source and binary files
SRC = $(wildcard *.cpp)
BIN = $(SRC:.cpp=)


# Rules
default: G15_Procedural_Solution \
         G15_Object_Oriented_Solution \
         G16_Cyclic_Visitor \
         G17_Variant \
         G17_Visitor \
         G18_Acyclic_Visitor \
         G19_Extensive_Hierarchy \
         G19_Strategy \
         G21_Command \
         G22_Example_1 \
         G22_Example_2 \
         G23_Function \
         G23_Strategy \
         G25_Classic_Observer \
         G25_Modern_Observer \
         G26_CRTP_1 \
         G26_CRTP_2 \
         G27_StrongType \
         G28_Bridge \
         G28_Pimpl \
         G30_Prototype \
         G31_External_Polymorphism \
         G32_Type_Erasure \
         G33_Small_Buffer_Optimization \
         G33_Manual_Virtual_Dispatch \
         G34_Non_Owning_Type_Erasure_1 \
         G34_Non_Owning_Type_Erasure_2 \
         G35_Decorator_1 \
         G35_Decorator_2 \
         G36_Compile_Time_Decorator \
         G36_Runtime_Decorator \
         G38_Singleton

G15_Procedural_Solution: G15_Procedural_Solution.cpp
	$(CXX) $(CXXFLAGS) -o G15_Procedural_Solution G15_Procedural_Solution.cpp

G15_Object_Oriented_Solution: G15_Object_Oriented_Solution.cpp
	$(CXX) $(CXXFLAGS) -o G15_Object_Oriented_Solution G15_Object_Oriented_Solution.cpp

G16_Cyclic_Visitor: G16_Cyclic_Visitor.cpp
	$(CXX) $(CXXFLAGS) -o G16_Cyclic_Visitor G16_Cyclic_Visitor.cpp

G17_Variant: G17_Variant.cpp
	$(CXX) $(CXXFLAGS) -o G17_Variant G17_Variant.cpp

G17_Visitor: G17_Visitor.cpp
	$(CXX) $(CXXFLAGS) -o G17_Visitor G17_Visitor.cpp

G18_Acyclic_Visitor: G18_Acyclic_Visitor.cpp
	$(CXX) $(CXXFLAGS) -o G18_Acyclic_Visitor G18_Acyclic_Visitor.cpp

G19_Extensive_Hierarchy: G19_Extensive_Hierarchy.cpp
	$(CXX) $(CXXFLAGS) -o G19_Extensive_Hierarchy G19_Extensive_Hierarchy.cpp

G19_Strategy: G19_Strategy.cpp
	$(CXX) $(CXXFLAGS) -o G19_Strategy G19_Strategy.cpp

G21_Command: G21_Command.cpp
	$(CXX) $(CXXFLAGS) -o G21_Command G21_Command.cpp

G22_Example_1: G22_Example_1.cpp
	$(CXX) $(CXXFLAGS) -o G22_Example_1 G22_Example_1.cpp

G22_Example_2: G22_Example_2.cpp
	$(CXX) $(CXXFLAGS) -o G22_Example_2 G22_Example_2.cpp

G23_Function: G23_Function.cpp
	$(CXX) $(CXXFLAGS) -o G23_Function G23_Function.cpp

G23_Strategy: G23_Strategy.cpp
	$(CXX) $(CXXFLAGS) -o G23_Strategy G23_Strategy.cpp

G25_Classic_Observer: G25_Classic_Observer.cpp
	$(CXX) $(CXXFLAGS) -o G25_Classic_Observer G25_Classic_Observer.cpp

G25_Modern_Observer: G25_Modern_Observer.cpp
	$(CXX) $(CXXFLAGS) -o G25_Modern_Observer G25_Modern_Observer.cpp

G26_CRTP_1: G26_CRTP_1.cpp
	$(CXX) $(CXXFLAGS) -o G26_CRTP_1 G26_CRTP_1.cpp

G26_CRTP_2: G26_CRTP_2.cpp
	$(CXX) $(CXXFLAGS) -o G26_CRTP_2 G26_CRTP_2.cpp

G27_StrongType: G27_StrongType.cpp
	$(CXX) $(CXXFLAGS) -o G27_StrongType G27_StrongType.cpp

G28_Bridge: G28_Bridge.cpp
	$(CXX) $(CXXFLAGS) -o G28_Bridge G28_Bridge.cpp

G28_Pimpl: G28_Pimpl.cpp
	$(CXX) $(CXXFLAGS) -o G28_Pimpl G28_Pimpl.cpp

G30_Prototype: G30_Prototype.cpp
	$(CXX) $(CXXFLAGS) -o G30_Prototype G30_Prototype.cpp

G31_External_Polymorphism: G31_External_Polymorphism.cpp
	$(CXX) $(CXXFLAGS) -o G31_External_Polymorphism G31_External_Polymorphism.cpp

G32_Type_Erasure: G32_Type_Erasure.cpp
	$(CXX) $(CXXFLAGS) -o G32_Type_Erasure G32_Type_Erasure.cpp

G33_Small_Buffer_Optimization: G33_Small_Buffer_Optimization.cpp
	$(CXX) $(CXXFLAGS) -o G33_Small_Buffer_Optimization G33_Small_Buffer_Optimization.cpp

G33_Manual_Virtual_Dispatch: G33_Manual_Virtual_Dispatch.cpp
	$(CXX) $(CXXFLAGS) -o G33_Manual_Virtual_Dispatch G33_Manual_Virtual_Dispatch.cpp

G34_Non_Owning_Type_Erasure_1: G34_Non_Owning_Type_Erasure_1.cpp
	$(CXX) $(CXXFLAGS) -o G34_Non_Owning_Type_Erasure_1 G34_Non_Owning_Type_Erasure_1.cpp

G34_Non_Owning_Type_Erasure_2: G34_Non_Owning_Type_Erasure_2.cpp
	$(CXX) $(CXXFLAGS) -o G34_Non_Owning_Type_Erasure_2 G34_Non_Owning_Type_Erasure_2.cpp

G35_Decorator_1: G35_Decorator_1.cpp
	$(CXX) $(CXXFLAGS) -o G35_Decorator_1 G35_Decorator_1.cpp

G35_Decorator_2: G35_Decorator_2.cpp
	$(CXX) $(CXXFLAGS) -o G35_Decorator_2 G35_Decorator_2.cpp

G36_Compile_Time_Decorator: G36_Compile_Time_Decorator.cpp
	$(CXX) $(CXXFLAGS) -o G36_Compile_Time_Decorator G36_Compile_Time_Decorator.cpp

G36_Runtime_Decorator: G36_Runtime_Decorator.cpp
	$(CXX) $(CXXFLAGS) -o G36_Runtime_Decorator G36_Runtime_Decorator.cpp

G38_Singleton: G38_Singleton.cpp
	$(CXX) $(CXXFLAGS) -o G38_Singleton G38_Singleton.cpp

clean:
	@$(RM) $(BIN)


# Setting the independent commands
.PHONY: default clean
