#!/usr/bin/env python
from __future__ import print_function
import os
import re
import sys
from pprint import pprint
from subprocess import Popen, check_output, STDOUT, check_call


def test(doctest, debug=False):
    # chdir into test's directory
    os.chdir(os.path.dirname(__file__))

    if debug:
        print(list(parse(doctest)))
    for pair in parse(doctest):
        check(*pair, debug=debug)


def parse(doctest):
    """
    Parse doctest into input/output pairs.
    """
    if sys.platform.startswith('linux'):
        doctest = doctest.format(make='make')
    else:
        doctest = doctest.format(make='gmake')
    in_output = False
    input = output = None
    for line in doctest.splitlines():
        if line.startswith('%'):
            # End previous pair, start next one
            if input:
                yield (input, '\n'.join(output))
            input = line.lstrip('% ')
            output = []
        else:
            output.append(line)
    # Last pair
    yield (input, '\n'.join(output))


def check(input, output, debug=False):
    """
    Execute input, check it matches output.
    """
    def dprint(*args):
        if debug:
            print(*args)
    dprint('=' * 20)
    ellipsis = True
    dprint(">> Going to run %s" % input)
    actual = check_output(input + '; exit 0', stderr=STDOUT, shell=True)
    dprint(">> Actually got %s" % actual)
    if ellipsis:
        output = '^' + output.replace('...', '.*', re.MULTILINE) + '$'
        dprint('Input:  ' + input)
        dprint('Output: ' + output)
        dprint('Actual: ' + actual)
        assert re.search(output, actual)
    else:
        assert output == actual
    dprint(">> Success!")


test(
"""\
% rm -f foo bar
% {make} clean all
...
...
% ./test-umask
% ls -l foo bar
-rw------- 1 tn tn 0 ... bar
-rw-rw-rw- 1 tn tn 0 ... foo
"""
)
