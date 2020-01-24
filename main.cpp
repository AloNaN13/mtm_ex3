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
        array[i]=i+4;
    }
    ParkingLot parkingLot(array);
    VehicleType vt_motor=MOTORBIKE;
    VehicleType vt_car=CAR;
    LicensePlate lp0="ABCD";
    Time t_motor_in(0,3,2);
    Time t_car(0,3,4);
    Time t_motor_out(0,3,7);
    parkingLot.enterParking(vt_motor,lp0,t_motor_in);
    LicensePlate lp1="ABCD-car";
    parkingLot.enterParking(vt_car,lp1,t_car);
    parkingLot.exitParking(lp0, t_motor_out);
    cout<<parkingLot;




    return 0;
}