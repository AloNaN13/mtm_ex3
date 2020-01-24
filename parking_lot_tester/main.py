import os
import sys

import input_gen, system_wrapper
import filecmp

def is_equals(file1_path,file2_path):
    with open(file1_path,'r') as file1:
        with open(file2_path,'r') as file2:
            lines1=file1.readlines()
            lines2=file2.readlines()

    if len(lines1)!=len(lines2):
        return False
    for i in range(len(lines1)):
        if lines1[i]!=lines2[i]:
            return False
    return True

if __name__ == '__main__':
    print("Welcome to the automatic tester.")
    NUMBER_OF_FILES = 100
    EXE_NAME="mtm_ex3.exe"
    print("Creating input files.")
    inputs = input_gen.run_input(NUMBER_OF_FILES)
    print("Input files created.")

    outputs_valid = ["io_files\\output_valid{}.txt".format(i + 1) for i in range(NUMBER_OF_FILES)]
    outputs_test = ["io_files\\output_test{}.txt".format(i + 1) for i in range(NUMBER_OF_FILES)]
    print("Creating output files.")

    for idx, input in enumerate(inputs):
        system_wrapper.run(input, outputs_valid[idx])
        os.system("{} {} > {}".format(EXE_NAME,input, outputs_test[idx]))
    sys.stdout=sys.__stdout__
    print("Output files created... Now it the time to go to the bathroom...")
    all_valid=True
    print("Comparing files - if files won't be equal, they would be printed.")
    for i in range(len(outputs_valid)):
        is_valid=is_equals(outputs_valid[i],outputs_test[i])
        all_valid=all_valid and is_valid
        if (not is_valid):
            print("{} and {} are equals: {}".format(outputs_valid[i],outputs_test[i],is_valid))
    if all_valid:
        print("Finished successfully.")
        print("NO MORE MATAM FOR YOU! YIHI!")
        print('(: prodk de lo rdrydw od hy prodkv prodkv')