#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"
#include "string"

#include "vector"

// #inclue <iostream>

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;
    using std::cout;
    //using std::vector;
    //using std::sort;




    // to put in vehicle.h

    class Vehicle {
    private:
        LicensePlate license_plate;
        VehicleType vehicle_type;
        Time time_of_entrance;
        bool got_fined;
    public:

        Vehicle(LicensePlate plate_number);
        Vehicle(LicensePlate plate_number, VehicleType type, Time time);
        ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle&) = default;
        VehicleType getType() const;
        bool operator== (const Vehicle &v1)const;
        bool getIfVehicleIsFined() const{
            return got_fined;
        }
        Time getEntranceTime()const {
            return time_of_entrance;
        }


        //other functs: getLicensePlate, >=, checkIfOvertime, more?

        //exceptions

    };

    Vehicle:: Vehicle(LicensePlate plate_number) :
        license_plate(plate_number), vehicle_type(FIRST), time_of_entrance(0, 0, 0), got_fined(false) {
    }

    Vehicle:: Vehicle(LicensePlate plate_number, VehicleType type, Time time) :
        license_plate(plate_number), vehicle_type(type), time_of_entrance(time), got_fined(false) {
    }

    VehicleType Vehicle:: getType() const {
        return this->vehicle_type;
    }
    bool Vehicle::operator==(const Vehicle &v1) const {
        return license_plate==v1.license_plate;
    }


    class Compare {
    public:
        bool operator() (const Vehicle& v1, const Vehicle &v2){
            return v1==v2;
        }
    };








    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;

    public:
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot() = default;
        // need to implement cctor and defassign ctor
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);


        const Vehicle* getVehicleFromLicensePlate(LicensePlate licensePlate)const;
        bool  checkIfExistsSpot(VehicleType vehicleType);
        void  enterVehicleToParking(const Vehicle &register_vehicle, VehicleType vt);
        int getPriceforVehicleAtExit(const Vehicle& vehicle,const Time exit_time);




        // execptions class

    };

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) :
            motorbikes_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[0])),
            handicapped_cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[1])),
            cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[2])){

        //expections

    }

    ParkingResult ParkingLot:: getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const {
        const Vehicle* vehicle_in_arr = getVehicleFromLicensePlate(licensePlate);
        if(vehicle_in_arr != NULL) {
            // get parking block, get parking number, put it in parkingSpot
            VehicleType vehicle_block = vehicle_in_arr->getType();
            unsigned int vehicle_number = -1;
            if(vehicle_block == MOTORBIKE){
                motorbikes_arr.getIndex(*vehicle_in_arr, vehicle_number);
            }
            if(vehicle_block == CAR){
                cars_arr.getIndex(*vehicle_in_arr, vehicle_number);
            }
            if(vehicle_block == HANDICAPPED){
                handicapped_cars_arr.getIndex(*vehicle_in_arr, vehicle_number);
                if(vehicle_number == -1){
                    cars_arr.getIndex(*vehicle_in_arr, vehicle_number);
                }
            }

            ParkingSpot temp_parking_spot(vehicle_block, vehicle_number);
            parkingSpot = temp_parking_spot;
            return SUCCESS;
        }
        return VEHICLE_NOT_FOUND;
    }




    const Vehicle* ParkingLot::getVehicleFromLicensePlate(LicensePlate licensePlate)const{
        Vehicle new_vehicle(licensePlate);
        const Vehicle* exists = motorbikes_arr[new_vehicle];
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



    void MtmParkingLot::ParkingLot:: enterVehicleToParking(const Vehicle& register_vehicle, VehicleType vt){
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

        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
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



    int ParkingLot::getPriceforVehicleAtExit(const Vehicle& vehicle,const Time exit_time){
        int price=0;
        Time total_stay_time=exit_time-vehicle.getEntranceTime();
        int total_hours=total_stay_time.toHours();
        if(vehicle.getType()==HANDICAPPED){
            price=15;
        } else if(vehicle.getType()==MOTORBIKE){

            if(total_hours>6){
                price=35;
            }
            else{
                price=10+ 5*(total_hours-1);
            }
        } else{
            if(total_hours>6){
                price=70;
            } else{
                price=20+10*(total_hours-1);
            }
        }

        if(vehicle.getIfVehicleIsFined()){
            price=price+250;
        }
        return price;
    }


    ParkingLotUtils:: ParkingResult ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){
        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists==NULL){
            ParkingLotPrinter::printExitFailure(cout,licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        int price=getPriceforVehicleAtExit(*exists,exitTime);
        ParkingSpot parkingSpot;
        getParkingSpot(licensePlate,parkingSpot);


        //Move to outside function
        if(exists->getType()==MOTORBIKE){
            motorbikes_arr.remove(*exists);
            //assert success
        }
        if(exists->getType()==CAR){
            cars_arr.remove(*exists);
        }
        //value= Handicapped
        unsigned int index;
        if(handicapped_cars_arr.getIndex(*exists,index)){//the car is in handicapped
            handicapped_cars_arr.remove(*exists);
        }
        //vechile is in the car
        cars_arr.remove(*exists);
        ParkingLotPrinter::printExitSuccess(cout,parkingSpot,exitTime,price);
        return SUCCESS;
    }

    ostream& ParkingLot:: operator<<(ostream& os, const ParkingLot& parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os);
        // create a new array with all of the vehicles and sort them in it?
        unsigned int vector_size = motorbikes_arr.getSize() + handicapped_cars_arr.getSize() + cars_arr.getSize();
        vector<Vehicle> parking_lot_vector(vector_size);
        for(unsigned int i=0; i < vector_size; i++){
            parking_lot_vector[i] = 
        }


        // for the array:

        ParkingLotPrinter::printVehicle(os, vehicleType, licensePlate, entranceTime);
        ParkingLotPrinter::printParkingSpot(os, parkingSpot);



        return os;
    }


}




#endif //MTMPARKINGLOT_PARKINGLOT_H












