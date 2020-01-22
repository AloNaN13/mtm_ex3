#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "UniqueArray.h"
#include "string"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;

    class ParkingLot {
    private:
        UniqueArray motorbikes_arr;
        UniqueArray handicapped_cars_arr;
        UniqueArray cars_arr;

    public:
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

        // execptions class

    };

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) {
        // create an array for each vehicle type
        this->motorbikes_arr = UniqueArray(parkingBlockSizes[0]);
        this->handicapped_cars_arr = UniqueArray(parkingBlockSizes[1]);
        this->cars_arr = UniqueArray(parkingBlockSizes[2]);

        //expections

    }

    ParkingLot:: ~ParkingLot() {
        ~UniqueArray(motorbikes_arr);
        ~UniqueArray(handicapped_cars_arr);
        ~UniqueArray(cars_arr);
    }


}




#endif //MTMPARKINGLOT_PARKINGLOT_H





// to put in vehicle.h

class Vehicle {
private:
    LicensePlate license_plate;
    VehicleType vehicle_type;
    Time time_of_entrance;
    bool is_overtime;
public:
    Vehicle(LicensePlate plate_number, VehicleType type, Time time);
    ~Vehicle();
    Vehicle(const Vehicle& other); // do we need this copy ctor?
    //default assign ctor
    //other functs: getLicensePlate, ==, >=, checkIfOvertime, more?

    //exceptions

};

Vehicle:: Vehicle(LicensePlate plate_number, VehicleType type, Time time) :
    license_plate(plate_number), vehicle_type(type), time_of_entrance(time), is_overtime(false) {
}

Vehicle:: ~Vehicle() {
    ~string(license_plate);
    ~Time(time_of_entrance);
}
// check the destructor for string

// do we need this copy ctor?
Vehicle(const Vehicle& other_vehicle);
Vehicle:: Vehicle(const Vehicle& other) :
    license_plate(other.license_plate), vehicle_type(other.vehicle_type), time_of_entrance(time), is_overtime(false) {
        //copy str
        //copy time
}

