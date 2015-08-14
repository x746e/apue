#!/usr/bin/env python
from __future__ import print_function
import os
import re
import sys
from pprint import pprint
from subprocess import Popen, check_output, STDOUT, check_call


def test(doctest, debug=False):
    # chdir into test's directory
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

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


def check(input, expected, debug=False):
    """
    Execute input, check it matches expected.
    """
    def dprint(*args):
        if debug:
            print(*args)
    dprint('=' * 20)

    ellipsis = True
    normaliza_whitespace = True

    dprint("Going to run '''%s'''" % input)
    actual = check_output(input + '; exit 0', stderr=STDOUT, shell=True)
    dprint("Actually got '''%s'''" % actual)

    # TODO: Escape special symbols.
    expected = '^%s$' % expected
    if ellipsis:
        expected = expected.replace('...', '.*')
    if normaliza_whitespace:
        expected = re.sub(r' +', r' +', expected)

    dprint("Expected: '''%s'''" % expected.strip())
    assert re.search(expected, actual, re.MULTILINE), \
            "%r didn't match %r" % (actual, expected)

    dprint(">> Success!")


test(
"""\
% rm -f foo bar
% {make} --no-print-directory clean all
...
% ./test-umask
% ls -l foo bar
-rw------- 1 tn tn 0 ... bar
-rw-rw-rw- 1 tn tn 0 ... foo
""", debug=False,
)
