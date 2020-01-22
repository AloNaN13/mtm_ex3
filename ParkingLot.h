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
    using std::cout;

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
        VehicleType getType() const;
        

        //other functs: getLicensePlate, >=, checkIfOvertime, more?

        //exceptions

    };

    Vehicle:: Vehicle(LicensePlate plate_number) :
        license_plate(plate_number), vehicle_type(FIRST), time_of_entrance(0, 0, 0), is_overtime(false) {
    }

    Vehicle:: Vehicle(LicensePlate plate_number, VehicleType type, Time time) :
        license_plate(plate_number), vehicle_type(type), time_of_entrance(time), is_overtime(false) {
    }

    VehicleType Vehicle:: getType() const {
        return this->vehicle_type;
    }



    class ParkingLot {
    private:
        UniqueArray <Vehicle*,Compare> motorbikes_arr;
        UniqueArray <Vehicle*,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle*,Compare> cars_arr;

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
        int getPriceforVehicleAtExit(Vehicle& vehicle,Time exit_time);




        // execptions class

    };

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) :
            motorbikes_arr ( UniqueArray<Vehicle*,Compare>(parkingBlockSizes[0])),
            handicapped_cars_arr ( UniqueArray<Vehicle*,Compare>(parkingBlockSizes[1])),
            cars_arr ( UniqueArray<Vehicle*,Compare>(parkingBlockSizes[2])){

        // create an array for each vehicle type
        /*
        this->motorbikes_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[0]);
        this->handicapped_cars_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[1]);
        this->cars_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[2]);
         */

        //expections

    }

    ParkingResult ParkingLot:: getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const {
        Vehicle* vehicle_in_arr = ParkingLot::getVehicleFromLicensePlate(licensePlate);
        if(vehicle_in_arr != NULL) {
            // get parking block, get parking number, put it in parkingSpot
            VehicleType vehicle_block = *vehicle_in_arr->getType();
            unsigned int vehicle_number = -1;
            if(vehicle_block == MOTORBIKE){
                motorbikes_arr.getIndex(*vehicle_in_arr, vehicle_number)
            }
            if(vehicle_block == CAR){
                cars_arr.getIndex(*vehicle_in_arr, vehicle_number)
            }
            if(vehicle_block == HANDICAPPED){
                handicapped_cars_arr.getIndex(*vehicle_in_arr, vehicle_number)
                if(vehicle_number == -1){
                    cars_arr.getIndex(*vehicle_in_arr, vehicle_number)
                }
            }

            ParkingSpot temp_parking_spot(vehicle_block, vehicle_number);
            parkingSpot = temp_parking_spot;
            return SUCCESS:
        }
        return VEHICLE_NOT_FOUND;
    }




    Vehicle* ParkingLot::getVehicleFromLicensePlate(LicensePlate licensePlate){
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




    bool ParkingLot:: checkIfExistsSpot(VehicleType vehicleType){
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

    ParkingLotUtils:: ParkingResult ParkingLot:: enterParking(VehicleType vehicleType,
                                                                             LicensePlate licensePlate, Time entranceTime){
        Vehicle new_vehicle(licensePlate,vehicleType,entranceTime);
        //getVehicleFromArray(motorbikes_arr,licensePlate);

        Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        ParkingSpot spot;
        if(exists!=NULL){
            getParkingSpot(licensePlate,spot);
            ParkingLotPrinter::printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }

        //vehicle isn't parked already
        if(!checkIfExistsSpot(vehicleType)){
            ParkingLotPrinter::printEntryFailureNoSpot(cout);
            return NO_EMPTY_SPOT;
        }
        enterVehicleToParking(new_vehicle,vehicleType);
        getParkingSpot(licensePlate,spot);
        ParkingLotPrinter::printEntrySuccess(cout,spot);
        return SUCCESS;
    }



    int getPriceforVehicleAtExit(Vehicle& vehicle,Time exit_time){
        int price=0;
        if(vehicle.getType()==HANDICAPPED){
            price=15;
        }
        else{

        }


        if(vehicle.gotfined()){
            price=price+250;

        }
        return price;
    }


    ParkingLotUtils:: ParkingResult MtmParkingLot::ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){
        Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists==NULL){
            ParkingLotPrinter::printExitFailure(cout,licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        //Move to outside function
        if(exists->getType()==MOTORBIKE){
            motorbikes_arr.remove(&exists);
            //assert success
        }
        if(exists->getType()==CAR){
            cars_arr.remove(&exists);
        }
        //value= Handicapped
        int index;
        if(handicapped_cars_arr.getIndex(&exists,index)){//the car is in handicapped
            handicapped_cars_arr.remove(&exists);
        }
        //vechile is in the car
        cars_arr.remove(&exists);
        //get the price


        ParkingLotPrinter::printExitSuccess(cout,licensePlate,exitTime,price);
        return SUCCESS;





    }



}




#endif //MTMPARKINGLOT_PARKINGLOT_H












