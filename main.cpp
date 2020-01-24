#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingLotPrinter.h"
#include "ParkingLot.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>
#include <cassert>


using std::istream;
using std::ifstream;
using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::map;

using namespace ParkingLotUtils;
using namespace MtmParkingLot;




int main() {
    std::cout << "Hello, World!" << std::endl;
    unsigned  int array[3];
    for(int i=0;i<3;i++){
        array[i]=i+4;
    }
    ParkingLot parkingLot(array);
    VehicleType vt_motor=MOTORBIKE;
    VehicleType vt_car=CAR;
    LicensePlate lp0="ABCD";
    Time t(0,3,2);
    parkingLot.enterParking(MOTORBIKE,lp0,t);
    LicensePlate lp1="RON";
    parkingLot.enterParking(CAR,lp1,t);
    parkingLot.enterParking(CAR,lp1,t);
    parkingLot.enterParking(CAR,"AVITAL",t);
    Time t2(1,5,4);
    parkingLot.inspectParkingLot(t2);


    //cout<<parkingLot;




    return 0;
}