#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"

#include <string>
#include <vector>
#include <algorithm>

// #include <iostream>

// const defines
static const ParkingLotUtils::Time::Hour HOURS_PER_DAY = 24;
const unsigned int FIRST_BLOCK_SIZES_INDEX = 0;
const unsigned int SECOND_BLOCK_SIZES_INDEX = 1;
const unsigned int LAST_BLOCK_SIZES_INDEX = 2;
const int ZERO_PRICE = 0;
const int MAX_PAID_HOURS = 6;
const int MOTORBIKE_FIRST_HOUR_PRICE = 10;
const int MOTORBIKE_EXTRA_HOUR_PRICE = 5;
const int MOTORBIKE_MAX_PRICE = 35;
const int HANDICAPPED_PRICE = 15;
const int CAR_FIRST_HOUR_PRICE = 20;
const int CAR_EXTRA_HOUR_PRICE = 10;
const int CAR_MAX_PRICE = 70;
const int FINE = 250;




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

        ParkingSpot parkingSpot;


    public:

        explicit Vehicle();
        explicit Vehicle(LicensePlate plate_number);
        Vehicle(LicensePlate plate_number, VehicleType type, Time time);
        ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle&) = default;
        VehicleType getType() const ;
        bool operator== (const Vehicle &v1)const;
        bool operator<(const Vehicle& v1)const{
            return parkingSpot<v1.parkingSpot;
        };
        bool getIfVehicleIsFined() const{
            return got_fined;
        }
        void setGotFined(){
            got_fined= true;
        }
        LicensePlate getLicensePlate() const{
            return license_plate;
        };
        ParkingSpot getVehicleParkingSpot()const { return parkingSpot;};
        void setParkingSpot(ParkingSpot parkingSpot_to_set){
            this->parkingSpot=parkingSpot_to_set;
        };
        Time getEntranceTime()const {
            return time_of_entrance;
        };


    };

    Vehicle:: Vehicle() :
        license_plate("DEFAULT"), vehicle_type(FIRST), time_of_entrance(0, 0, 0), got_fined(false) {}

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
        explicit MoreThan24Hours(Time inspection):inspectionTime(inspection){}
        bool operator()(const Vehicle& vehicle)const override{
            Time totalHours=inspectionTime-vehicle.getEntranceTime();
            return totalHours.toHours()> HOURS_PER_DAY;

        }
    };


    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;

        const Vehicle* getVehicleFromLicensePlate(LicensePlate licensePlate)const;
        bool checkIfExistsSpot(const VehicleType vehicleType);
        bool enterVehicleToParking(Vehicle &register_vehicle,const VehicleType vt);
        static int getPriceForVehicleAtExit(const Vehicle& vehicle,const Time exit_time);
        unsigned int filterUniqueArray(UniqueArray<Vehicle,Compare>& wanted_uq,Time& inspectionTime);
        //const bool CompareParkingSpots(Vehicle& vehicle1, Vehicle& vehicle2) const;

    public:
        explicit ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot() = default;
        ParkingLot(const ParkingLot& other) = default;
        ParkingLot& operator=(const ParkingLot&) = delete;
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);







        // exceptions class

    };

    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) :
            motorbikes_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[FIRST_BLOCK_SIZES_INDEX])),
            handicapped_cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[SECOND_BLOCK_SIZES_INDEX])),
            cars_arr ( UniqueArray<Vehicle,Compare>(parkingBlockSizes[LAST_BLOCK_SIZES_INDEX])){

        //exceptions

    }

    ParkingResult ParkingLot:: getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const {
        const Vehicle* vehicle_in_arr = getVehicleFromLicensePlate(licensePlate);
        if(vehicle_in_arr != nullptr) {
            // get parking block, get parking number, put it in parkingSpot
            parkingSpot = (*vehicle_in_arr).getVehicleParkingSpot();
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
        if(exists!=nullptr){
            return exists;
        }
        exists=cars_arr[new_vehicle];
        if(exists!=nullptr){
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
    bool MtmParkingLot::ParkingLot:: enterVehicleToParking(Vehicle& register_vehicle, const VehicleType vt){
        unsigned int index;
        if(vt==MOTORBIKE){
            try {
                index = motorbikes_arr.insert(register_vehicle);
            }catch (UniqueArray<class Element,class Compare>::UniqueArrayIsFullException& e){
                return false;
            }
            register_vehicle.setParkingSpot(ParkingSpot(vt, index));
            (*motorbikes_arr.getElement(index)).setParkingSpot(register_vehicle.getVehicleParkingSpot());
            return true;
        }
        if(vt==CAR||handicapped_cars_arr.getCount()==handicapped_cars_arr.getSize()){
            try {
                index = cars_arr.insert(register_vehicle);
            }catch (UniqueArray<class Element,class Compare>::UniqueArrayIsFullException& e){
                return false;
            }
            register_vehicle.setParkingSpot(ParkingSpot(CAR, index));
            (*cars_arr.getElement(index)).setParkingSpot(register_vehicle.getVehicleParkingSpot());
            return true;
        }
        try {
            index = handicapped_cars_arr.insert(register_vehicle);
        }catch (UniqueArray<class Element,class Compare>::UniqueArrayIsFullException& e){
            return false;
        }
        register_vehicle.setParkingSpot(ParkingSpot(vt, index));
        (*handicapped_cars_arr.getElement(index)).setParkingSpot(register_vehicle.getVehicleParkingSpot());
        return true;
    }

    ParkingLotUtils:: ParkingResult ParkingLot:: enterParking(VehicleType vehicleType,
            LicensePlate licensePlate, Time entranceTime){
        Vehicle new_vehicle(licensePlate,vehicleType,entranceTime);

        ParkingSpot spot;
        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists!=nullptr){
            //getParkingSpot(licensePlate,spot);
            spot=(*exists).getVehicleParkingSpot();
            ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,(*exists).getEntranceTime());
            ParkingLotPrinter::printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }

        if(!checkIfExistsSpot(vehicleType)){
            ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,entranceTime);
            ParkingLotPrinter::printEntryFailureNoSpot(cout);
            return NO_EMPTY_SPOT;
        }
        if(!enterVehicleToParking(new_vehicle,vehicleType)){
            ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,entranceTime);
            ParkingLotPrinter::printEntryFailureNoSpot(cout);
            return NO_EMPTY_SPOT;
        }
        //insers was succesfull

        spot=new_vehicle.getVehicleParkingSpot();
        ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,entranceTime);
        ParkingLotPrinter::printEntrySuccess(cout,spot);
        return SUCCESS;
    }

    /**
        * @brief calculets the price the given vehicle should pay
        *
        * @param vehicle-the wanted vehicle
        * @param exit_time-the exit time of the given vehicle from the parking_lot
        *
        * return- the amount the vehicle should pay at exit, considering its type
        * ,hours of stay and if it got fined
        */
    int ParkingLot::getPriceForVehicleAtExit(const Vehicle& vehicle,const Time exit_time){
        int price=ZERO_PRICE;
        Time total_stay_time=exit_time-vehicle.getEntranceTime();
        int total_hours=total_stay_time.toHours();
        if(total_hours==0){
            return ZERO_PRICE;
        }
        if(vehicle.getType()==HANDICAPPED){
            price=HANDICAPPED_PRICE;
        } else if(vehicle.getType()==MOTORBIKE){
            if(total_hours>MAX_PAID_HOURS){
                price=MOTORBIKE_MAX_PRICE;
            }
            else{
                price=MOTORBIKE_FIRST_HOUR_PRICE+ MOTORBIKE_EXTRA_HOUR_PRICE*(total_hours-1);
            }
        } else{
            if(total_hours>MAX_PAID_HOURS){
                price=CAR_MAX_PRICE;
            } else{
                price=CAR_FIRST_HOUR_PRICE+ CAR_EXTRA_HOUR_PRICE*(total_hours-1);
            }
        }

        if(vehicle.getIfVehicleIsFined()){
            price=price+FINE;
        }
        return price;
    }


    ParkingLotUtils:: ParkingResult ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){
        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists==nullptr) {
            ParkingLotPrinter::printExitFailure(cout, licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        int price=getPriceForVehicleAtExit(*exists,exitTime);
        ParkingLotPrinter::printVehicle(cout, (*exists).getType(),licensePlate,(*exists).getEntranceTime());
        ParkingSpot parking_spot=(*exists).getVehicleParkingSpot();
        //getParkingSpot(licensePlate,parkingSpot);

        unsigned int index;
        //if(exists->getType()==MOTORBIKE){
        if(parking_spot.getParkingBlock()==MOTORBIKE){
            motorbikes_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,price);
            return SUCCESS;
            //assert success
        }else if(parking_spot.getParkingBlock()==CAR){
            cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,price);
            return SUCCESS;
        }
        //value= Handicapped
        else if(handicapped_cars_arr.getIndex(*exists,index) == true){//the car is in handicapped
            handicapped_cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,price);
            return SUCCESS;
        }
        //vehicle is in the car
        else{
            cars_arr.remove(*exists);
        }
        ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,price);
        return SUCCESS;
    }

    /**
        * @brief filters the vehicles at the given u
        *
        * @param vehicle-the wanted vehicle
        * @param exit_time-the exit time of the given vehicle from the parking_lot
        *
        * return- the amount vehicles that passed the filter
        */
    unsigned int ParkingLot::filterUniqueArray(UniqueArray<Vehicle,Compare>& wanted_uq,Time& inspectionTime){
        MoreThan24Hours inspection(inspectionTime);


        UniqueArray<Vehicle,Compare> uq_filtered((wanted_uq.filter(inspection)));
        for(unsigned int i=0;i<wanted_uq.getSize();i++){
            if((uq_filtered.getElement(i))!=nullptr){
                (*(wanted_uq.getElement(i))).setGotFined();
            }

        }
        return uq_filtered.getCount();
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime){
        MoreThan24Hours inspection(inspectionTime);
        unsigned int count_fined=0;
        count_fined=count_fined+filterUniqueArray(motorbikes_arr,inspectionTime);
        count_fined=count_fined+filterUniqueArray(cars_arr,inspectionTime);
        count_fined=count_fined+filterUniqueArray(handicapped_cars_arr,inspectionTime);
        ParkingLotPrinter::printInspectionResult(cout,inspectionTime,count_fined);

    }

    /*
    const bool ParkingLot:: CompareParkingSpots(Vehicle& vehicle1, Vehicle& vehicle2) const {
        ParkingSpot parking_spot_1, parking_spot_2;
        ParkingLot::getParkingSpot(vehicle1.getLicensePlate(), parking_spot_1);
        ParkingLot::getParkingSpot(vehicle2.getLicensePlate(), parking_spot_2);
        return (parking_spot_1 < parking_spot_2);
    };
    */

    ostream& operator<<(ostream& os, const ParkingLot& parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os);
        // create a new array with all of the vehicles and sort them in it?
        /*
        unsigned int motorbikes = parkingLot.motorbikes_arr.getCount();
        unsigned  int handicapped = parkingLot.handicapped_cars_arr.getCount();
        unsigned  int cars = parkingLot.cars_arr.getCount();
        unsigned int vector_size = motorbikes + handicapped + cars;
        */

        std::vector<Vehicle> parking_lot_vector;
        for(unsigned int i=0; i < parkingLot.motorbikes_arr.getSize(); i++){
            if(parkingLot.motorbikes_arr.getElement(i)!=nullptr) {
                parking_lot_vector.push_back(*(parkingLot.motorbikes_arr.getElement(i)));
            }
        }
        for(unsigned int i=0; i < parkingLot.cars_arr.getSize(); i++) {
            if (parkingLot.cars_arr.getElement(i) != nullptr) {
                parking_lot_vector.push_back(*(parkingLot.cars_arr.getElement(i)));
            }
        }
        for(unsigned int i=0; i < parkingLot.handicapped_cars_arr.getSize(); i++){
            if(parkingLot.handicapped_cars_arr.getElement(i)!=nullptr) {
                parking_lot_vector.push_back( *parkingLot.handicapped_cars_arr.getElement(i));
            }
        }
        std::sort(parking_lot_vector.begin(), parking_lot_vector.end());

        // for the array:
        for(unsigned int j = 0; j < parking_lot_vector.size(); j++) {
            ParkingLotPrinter::printVehicle(os, parking_lot_vector[j].getType(),
                                            parking_lot_vector[j].getLicensePlate(),
                                            parking_lot_vector[j].getEntranceTime());
            ParkingSpot parking_spot;
            parkingLot.getParkingSpot(parking_lot_vector[j].getLicensePlate(), parking_spot);
            ParkingLotPrinter::printParkingSpot(os, parking_spot);
        }


        return os;
    }
    // see if i can make this function better
    // try to write it differently while no vector_size
    // try with only parkingSpot?
    // try the for loop with iterator?


}




#endif //MTMPARKINGLOT_PARKINGLOT_H















