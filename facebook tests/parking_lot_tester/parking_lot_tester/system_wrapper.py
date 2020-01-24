from parking_lot import *
from vehicle import *
import sys
import os


class Wrapper(object):
    def __init__(self):
        self.current_time = Time()
        print("Enter parking size for Motorbike:")
        print("Enter parking size for Handicapped:")
        print("Enter parking size for Car:")
        print("Enter commands:")

    def enter(self, args, parking_lot):
        vehicle_type = str_to_vehicle[args[0]]
        license = args[1]
        parking_lot.enter_parking(vehicle_type, license,self.current_time)

    def exit(self, args, parking_lot):
        license = args[0]
        parking_lot.exit_parking(license,self.current_time  )

    def inspect(self, args, parking_lot):
        parking_lot.inspectParkingLot(self.current_time)

    def print_all(self, args, parking_lot):
        parking_lot.to_stream(sys.stdout)

    def pass_time(self, args, parking_lot):
        diff_time = fromMinutes(int(args[0]))
        self.current_time += diff_time
        print("Current time: "+str(self.current_time))

    def handle_commands(self, commands, parking_lot):
        str_to_operation = {
            "ENTER": self.enter,
            "EXIT": self.exit,
            "INSPECT": self.inspect,
            "PRINT": self.print_all,
            "PASS_TIME": self.pass_time
        }
        for command in commands:
            if command is not None and command != '0\n':
                parts = command.split(" ")
                str_to_operation[parts[0]](parts[1:], parking_lot)
                pass

def run(input_path,output_path):
    sys.stdout = open(output_path, 'w')
    with open(input_path) as f:
        commands = f.readlines()
    new_commands = []
    for command in commands:
        new_commands.append(command.replace('\n', ''))
    # commands = list(filter(lambda a: a != "0\n", commands))
    sizes = new_commands[:3]
    sizes = [int(i) for i in sizes]
    parking_lot = ParkingLot(sizes)
    wrapper = Wrapper()
    wrapper.handle_commands(new_commands[3:], parking_lot)
    sys.stdout.close()

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Fucking shit give me file motherfucker!!!")
        exit()
    if not os.path.isfile(sys.argv[1]):
        print("Incorrect shit path give me file motherfucker!!!")
        exit()
    run(sys.argv[1],sys.argv[2])
