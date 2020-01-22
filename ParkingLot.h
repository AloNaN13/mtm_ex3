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
        Vehicle(LicensePlate plate_number);
        Vehicle(LicensePlate plate_number, VehicleType type, Time time);
        ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle&) = delete;

        //other functs: getLicensePlate, >=, checkIfOvertime, more?

        //exceptions

    };

    Vehicle:: Vehicle(LicensePlate plate_number) :
        license_plate(plate_number), vehicle_type(FIRST), time_of_entrance(0, 0, 0), is_overtime(false) {
    }

    Vehicle:: Vehicle(LicensePlate plate_number, VehicleType type, Time time) :
        license_plate(plate_number), vehicle_type(type), time_of_entrance(time), is_overtime(false) {
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


        Vehicle* getVehicleFromLicensePlate(LicensePlate licensePlate);
        bool  checkIfExistsSpot(VehicleType vehicleType);
        void  enterVehicleToParking(Vehicle &register_vehicle, VehicleType vt);




        // execptions class

    };

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) {
        // create an array for each vehicle type
        this->motorbikes_arr = UniqueArray(parkingBlockSizes[0]);
        this->handicapped_cars_arr = UniqueArray(parkingBlockSizes[1]);
        this->cars_arr = UniqueArray(parkingBlockSizes[2]);

        //expections

    }

    ParkingResult ParkingLot:: getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const{

    }





    //mimush:


    Vehicle* MtmParkingLot::ParkingLot::getVehicleFromLicensePlate(LicensePlate licensePlate){
        Vehicle new_vehicle(licensePlate);
        Vehicle* exists =motorbikes_arr[new_vehicle];
        if(exists!=NULL){
            return exists;
        }
        exists=cars_arr[new_vehicle];
        if(exists!=NULL){
            return exists;
        }
        exists=handicapped_cars_arr[new_vehicle];
        return  exists;
    }




    bool MtmParkingLot::ParkingLot:: checkIfExistsSpot(VehicleType vehicleType){
        if(vehicleType==MOTORBIKE){
            return motorbikes_arr.getCount() != motorbikes_arr.getSize();
        }
        if(vehicleType==HANDICAPPED){
            if(handicapped_cars_arr.getCount()!=handicapped_cars_arr.getCount()){
                return true;
            }
        }

        return cars_arr.getSize() != cars_arr.getCount();
    }



    void MtmParkingLot::ParkingLot:: enterVehicleToParking(Vehicle &register_vehicle, VehicleType vt){
        if(vt==MOTORBIKE){
            motorbikes_arr.insert(register_vehicle);
            //assert success
            return;
        }
        else if(vt==HANDICAPPED){
            if(handicapped_cars_arr.getCount()!=handicapped_cars_arr.getSize()){
                handicapped_cars_arr.insert(register_vehicle);
                return;
            }
        }
        //if we are here than the vt is handicappes or cars and in any case we should
        //insert the vehicle to the cars section
        cars_arr.insert(register_vehicle);
    }

    ParkingLotUtils:: ParkingResult MtmParkingLot::ParkingLot:: enterParking(VehicleType vehicleType,
                                                                             LicensePlate licensePlate, Time entranceTime){
        Vehicle new_vehicle(vehicleType,licensePlate,entranceTime);
        //getVehicleFromArray(motorbikes_arr,licensePlate);

        Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        ParkingSpot spot;
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
        enterVehicleToParking(new_vehicle,vehicleType);
        ParkingLotPrinter::printEntrySuccess(cout,new_vehicle);
        return SUCCESS;
    }

    ParkingLotUtils:: ParkingResult MtmParkingLot::ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){




    }



}




#endif //MTMPARKINGLOT_PARKINGLOT_H












