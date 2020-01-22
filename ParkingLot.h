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




    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;

    public:
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);


        Vehicle& getVehicleFromArray( UniqueArray <Vehicle,Compare> wanted_array,LicensePlate licensePlate);

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




    //mimush:
    Vehicle&

    bool MtmParkingLot::ParkingLot:: checkIfExistsSpot(VehicleType vehicleType){
        if(vehicleType==MOTORBIKE){
            return motorbikes.getCount() != motorbikes.getSize();
        }
        if(vehicleType==HANDICAPPED){
            if(handicappeds.getCount()!=handicappeds.getCount()){
                return true;
            }
        }

        return cars.getSize() != cars.getCount();

    }



    void MtmParkingLot::ParkingLot:: enterVehicleToparking(Vehicle &register_vehicle, VehicleType vt){
        if(vt==MOTORBIKE){
            motorbikes.insert(register_vehicle);
            //assert success
            return;
        }
        else if(vt==HANDICAPPED){
            if(handicappeds.getCount()!=handicappeds.getSize()){
                handicappeds.insert(register_vehicle);
                return;
            }
        }
        //if we are here than the vt is handicappes or cars and in any case we should
        //insert the vehicle to the cars section
        cars.insert(register_vehicle);

    }

    ParkingLotUtils:: ParkingResult MtmParkingLot::ParkingLot:: enterParking(VehicleType vehicleType,
                                                                             LicensePlate licensePlate, Time entranceTime){
        //bulid new_v
        Vehicle v(vehicleType,licensePlate,entranceTime);
        //chekc if existx in one of the unique arrays
        Vehicle* exists= motorbikes[v];
        ParkingSpot spot;

        //shihpul kod, fix later
        if(exists!=NULL){
            getParkingSpot(licensePlate,spot);
            printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }
        exists=handicappeds[v];
        if(exists!=NULL){
            getParkingSpot(licensePlate,spot);
            printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }
        exists=cars[v];
        if(exists!=NULL){
            getParkingSpot(licensePlate,spot);
            printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }
        //vehicle isn't parked already
        if(!checkIfExistsSpot(vehicleType)){
            ParkingLotPrinter::printEntryFailureNoSpot(cout);
            return NO_EMPTY_SPOT;
        }
        enterVehicleToparking(v,vehicleType);
        ParkingLotPrinter::printEntrySuccess(cout, v);
        return SUCCESS;
    }

    ParkingLotUtils:: ParkingResult MtmParkingLot::ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){




    }



}




#endif //MTMPARKINGLOT_PARKINGLOT_H












