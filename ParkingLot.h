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

// consts definitions
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

    class Vehicle {
    private:
        LicensePlate license_plate;
        VehicleType vehicle_type;
        Time time_of_entrance;
        bool got_fined;
        ParkingSpot parkingSpot;
    public:
        Vehicle();
        explicit Vehicle(LicensePlate& plate_number);
        Vehicle(LicensePlate& plate_number, VehicleType type, Time& time);
        ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle&) = default;
        bool operator== (const Vehicle& v1)const;
        bool operator<(const Vehicle& v1)const{
            return parkingSpot<v1.parkingSpot;
        }
        LicensePlate getLicensePlate()const{return license_plate;}
        VehicleType getType()const{return vehicle_type;}
        Time getEntranceTime()const{return time_of_entrance;}
        void setGotFined(){got_fined= true;}
        bool getIfVehicleIsFined()const{return got_fined;}
        void setParkingSpot(ParkingSpot parkingSpot_to_set){
            parkingSpot=parkingSpot_to_set;
        }
        ParkingSpot getVehicleParkingSpot()const{return parkingSpot;}

    };

    Vehicle:: Vehicle() :
        license_plate("DEFAULT"), vehicle_type(FIRST),
        time_of_entrance(0, 0, 0), got_fined(false), parkingSpot(MOTORBIKE, 0) {
    }

    Vehicle:: Vehicle(LicensePlate &plate_number) :
        license_plate(plate_number), vehicle_type(FIRST),
        time_of_entrance(0, 0, 0), got_fined(false), parkingSpot(MOTORBIKE, 0) {
    }

    Vehicle:: Vehicle(LicensePlate &plate_number, VehicleType type, Time& time):
        license_plate(plate_number), vehicle_type(type),
        time_of_entrance(time), got_fined(false), parkingSpot(MOTORBIKE, 0) {
    }

    bool Vehicle::operator==(const Vehicle &v1) const {
        return license_plate==v1.license_plate;
    }


    /**
        *The class that will be sent to the UniqueArray template, and will
        * be used there as the compare function
        * the operator () will return if v1==v2 (using the == of vehicle)
        * that will return if the license plate of v1==license plate of v2
        * (using the operator == of String)
       */
    class Compare {
    public:
        Compare()= default;
        ~Compare()= default;
        Compare(const Compare& other)=delete;
        Compare& operator=(const Compare& other)=delete;

        bool operator() (const Vehicle& vehicle_1, const Vehicle& vehicle_2) {
            return vehicle_1==vehicle_2;
        }
    };


    /**
        *The class that wii be used as the filter in the filter function
        * at UniqueArray
        * the operator () will return if at the time of the inspection
        * the vehicle stayed more than 24 hours
        */
    class MoreThan24Hours :public UniqueArray<Vehicle,Compare>::Filter {
    private:
        Time inspectionTime;
    public:
        explicit MoreThan24Hours(Time& inspection):inspectionTime(inspection) {}
        ~MoreThan24Hours()= default;
        MoreThan24Hours(const MoreThan24Hours& other)= delete;
        MoreThan24Hours&operator=(const MoreThan24Hours& other)= delete;

        bool operator()(const Vehicle& vehicle)const override {
            Time totalHours=inspectionTime-vehicle.getEntranceTime();
            return totalHours.toHours()> HOURS_PER_DAY;
        }
    };


    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;
        const Vehicle* getVehicleFromLicensePlate(LicensePlate& licensePlate)
            const;
        bool checkIfExistsSpot(VehicleType vehicleType);
        void enterVehicleToParking(Vehicle &register_vehicle,
                VehicleType vehicleType);
        static int getPriceForVehicleAtExit(const Vehicle& vehicle,
                const Time& exit_time);
        static unsigned int filterUniqueArray(UniqueArray<Vehicle,
                Compare>& wanted_uq,Time& inspectionTime);
        static void enterVehiclesToVectorFromArray(std::vector<Vehicle>&
                parking_lot_vector,const UniqueArray<Vehicle,
                Compare>& wanted_uq) ;
    public:
        explicit ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot() = default;
        ParkingLot(const ParkingLot& other) = default;
        ParkingLot& operator=(const ParkingLot&) = delete;
        ParkingResult enterParking(VehicleType vehicleType,
                LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate,
                ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

    };

    /**
        * @brief returns a pointer to a vehicle in the parking lot with
        * the same license plate as the given to the function
        *
        * @param licensePlate- the wanted licenseplate
        * @return pointer to the wanted vehicle if such exists
        * @return NULL-otherwise
        */
    const Vehicle* ParkingLot::getVehicleFromLicensePlate(
            LicensePlate& licensePlate)const{
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
        return exists;
    }

    /**
         * @brief returns if there is a free spot that a vehicle with the
         * given VehicleType can park there
         *
         * @param vehicleType- the type of the vehicle
         * @return true- if there is a free spot for a vehicle with the given VT
         * @return false- if there isn't such a spot
         */
    bool ParkingLot:: checkIfExistsSpot(VehicleType vehicleType){
        if(vehicleType==MOTORBIKE){
            return motorbikes_arr.getCount() != motorbikes_arr.getSize();
        }
        if(vehicleType==HANDICAPPED){
            if(handicapped_cars_arr.getCount()!=handicapped_cars_arr.getSize()){
                return true;
            }
        }
        return (cars_arr.getSize() != cars_arr.getCount());
    }

    /**
         * @brief enters a vehicle to the parking lot
         *
         * @param registered_vehicle-the vehicle we should enter
         * @param vehicleType-the VehicleType of the vehicle
         */
    void MtmParkingLot::ParkingLot:: enterVehicleToParking(
            Vehicle& registered_vehicle, VehicleType vehicleType){
        unsigned int index = 0;
        if(vehicleType==MOTORBIKE){
            index = motorbikes_arr.insert(registered_vehicle);
            registered_vehicle.setParkingSpot(ParkingSpot(vehicleType, index));
            (*motorbikes_arr.getElement(index)).setParkingSpot(
                    registered_vehicle.getVehicleParkingSpot());
            return ;
        }
        if(vehicleType==CAR||handicapped_cars_arr.getCount()==
            handicapped_cars_arr.getSize()){
            index = cars_arr.insert(registered_vehicle);
            registered_vehicle.setParkingSpot(ParkingSpot(CAR, index));
            (*cars_arr.getElement(index)).setParkingSpot(
                    registered_vehicle.getVehicleParkingSpot());
            return ;
        }
        index = handicapped_cars_arr.insert(registered_vehicle);
        registered_vehicle.setParkingSpot(ParkingSpot(vehicleType, index));
        (*handicapped_cars_arr.getElement(index)).setParkingSpot(
                registered_vehicle.getVehicleParkingSpot());
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
    int ParkingLot::getPriceForVehicleAtExit(const Vehicle& vehicle,
            const Time& exit_time){
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
                price=MOTORBIKE_FIRST_HOUR_PRICE +
                        MOTORBIKE_EXTRA_HOUR_PRICE*(total_hours-1);
            }
        } else{
            if(total_hours>MAX_PAID_HOURS){
                price=CAR_MAX_PRICE;
            } else{
                price=CAR_FIRST_HOUR_PRICE +
                        CAR_EXTRA_HOUR_PRICE*(total_hours-1);
            }
        }
        if(vehicle.getIfVehicleIsFined()){
            price=price+FINE;
        }
        return price;
    }


    /**
       * @brief filters the vehicles that are over 24 hours and sets the
       * the got_fined of the vehicle accordenly
       *
       * @param vehicle-the wanted vehicle
       * @param exit_time- exit time of the given vehicle from the parking_lot
       *
       * return- the amount vehicles that passed the filter (meaning-are over
       * 24 hours)
       */
    unsigned int ParkingLot::filterUniqueArray(UniqueArray<Vehicle,
            Compare>& wanted_uq,Time& inspectionTime){
        MoreThan24Hours inspection(inspectionTime);
        UniqueArray<Vehicle,Compare> uq_filtered((wanted_uq.filter(inspection)));
        int count=0;
        for(unsigned int i=0;i<wanted_uq.getSize();i++){
            if((uq_filtered.getElement(i))!=nullptr){
                if(!(*(wanted_uq.getElement(i))).getIfVehicleIsFined()){
                    count++;
                }
                (*(wanted_uq.getElement(i))).setGotFined();
            }
        }
        return count;
    }


    /**
      * @brief copies the vehicle from the given UniqueArray to the given vector
      *
      * @param parking_lot_vector-the vector we should insert the vehicles from
      * the given uniqueArray
      * @param wanted_uq-the UniquArray from which we should copy vehicles to
      * the vector
      */
    void ParkingLot::enterVehiclesToVectorFromArray(std::vector<Vehicle>&
    parking_lot_vector,const UniqueArray<Vehicle,Compare>& wanted_uq) {
        for(unsigned int i=0; i <wanted_uq.getSize(); i++){
            if(wanted_uq.getElement(i)!=nullptr) {
                parking_lot_vector.push_back(*(wanted_uq.getElement(i)));
            }
        }
    }


    ParkingLot:: ParkingLot(unsigned int parkingBlockSizes[]) :
            motorbikes_arr ( UniqueArray<Vehicle,Compare>(
                    parkingBlockSizes[FIRST_BLOCK_SIZES_INDEX])),
            handicapped_cars_arr ( UniqueArray<Vehicle,Compare>(
                    parkingBlockSizes[SECOND_BLOCK_SIZES_INDEX])),
            cars_arr ( UniqueArray<Vehicle,Compare>(
                    parkingBlockSizes[LAST_BLOCK_SIZES_INDEX])){
    }

    ParkingResult ParkingLot:: getParkingSpot(LicensePlate licensePlate,
            ParkingSpot& parkingSpot) const {
        const Vehicle* vehicle_in_arr=getVehicleFromLicensePlate(licensePlate);
        if(vehicle_in_arr != nullptr) {
            parkingSpot = (*vehicle_in_arr).getVehicleParkingSpot();
            return SUCCESS;
        }
        return VEHICLE_NOT_FOUND;
    }

    ParkingLotUtils:: ParkingResult ParkingLot:: enterParking(
            VehicleType vehicleType, LicensePlate licensePlate,
            Time entranceTime){
        Vehicle new_vehicle(licensePlate,vehicleType,entranceTime);
        ParkingSpot spot;
        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists!=nullptr){
            spot=(*exists).getVehicleParkingSpot();
            ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,
                    (*exists).getEntranceTime());
            ParkingLotPrinter::printEntryFailureAlreadyParked(cout,spot);
            return VEHICLE_ALREADY_PARKED;
        }
        if(!checkIfExistsSpot(vehicleType)){
            ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,
                    entranceTime);
            ParkingLotPrinter::printEntryFailureNoSpot(cout);
            return NO_EMPTY_SPOT;
        }
        enterVehicleToParking(new_vehicle,vehicleType);
        spot=new_vehicle.getVehicleParkingSpot();
        ParkingLotPrinter::printVehicle(cout,vehicleType,licensePlate,
                entranceTime);
        ParkingLotPrinter::printEntrySuccess(cout,spot);
        return SUCCESS;
    }

    ParkingLotUtils:: ParkingResult ParkingLot::
    exitParking(LicensePlate licensePlate, Time exitTime){
        const Vehicle* exists= getVehicleFromLicensePlate(licensePlate);
        if(exists==nullptr) {
            ParkingLotPrinter::printExitFailure(cout, licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        int price=getPriceForVehicleAtExit(*exists,exitTime);
        ParkingLotPrinter::printVehicle(cout, (*exists).getType(),licensePlate,
                (*exists).getEntranceTime());
        ParkingSpot parking_spot=(*exists).getVehicleParkingSpot();
        unsigned int index;
        if(parking_spot.getParkingBlock()==MOTORBIKE){
            motorbikes_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,
                    price);
            return SUCCESS;
        }else if(parking_spot.getParkingBlock()==CAR){
            cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,
                    price);
            return SUCCESS;
        }else if(handicapped_cars_arr.getIndex(*exists, index)){
            handicapped_cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,
                    price);
            return SUCCESS;
        }else{
            cars_arr.remove(*exists);
        }
        ParkingLotPrinter::printExitSuccess(cout,parking_spot,exitTime,price);
        return SUCCESS;
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime){
        unsigned int count_fined=0;
        count_fined=count_fined+filterUniqueArray(motorbikes_arr,
                inspectionTime);
        count_fined=count_fined+filterUniqueArray(cars_arr,inspectionTime);
        count_fined=count_fined+filterUniqueArray(handicapped_cars_arr,
                inspectionTime);
        ParkingLotPrinter::printInspectionResult(cout,inspectionTime,
                count_fined);
    }

    ostream& operator<<(ostream& os, const ParkingLot& parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os);

        std::vector<Vehicle> parking_lot_vector;
        //insert vehicles to the vector
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.motorbikes_arr);
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.handicapped_cars_arr);
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.cars_arr);
        //sorting the vehicle
        std::sort(parking_lot_vector.begin(), parking_lot_vector.end());
        //printing the vector
        for(unsigned int j = 0; j < parking_lot_vector.size(); j++) {
            ParkingLotPrinter::printVehicle(os, parking_lot_vector[j].getType(),
                    parking_lot_vector[j].getLicensePlate(),
                    parking_lot_vector[j].getEntranceTime());
            ParkingSpot parking_spot;
            parkingLot.getParkingSpot(parking_lot_vector[j].getLicensePlate(),
                    parking_spot);
            ParkingLotPrinter::printParkingSpot(os, parking_spot);
        }
        return os;
    }
}

#endif //MTMPARKINGLOT_PARKINGLOT_H
