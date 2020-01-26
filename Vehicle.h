#ifndef MTM_EX3_VEHICLE_H
#define MTM_EX3_VEHICLE_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "UniqueArray.h"

#include <string>

// consts definitions
static const ParkingLotUtils::Time::Hour HOURS_PER_DAY = 24;
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

    /**
     * This class is used as a helper class for ParkingLot.
     * It holds all of the information about a vehicle in the parking lot-
     * one that is entering, exiting and such.
     *
     * Its field hold the license plate, vehicle type, entrance time, parking
     * spot and wheter it got a fine or not.
     * Its functions are ctors/dtors, operators and getters/setters, therefore
     * we saw no need to comment each and every one.
   */
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
        bool operator== (const Vehicle& vehicle_1) const;
        bool operator<(const Vehicle& vehicle_1) const;
        LicensePlate getLicensePlate() const;
        VehicleType getType() const;
        Time getEntranceTime() const;
        void setGotFined();
        bool getIfVehicleIsFined() const;
        void setParkingSpot(ParkingSpot parkingSpot_to_set);
        ParkingSpot getVehicleParkingSpot() const;
    };

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
        explicit MoreThan24Hours(Time& inspection) :
            inspectionTime(inspection) {};
        ~MoreThan24Hours()= default;
        MoreThan24Hours(const MoreThan24Hours& other)= delete;
        MoreThan24Hours&operator=(const MoreThan24Hours& other)= delete;

        bool operator() (const Vehicle& vehicle)const override {
            Time totalHours=inspectionTime-vehicle.getEntranceTime();
            return totalHours.toHours()> HOURS_PER_DAY;
        }
    };

}


#endif //MTM_EX3_VEHICLE_H
