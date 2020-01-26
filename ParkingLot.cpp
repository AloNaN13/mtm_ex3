#include "ParkingLot.h"

#include <string>
#include <vector>
#include <algorithm>


namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;
    using std::cout;
    using std::vector;
    using std::sort;

    /**
    * The implementation of all the private functions of ParkingLot
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

    unsigned int ParkingLot::filterUniqueArray(UniqueArray<Vehicle,
            Compare>& wanted_uq,Time& inspectionTime){
        MoreThan24Hours inspection(inspectionTime);
        UniqueArray<Vehicle,Compare>uq_filtered((wanted_uq.filter(inspection)));
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

    void ParkingLot::enterVehiclesToVectorFromArray(vector<Vehicle>&
    parking_lot_vector,const UniqueArray<Vehicle,Compare>& wanted_uq) {
        for(unsigned int i=0; i <wanted_uq.getSize(); i++){
            if(wanted_uq.getElement(i)!=nullptr) {
                parking_lot_vector.push_back(*(wanted_uq.getElement(i)));
            }
        }
    }


    /**
    * The implementation of all the public functions of ParkingLot
    */

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
        ParkingSpot spot=(*exists).getVehicleParkingSpot();
        unsigned int index;
        if(spot.getParkingBlock()==MOTORBIKE){
            motorbikes_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,spot,exitTime,price);
            return SUCCESS;
        }else if(spot.getParkingBlock()==CAR){
            cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,spot,exitTime,price);
            return SUCCESS;
        }else if(handicapped_cars_arr.getIndex(*exists, index)){
            handicapped_cars_arr.remove(*exists);
            ParkingLotPrinter::printExitSuccess(cout,spot,exitTime,price);
            return SUCCESS;
        }else{
            cars_arr.remove(*exists);
        }
        ParkingLotPrinter::printExitSuccess(cout,spot,exitTime,price);
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

        vector<Vehicle> parking_lot_vector;
        //insert vehicles to the vector
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.motorbikes_arr);
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.handicapped_cars_arr);
        parkingLot.enterVehiclesToVectorFromArray(parking_lot_vector,
                parkingLot.cars_arr);
        //sorting the vehicle
        sort(parking_lot_vector.begin(), parking_lot_vector.end());
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

