cmake_minimum_required(VERSION 3.15)
project(mtm_ex3)

set(CMAKE_CXX_STANDARD 11)

#include_directories(.)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

#add_executable(ex3 main.cpp UniqueArray.h tests)




add_executable(mtm_ex3
        #tests/UniqueArrayTest.cpp
        #MtmParkingLot.cpp
        main.cpp
        ParkingLot.h
        ParkingLotPrinter.cpp
        ParkingLotPrinter.h
        ParkingLotTypes.h
        ParkingSpot.cpp
        ParkingSpot.h
        Time.cpp
        Time.h
        UniqueArray.h
        UniqueArrayImp.h)
#Vehicle.h)