#!/usr/bin/env python

import subprocess
import sys
import pty
import select
import os
import signal

p = None


def handle_timeout(no, use):
    print('ticktok')
    p.terminate()
    sys.exit(0)


if __name__ == '__main__':
    args = ['./node', 'bin_from_web.js']
    p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    signal.signal(signal.SIGALRM, handle_timeout)
    signal.alarm(10)

    while p.poll() is None:
        r, w, e = select.select([sys.stdin, p.stdout, p.stderr], [], [])
        if sys.stdin in r:
            d = os.read(sys.stdin.fileno(), 1024)
            os.write(p.stdin.fileno(), d)
        elif p.stdout in r:
            o = os.read(p.stdout.fileno(), 1024)
            if o:
                os.write(sys.stdout.fileno(), o)
        elif p.stderr in r:
            o = os.read(p.stderr.fileno(), 1024)
            if o:
                os.write(sys.stdout.fileno(), o)
    
    p.terminate()

