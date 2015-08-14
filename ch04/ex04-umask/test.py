#!/usr/bin/env python
from __future__ import print_function
import os
import re
import sys
from pprint import pprint

from subprocess import STDOUT
try:
    from subprocess import check_output
except ImportError:
    from subprocess import Popen, CalledProcessError, PIPE
    # Copied from python2.7 stdlib
    def check_output(*popenargs, **kwargs):
        r"""Run command with arguments and return its output as a byte string.

        If the exit code was non-zero it raises a CalledProcessError.  The
        CalledProcessError object will have the return code in the returncode
        attribute and output in the output attribute.

        The arguments are the same as for the Popen constructor.  Example:

        >>> check_output(["ls", "-l", "/dev/null"])
        'crw-rw-rw- 1 root root 1, 3 Oct 18  2007 /dev/null\n'

        The stdout argument is not allowed as it is used internally.
        To capture standard error in the result, use stderr=STDOUT.

        >>> check_output(["/bin/sh", "-c",
        ...               "ls -l non_existent_file ; exit 0"],
        ...              stderr=STDOUT)
        'ls: non_existent_file: No such file or directory\n'
        """
        if 'stdout' in kwargs:
            raise ValueError('stdout argument not allowed, it will be overridden.')
        process = Popen(stdout=PIPE, *popenargs, **kwargs)
        output, unused_err = process.communicate()
        retcode = process.poll()
        if retcode:
            cmd = kwargs.get("args")
            if cmd is None:
                cmd = popenargs[0]
            raise CalledProcessError(retcode, cmd, output=output)
        return output


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
-rw------- 1 tn ... bar
-rw-rw-rw- 1 tn ... foo
""", debug=False,
)
