#include "Vehicle.h"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;

    Vehicle:: Vehicle() :
            license_plate("DEFAULT"), vehicle_type(FIRST),
            time_of_entrance(0,0,0),got_fined(false),parkingSpot(MOTORBIKE,0){
    }

    Vehicle:: Vehicle(LicensePlate &plate_number) :
            license_plate(plate_number), vehicle_type(FIRST),
            time_of_entrance(0,0,0),got_fined(false),parkingSpot(MOTORBIKE,0){
    }

    Vehicle:: Vehicle(LicensePlate &plate_number, VehicleType type, Time& time):
            license_plate(plate_number), vehicle_type(type),
            time_of_entrance(time),got_fined(false),parkingSpot(MOTORBIKE,0){
    }

    bool Vehicle::operator==(const Vehicle& vehicle_1) const {
        return license_plate==vehicle_1.license_plate;
    }

    bool Vehicle::operator<(const Vehicle& vehicle_1) const {
        return parkingSpot<vehicle_1.parkingSpot;
    }

    LicensePlate Vehicle::getLicensePlate() const {
        return license_plate;
    }

    VehicleType Vehicle::getType() const {
        return vehicle_type;
    }

    Time Vehicle::getEntranceTime() const {
        return time_of_entrance;
    }

    void Vehicle::setGotFined() {
        got_fined= true;
    }

    bool Vehicle::getIfVehicleIsFined() const {
        return got_fined;
    }

    void Vehicle::setParkingSpot(ParkingSpot parkingSpot_to_set) {
        parkingSpot=parkingSpot_to_set;
    }

    ParkingSpot Vehicle::getVehicleParkingSpot() const {
        return parkingSpot;
    }

}