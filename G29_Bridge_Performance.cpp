/**************************************************************************************************
*
* \file G29_Bridge_Performance.cpp
* \brief Guideline 29: Be Aware of Bridge Performance Gains and Losses
*
* Copyright (C) 2022 Klaus Iglberger - All Rights Reserved
*
* This file is part of the supplemental material for the O'Reilly book "C++ Software Design"
* (https://www.oreilly.com/library/view/c-software-design/9781098113155/).
*
* Copy-and-paste the following code into 'quick-bench.com'. Benchmark the time to determine
* the oldest person contained in a std::vector of Persons.
*
**************************************************************************************************/


#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>


//---- Benchmark configuration --------------------------------------------------------------------

constexpr size_t size( 10000 );  // Size of the generated container
constexpr size_t iterations( 10 );  // Number of benchmark iterations

#define BENCHMARK_PERSON1 1   // No bridge: all data members in class
#define BENCHMARK_PERSON2 1   // Complete bridge: all data members behind pimpl
#define BENCHMARK_PERSON3 1   // Partial bridge: only rarely used data members behind pimpl


//---- Random Number Setup ------------------------------------------------------------------------

std::random_device rd{};
const unsigned int seed( rd() );

std::mt19937 rng{};
std::uniform_int_distribution<int> dist( 1957, 2004 );

int get_random_year_of_birth()
{
   return dist( rd );
}


//---- Person implementations ---------------------------------------------------------------------

struct Person1
{
   std::string forename{ "Homer" };
   std::string surname{ "Simpson" };
   std::string address{ "712 Red Bark Lane" };
   std::string zip{ "89011" };
   std::string city{ "Henderson" };
   std::string state{ "Nevada" };
   int year_of_birth{ get_random_year_of_birth() };
};

struct Person2
{
   struct Pimpl {
      std::string forename{ "Homer" };
      std::string surname{ "Simpson" };
      std::string address{ "712 Red Bark Lane" };
      std::string zip{ "89011" };
      std::string city{ "Henderson" };
      std::string state{ "Nevada" };
      int year_of_birth{ get_random_year_of_birth() };
   };

   std::unique_ptr<Pimpl> pimpl{ new Pimpl{} };
};

struct Person3
{
   std::string forename{ "Homer" };
   std::string surname{ "Simpson" };
   int year_of_birth{ get_random_year_of_birth() };

   struct Pimpl {
      std::string address{ "712 Red Bark Lane" };
      std::string zip{ "89011" };
      std::string city{ "Henderson" };
      std::string state{ "Nevada" };
   };

   std::unique_ptr<Pimpl> pimpl{ new Pimpl{} };
};


//---- Benchmark for Person1 ----------------------------------------------------------------------

static void determineOldestPerson1(benchmark::State& state)
{
   std::vector<Person1> persons( size );

   for( auto _ : state )
   {
      benchmark::DoNotOptimize(
         std::min_element( begin(persons), end(persons), []( auto const& p1, auto const& p2 ){
            return p1.year_of_birth < p2.year_of_birth;
         } )
      );
   }
}
#if BENCHMARK_PERSON1
BENCHMARK(determineOldestPerson1)->Iterations(iterations);
#endif


//---- Benchmark for Person2 ----------------------------------------------------------------------

static void determineOldestPerson2(benchmark::State& state)
{
   std::vector<Person2> persons( size );

   for( auto _ : state )
   {
      benchmark::DoNotOptimize(
         std::min_element( begin(persons), end(persons), []( auto const& p1, auto const& p2 ){
            return p1.pimpl->year_of_birth < p2.pimpl->year_of_birth;
         } )
      );
   }
}
#if BENCHMARK_PERSON2
BENCHMARK(determineOldestPerson2)->Iterations(iterations);
#endif


//---- Benchmark for Person3 ----------------------------------------------------------------------

static void determineOldestPerson3(benchmark::State& state)
{
   std::vector<Person3> persons( size );

   for( auto _ : state )
   {
      benchmark::DoNotOptimize(
         std::min_element( begin(persons), end(persons), []( auto const& p1, auto const& p2 ){
            return p1.year_of_birth < p2.year_of_birth;
         } )
      );
   }
}
#if BENCHMARK_PERSON3
BENCHMARK(determineOldestPerson3)->Iterations(iterations);
#endif

