from enum import IntEnum
from math import floor


class ParkingSpot(object):
    def __init__(self, parking_block, number):
        self.parking_block = parking_block
        self.number = number

    def __str__(self):
        return str(self.parking_block) + "-" + str(self.number)


class VehicleType(IntEnum):
    MOTORBIKE = 0
    HANDICAPPED = 1
    CAR = 2

    def __str__(self):
        vehicle_to_str = {
            VehicleType.MOTORBIKE: "Motorbike",
            VehicleType.HANDICAPPED: "Handicapped",
            VehicleType.CAR: "Car",
        }
        return vehicle_to_str[self]


def is_full(lst):
    if len(lst) == 0:
        return True
    for x in lst:
        if x == None:
            return False
    return True


class Time(object):
    def __init__(self, day=0, hour=0, minute=0):
        if (hour > 24 or minute > 60):
            raise Exception("Hour must be <= 24 and Minute must be <= 60")

        self.day = day
        self.hour = hour
        self.minute = minute

    def __add__(self, other):
        return fromMinutes((toMinutes(self) + toMinutes(other)))

    def __sub__(self, other):
        minutes = abs(toMinutes(self) - toMinutes(other))
        return fromMinutes(minutes)

    def __str__(self):
        return "Day {}, {:02d}:{:02d}".format(self.day, self.hour, self.minute)

    def toHours(self):
        return self.day * 24 + self.hour + int(self.minute > 0)


def fromMinutes(minutes):
    hours = floor(minutes / 60)
    minutes = floor(minutes % 60)
    days = floor(hours / 24)
    hours = hours % 24
    return Time(days, hours, minutes)


def toMinutes(time):
    return (time.day * 24 * 60 + time.hour * 60 + time.minute)


def calc_defaulty_price(first_hour_price, other_hour_price, entrance_time,
                        exit_time):
    price = 0
    hours_amount = (exit_time - entrance_time).toHours()
    if hours_amount >= 1:
        price += first_hour_price
        hours_amount -= 1
    for i in range(5):
        if hours_amount >= 1:
            price += other_hour_price
            hours_amount -= 1
    return price


class Vehicle(object):
    def __init__(self, my_type, license, entrance_time):
        self.license = license
        self.my_type = my_type
        self.entrance_time = entrance_time
        self.got_ticket = False

    def calc_price(self, exit_time):
        if self.got_ticket:
            return 250
        return 0

    def get_parking(self, parkings):
        return self.my_type


class MotorBike(Vehicle):

    def calc_price(self, exit_time):
        self.first_hour_price = 10
        self.other_hour_price = 5

        return calc_defaulty_price(self.first_hour_price, self.other_hour_price,
                                   self.entrance_time, exit_time) + 250 * self.got_ticket


class Car(Vehicle):
    def calc_price(self, exit_time):
        self.first_hour_price = 20
        self.other_hour_price = 10

        return calc_defaulty_price(self.first_hour_price, self.other_hour_price,
                                   self.entrance_time, exit_time) + 250 * self.got_ticket


class Handicapped(Vehicle):
    def calc_price(self, exit_time):
        if (self.entrance_time - exit_time).toHours() == 0:
            return 0
        return 15 + 250 * int(self.got_ticket)

    def get_parking(self, parkings):
        if (is_full(parkings[int(VehicleType.HANDICAPPED)])):
            return VehicleType.CAR
        return VehicleType.HANDICAPPED


str_to_vehicle = {
    "Motorbike": VehicleType.MOTORBIKE,
    "Handicapped": VehicleType.HANDICAPPED,
    "Car": VehicleType.CAR,

}
