#!/bin/python3

from os.path import isfile, isdir, join, basename
from os.path import exists as path_exists
from os import system as shell
from os import remove, makedirs, listdir
from sys import exit as sys_exit
from shutil import make_archive, rmtree, copyfile
from filecmp import cmp

from re import findall
from random import randint as rnd
from multiprocessing import Process

EXPECTED_FILES = "./tests"

def file_name_2_number(file_name):
    name = file_name.split(".")[0]
    number = findall(r"\d+", name)[-1]
    return int(number)

def get_last_file_number():
    files = [f for f in listdir(EXPECTED_FILES) if isfile(join(EXPECTED_FILES, f))]
    files.sort(key=file_name_2_number)
    last_file = files[-1] if len(files) > 0 else "test0.c"  # empty
    last_num = file_name_2_number(last_file)
    return last_num


def execute(i):
    print(f"- - - test{i} - - -")

    prog = f"./test{i}.out"
    expected = f"{EXPECTED_FILES}/test{i}.txt"

    shell(f"{prog}") # > {result}
    shell(f"echo {expected}")
    shell(f"cat {expected}")


if __name__ == "__main__":
    start_index = 1
    end_index = get_last_file_number()

    for i in range(start_index, end_index+1):
        execute(i)
