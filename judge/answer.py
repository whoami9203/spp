import os
import stat as stat_util
from time import sleep
from pwn import *
import errors

class Answer:
    def __init__(self, filename):
        with open(filename, 'r') as f:
            self.commands = f.readlines()
            self.p = None
            self.pipes = []
            self.last = None
            self.hist = set()

    def execute(self, args):
        try:
            self.p = process(['./main'] + args, stderr=None)
            for line in self.commands:
                sep = line.split(' ')
                timeout = float(sep[0])
                operation = sep[1].strip()
                arg = ' '.join(sep[2:])
                if operation == 'O':
                    self.open_pipe(timeout, arg)
                elif operation == 'R':
                    self.expect_stdout(timeout, arg)
                elif operation == 'W':
                    self.write_pipe(timeout, arg)
                elif operation == 'D':
                    self.send_signal(timeout)
                elif operation == 'F':
                    self.check_file(timeout, arg)
        except Exception as e:
            if self.abort():
                raise e
            else:
                raise errors.RE
        self.wait_process()

    def special_judge(self, args):
        try:
            self.p = process(['./main'] + args, stderr=None)

            if args[-1] != '-1':
                self.open_pipe(0.5, '2_max_subarray')

                for line in self.commands[:-1]:
                    self.write_pipe(0, line)

            # run for maximum 60 secs or 60 lines
            for _ in range(60):
                try:
                    l = self.p.recvline(timeout=1.2)
                    if l:
                        self.last = l
                except EOFError:
                    break
            if self.last != self.commands[-1].encode():
                raise errors.WA
        except Exception as e:
            self.abort()
            raise e

    def open_pipe(self, timeout, filename):
        sleep(timeout)
        filename = filename.strip()
        try:
            statbuf = os.stat(filename)
        except FileNotFoundError:
            raise errors.TLE
        if not stat_util.S_ISFIFO(statbuf.st_mode):
            raise errors.NotFIFO

        self.pipes.append(open(filename, 'wb+', buffering=0))

    def expect_stdout(self, timeout, line):
        line = line.encode().strip().split(b' ')
        try:
            content = self.p.recvline(timeout=timeout).strip().split(b' ')
        except EOFError:
            raise errors.WA

        if content == [b'']:
            raise errors.TLE

        if len(content) != len(line):
            raise errors.WA

        for c, l in zip(content, line):
            if l == b'*':
                continue
            elif l == b'?':
                if self.last == c:
                    raise errors.WA
                else:
                    self.last = c
            elif l == b'=':
                if c != self.last:
                    raise errors.WA
            elif l == b'/':
                if c in self.hist:
                    raise errors.WA
                else:
                    self.hist.add(c)
            elif c != l:
                raise errors.WA

        # print(b' '.join(content))

    def write_pipe(self, timeout, arg):
        sleep(timeout)
        arg = arg.split(' ')
        pipe_id = int(arg[0])
        content = ' '.join(arg[1:]).encode()
        if self.pipes[pipe_id].write(content) != len(content):
            raise errors.JE

    def send_signal(self, timeout):
        sleep(timeout)
        os.kill(self.p.pid, 20)

    def check_file(self, timeout, filename):
        sleep(timeout)
        filename = filename.strip()
        if os.path.exists(filename):
            raise errors.FileNotRemoved

    def abort(self):
        self.p.kill()
        return self.p.poll(True) >= -1

    def wait_process(self):
        sleep(1.5)
        res = self.p.poll()
        if res is None:
            self.p.kill()
            raise errors.TLE
        elif res != 0:
            raise errors.RE