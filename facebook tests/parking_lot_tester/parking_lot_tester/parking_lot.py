from vehicle import *
import sys

def printParkingSpotNoEndl(print_func, parkingSpot):
    print_func(", parked in: " + str(parkingSpot), end="")


def printParkingSpot(print_func, parkingSpot):
    printParkingSpotNoEndl(print_func, parkingSpot)
    print_func("")


def printVehicle(print_func, vehicle_type, license, entrance_time):
    print_func("Vehicle Type: " + str(vehicle_type) + ", License Plate: " + license
               + ", arrived on: " + str(entrance_time), end="")


def printEntrySuccess(print_func, parkingSpot):
    print_func(" has successfully entered the Parking Lot", end="")
    printParkingSpot(print_func, str(parkingSpot))


def printEntryFailureNoSpot(print_func, ):
    print_func(" failed entering Parking Lot : no available parking spot!")


def printEntryFailureAlreadyParked(print_func, spot):
    print_func(" is already in the Parking Lot", end="")
    printParkingSpot(print_func, spot)


def printExitSuccess(print_func, spot, exit_time, price):
    printParkingSpotNoEndl(print_func, spot)
    print_func(" has exited Parking Lot on: " + str(exit_time) + ", Total price to pay: " + str(price))


def printExitFailure(print_func, license):
    print_func("Error: Vehicle with License Plate: " + license + " not found!")


def printInspectionResult(print_func, inspectionTime, count):
    print_func("Parking lot was inspected on: " + str(inspectionTime) + ". Total of "
               + str(count) + " vehicles were fined.")


def printParkingLotTitle(print_func):
    print_func("=============")
    print_func("MtmParkingLot")
    print_func("=============")


class StreamWrapper(object):
    def __init__(self, stream):
        self.stream = stream

    def write_to(self, text, end="\n"):
        self.stream.write(text + end)


class ParkingLot(object):
    def __init__(self, sizes):
        self.parkings = []
        self.vehicle_to_type = {
            VehicleType.MOTORBIKE: MotorBike,
            VehicleType.CAR: Car,
            VehicleType.HANDICAPPED: Handicapped
        }
        for size in sizes:
            parking = [None for i in range(size)]
            self.parkings.append(parking)

    def get_vehicle_pos(self, license):
        for i, parking in enumerate(self.parkings):
            for j, vehicle in enumerate(parking):
                if vehicle != None and vehicle.license == license:
                    return i, j
        return None, None

    def get_first_empty_spot(self, parking_id):
        for i, spot in enumerate(self.parkings[parking_id]):
            if spot is None:
                return i
        return 0

    def enter_parking(self, vehicle_type, license, entrance_time):
        i, j = self.get_vehicle_pos(license)
        if i is not None and j is not None:
            spot = ParkingSpot(VehicleType(i), j)
            printVehicle(print, vehicle_type, license,
                         self.parkings[i][j].entrance_time)
            printEntryFailureAlreadyParked(print, spot)
            return
        printVehicle(print, vehicle_type, license, entrance_time)
        vehicle = self.vehicle_to_type[vehicle_type](vehicle_type,
                                                     license, entrance_time)
        should_be_in_parking = vehicle.get_parking(self.parkings)
        if is_full(self.parkings[int(should_be_in_parking)]):
            printEntryFailureNoSpot(print)
            return
        first_empty_spot_index = self.get_first_empty_spot(should_be_in_parking)
        self.parkings[should_be_in_parking][first_empty_spot_index] = vehicle
        printEntrySuccess(print, ParkingSpot(should_be_in_parking, first_empty_spot_index))

    def exit_parking(self, license, exit_time):
        i, j = self.get_vehicle_pos(license)
        if i is None and j is None:
            printExitFailure(print, license)
            return
        is_shitty=True
        for parking in self.parkings:
            is_shitty= is_shitty and len(parking)==0
        if is_shitty:
            return

        vehicle = self.parkings[i][j]
        printVehicle(print, vehicle.my_type, license, vehicle.entrance_time)
        price = vehicle.calc_price(exit_time)
        printExitSuccess(print, ParkingSpot(VehicleType(i), j), exit_time, price)
        self.parkings[i][j] = None

    def inspectParkingLot(self, inspection_time):
        count = 0
        for parking in self.parkings:
            for vehicle in parking:
                if vehicle is not None:
                    if (inspection_time - vehicle.entrance_time).toHours() > 24 and \
                            not vehicle.got_ticket:
                        vehicle.got_ticket = True
                        count += 1
        printInspectionResult(print, inspection_time, count)

    def to_stream(self, stream):
        wrapper = StreamWrapper(stream)
        printParkingLotTitle(wrapper.write_to)
        for idx, parking in enumerate(self.parkings):
            for v_idx, vehicle in enumerate(parking):
                if vehicle != None:
                    printVehicle(wrapper.write_to, vehicle.my_type, vehicle.license, vehicle.entrance_time)
                    printParkingSpot(wrapper.write_to, ParkingSpot(VehicleType(idx), v_idx))


if __name__ == '__main__':
    parking_lot = ParkingLot([1, 2, 3])
    parking_lot.enter_parking(VehicleType.MOTORBIKE, "Im cripple0", Time())
    parking_lot.exit_parking("Im cripple0", Time())
    # parking_lot.to_stream(sys.stdout)
    # parking_lot.inspectParkingLot(Time(day=5))
    # parking_lot.exit_parking("Im cripple0", Time(day=1))
    # parking_lot.to_stream(sys.stdout)
