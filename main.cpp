#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingLotPrinter.h"
#include "ParkingLot.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>

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
        array[i]=i;
    }
    ParkingLot parkingLot(array);
    VehicleType vt=MOTORBIKE;
    LicensePlate lp="ABCD";
    Time t(0,3,2);
    parkingLot.enterParking(vt,lp,t);


    return 0;
}