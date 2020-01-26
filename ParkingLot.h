#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotPrinter.h"
#include "Vehicle.h"

#include <string>
#include <vector>
#include <algorithm>

// consts definitions
const unsigned int FIRST_BLOCK_SIZES_INDEX = 0;
const unsigned int SECOND_BLOCK_SIZES_INDEX = 1;
const unsigned int LAST_BLOCK_SIZES_INDEX = 2;


namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;
    using std::vector;

    class ParkingLot {
    private:
        UniqueArray <Vehicle,Compare> motorbikes_arr;
        UniqueArray <Vehicle,Compare> handicapped_cars_arr;
        UniqueArray <Vehicle,Compare> cars_arr;

        /**
        * @brief returns a pointer to a vehicle in the parking lot with
        * the same license plate as the given to the function
        *
        * @param licensePlate- the wanted licenseplate
        * @return pointer to the wanted vehicle if such exists
        * @return NULL-otherwise
        */
        const Vehicle* getVehicleFromLicensePlate(LicensePlate& licensePlate)
            const;

        /**
         * @brief returns if there is a free spot that a vehicle with the
         * given VehicleType can park there
         *
         * @param vehicleType- the type of the vehicle
         * @return true- if there is a free spot for a vehicle with the given VT
         * @return false- if there isn't such a spot
         */
        bool checkIfExistsSpot(VehicleType vehicleType);

        /**
         * @brief enters a vehicle to the parking lot
         *
         * @param registered_vehicle-the vehicle we should enter
         * @param vehicleType-the VehicleType of the vehicle
         */
        void enterVehicleToParking(Vehicle &registered_vehicle,
                VehicleType vehicleType);

        /**
        * @brief calculates the price the given vehicle should pay
        *
        * @param vehicle-the wanted vehicle
        * @param exit_time- exit time of the given vehicle from the parking_lot
        *
        * return- the amount the vehicle should pay at exit, considering
         * its type ,hours of stay and if it got fined
        */
        static int getPriceForVehicleAtExit(const Vehicle& vehicle,
                const Time& exit_time);

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
        static unsigned int filterUniqueArray(UniqueArray<Vehicle,
                Compare>& wanted_uq,Time& inspectionTime);

        /**
      * @brief copies the vehicle from the given UniqueArray to the given vector
      *
      * @param parking_lot_vector-the vector we should insert the vehicles from
      * the given uniqueArray
      * @param wanted_uq-the UniquArray from which we should copy vehicles to
      * the vector
      */
        static void enterVehiclesToVectorFromArray(vector<Vehicle>&
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

}


#endif //MTMPARKINGLOT_PARKINGLOT_H
