#!/bin/python -u
# Usage: ./judge.py TESTCASE=[testcase ID] TARGET=[student directory]

from pwn import *
from shutil import copy as cp
from os import chdir as cd, system

from answer import Answer
import errors

def copy_testcase(testcase, target):
    # skip subtask 3, as they require the students' own main.c and threads.c
    try:
        if testcase[:2] != '3-' and testcase[:2] != '4-':
            cp(f'testcase/{testcase}.c', f'{target}/threads.c')
            cp(f'testcase/{testcase}m.c', f'{target}/main.c')
        else:
            cp('testcase/main.c', f'{target}/main.c')
    except FileNotFoundError:
        print('No Submission')

def read_arguments(testcase):
    with open(f'testcase/{testcase}r') as f:
        return f.read().split()

def read_answer(testcase):
    return Answer(f'testcase/{testcase}a')

def cleanup(target):
    return system(f'cd {target}; git clean -fd >/dev/null 2>/dev/null; git reset --hard >/dev/null 2>/dev/null; find -maxdepth 1 -type p -exec rm -rf {"{}"} +')

def make():
    res = system('make >/dev/null 2>/dev/null')
    if res:
        raise errors.MakeFail

def main():
    testcase = args.TESTCASE
    target = args.TARGET
    if cleanup(target):
        print('cleanup fail')
        return
    copy_testcase(testcase, target)
    argument = read_arguments(testcase)
    answer = read_answer(testcase)
    cd(target)
    make()
    try:
        if testcase[:2] == '3-':
            answer.special_judge(argument)
        else:
            answer.execute(argument)
    except Exception as e:
        print(type(e))

if __name__ == '__main__':
    main()
