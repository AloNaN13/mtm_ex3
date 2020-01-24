from random import randint, choices, choice
import string, sys

MAX_SIZE = 10
MAX_LINES_IN_FILE = 100
LICENSE_PLATES_NUM = 15
MAX_LICENSE_PLATE_LENGTH = 7
MIN_LICENSE_PLATE_LENGTH = 4
MAX_TIME = 1000
NUM_OF_FILES = 5
weights = [0.5, 0.07, 0.3, 0.1, 0.03]

FILE_PATH = "io_files\input"
FILE_END = ".txt"
TYPES = ["Motorbike", "Car", "Handicapped"]


def gen_enter(plate):
    output = "ENTER " + plate[1] + " " + plate[0] + "\n"
    return output


def gen_pass_time(plate):
    time = randint(1, MAX_TIME)
    output = "PASS_TIME " + str(time) + "\n"
    return output


def gen_exit(plate):
    output = "EXIT " + plate[0] + "\n"
    return output


def gen_inspection(plate):
    output = "INSPECT" + "\n"
    return output


def gen_print(plate=""):
    output = "PRINT" + "\n"
    return output


def gen_sizes():
    motorbike_size = str(randint(1, MAX_SIZE)) + '\n'
    car_size = str(randint(1, MAX_SIZE)) + '\n'
    handicapped_size = str(randint(1, MAX_SIZE)) + '\n'
    return motorbike_size + car_size + handicapped_size


def gen_license_plates():
    plates = []

    for i in range(LICENSE_PLATES_NUM):
        license_plate = ''.join(choice(string.ascii_uppercase + string.digits + string.ascii_lowercase) for _ in
                                range(randint(MIN_LICENSE_PLATE_LENGTH, MAX_LICENSE_PLATE_LENGTH)))
        while(license_plate in [p[0] for p in plates]):
            license_plate = ''.join(choice(string.ascii_uppercase + string.digits + string.ascii_lowercase) for _ in
                                    range(randint(MIN_LICENSE_PLATE_LENGTH, MAX_LICENSE_PLATE_LENGTH)))
        plates.append((license_plate, choice(TYPES)))
    return plates


gen_funcs = [gen_enter, gen_pass_time, gen_exit, gen_inspection, gen_print]


def randomize_line(plate):
    output = (choices(gen_funcs, weights)[0](plate))
    return output


def calc_file_path(i):
    return FILE_PATH + str(i + 1) + FILE_END


def main():
    license_plates = gen_license_plates()
    output = ""
    output += gen_sizes()
    for i in range(MAX_LINES_IN_FILE):
        plate = choice(license_plates)
        output += randomize_line(plate)
    output += gen_print()
    # print (output)
    return output

def run_input(num_of_files):
    lst=[]
    for i in range(num_of_files):
        output = main()
        pth=calc_file_path(i)
        f = open(pth, "w")
        f.write(output)
        f.close()
        lst.append(pth)
    return lst

if __name__ == "__main__":
    if len(sys.argv) == 2:
        NUM_OF_FILES = int(sys.argv[1])
    run_input(NUM_OF_FILES)
