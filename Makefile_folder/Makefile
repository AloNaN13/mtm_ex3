CC = g++
OBJS = MtmParkingLot.o ParkingLot.o Vehicle.o ParkingSpot.o ParkingLotPrinter.o Time.o
EXEC = MtmParkingLot
DEBUG_FLAG = -g
COMP_FLAG = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@

	
UniqueArray.o : UniqueArray.h UniqueArrayImp.h
	$(CC) $(COMP_FLAG) -c $(DEBUG_FLAG) UniqueArray.h UniqueArrayImp.h


MtmParkingLot.o : MtmParkingLot.cpp ParkingLot.h UniqueArray.h UniqueArrayImp.h
	$(CC) $(COMP_FLAG) -c $(DEBUG_FLAG) MtmParkingLot.cpp

ParkingLot.o : ParkingLot.h ParkingLot.cpp Vehicle.h
	$(CC) $(COMP_FLAG) -c  $(DEBUG_FLAG) ParkingLot.cpp
	
Vehicle.o : Vehicle.h Vehicle.cpp
	$(CC) $(COMP_FLAG) -c  $(DEBUG_FLAG) Vehicle.cpp
	
ParkingSpot.o : ParkingSpot.h ParkingSpot.cpp
	$(CC) $(COMP_FLAG) -c  $(DEBUG_FLAG) ParkingSpot.cpp

ParkingLotPrinter.o : ParkingLotPrinter.h ParkingLotPrinter.cpp
	$(CC) $(COMP_FLAG) -c  $(DEBUG_FLAG) ParkingLotPrinter.cpp

Time.o : Time.h Time.cpp
	$(CC) $(COMP_FLAG) -c  $(DEBUG_FLAG) Time.cpp


clean:
	rm -f $(OBJS)
