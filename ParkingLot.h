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
        bool got_fined;
    public:

        Vehicle(LicensePlate plate_number);
        Vehicle(LicensePlate plate_number, VehicleType type, Time time);
        ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle&) = delete;
        VehicleType getType() const;
        bool operator== (const Vehicle &v1)const;
        bool getIfVehicleIsFined() const{
            return got_fined;
        }
        void setGotFined(){
            got_fined= true;
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


class MoreThan24Hours :public UniqueArray<Vehicle,Compare>::Filter{
    private:
    Time inspectionTime;
    public:
        explicit MoreThan24Hours(Time inspectionTime):inspectionTime(inspectionTime){}
        bool operator()(const Vehicle& vehicle)const override{
            Time totalHours=inspectionTime-vehicle.getEntranceTime();
            return totalHours.toHours()>=24;
        }
    };


    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;

        const Vehicle* getVehicleFromLicensePlate(LicensePlate licensePlate)const;
        bool  checkIfExistsSpot(const VehicleType vehicleType);
        void  enterVehicleToParking(const Vehicle &register_vehicle,const  VehicleType vt);
        int getPriceForVehicleAtExit(const Vehicle& vehicle,const Time exit_time);
        unsigned int filterUniqueArray(UniqueArray<Vehicle,Compare>& wanted_uq,MoreThan24Hours& inspectopn);

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

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) :
            motorbikes_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[0])),
            handicapped_cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[1])),
            cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[2])){

        // create an array for each vehicle type
        /*
        this->motorbikes_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[0]);
        this->handicapped_cars_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[1]);
        this->cars_arr = UniqueArray <Vehicle,Compare> (parkingBlockSizes[2]);
         */

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

    /**
         * @brief returns a pointer to a vehicle in the parking lot with
         * the same license plate as the given to the function
         *
         * @param licensePlate- the wanted licenseplate
         * @return pointer to the wanted vehicle if such exists
         * @return NULL-otherwise
         */
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

    /**
         * @brief returns if there is a free spot that a vehicle with the
         * given VehicleType can park there
         *
         * @param vehicleType- the type of the vehicle
         * @return true- if there is a free spot for a vehicle with the given VT
         * @return false- if there isn't such a spot
         */

    bool ParkingLot:: checkIfExistsSpot(const VehicleType vehicleType){
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


    /**
         * @brief enters a vehicle to the parking lot
         *
         * @param register_vehicle-the vehicle we should enter
         * @param vt-the VehicleType of the vehicle
         */
    void MtmParkingLot::ParkingLot:: enterVehicleToParking(const Vehicle& register_vehicle, const VehicleType vt){
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



    int ParkingLot::getPriceForVehicleAtExit(const Vehicle& vehicle,const Time exit_time){
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
        int price=getPriceForVehicleAtExit(*exists,exitTime);
        ParkingSpot parkingSpot;
        getParkingSpot(licensePlate,parkingSpot);

        unsigned int index;
        //Move to outside function
        if(exists->getType()==MOTORBIKE){
            motorbikes_arr.remove(*exists);
            //assert success
        }else if(exists->getType()==CAR){
            cars_arr.remove(*exists);
        }
        //value= Handicapped
        else if(handicapped_cars_arr.getIndex(*exists,index)){//the car is in handicapped
            handicapped_cars_arr.remove(*exists);
        }
        //vechile is in the car
        else{
            cars_arr.remove(*exists);
        }
        ParkingLotPrinter::printExitSuccess(cout,parkingSpot,exitTime,price);
        return SUCCESS;
    }

    unsigned int ParkingLot::filterUniqueArray(UniqueArray<Vehicle,Compare>& wanted_uq,MoreThan24Hours& inspection){
        UniqueArray<Vehicle,Compare> uq_filtered =wanted_uq.filter(inspection);
        for(unsigned int i=0;i<uq_filtered.getSize();i++){
            if(!uq_filtered.getElement(i)){
                wanted_uq.getElement(i)->setGotFined();
            }
        }
        return uq_filtered.getCount();
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime){
        MoreThan24Hours inspection(inspectionTime);
        unsigned int count_fined=0;
        //UniqueArray<Vehicle,Compare> motors_filtered =motorbikes_arr.filter(inspection);
        count_fined=count_fined+filterUniqueArray(motorbikes_arr,inspection);
        count_fined=count_fined+filterUniqueArray(cars_arr,inspection);
        count_fined+filterUniqueArray(handicapped_cars_arr,inspection);
        ParkingLotPrinter::printInspectionResult(cout,inspectionTime,count_fined);
        
    }


}




#endif //MTMPARKINGLOT_PARKINGLOT_H












