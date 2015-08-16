from __future__ import print_function
import os
import re
import sys
import platform
from itertools import count
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

from colors import C


def _default_debug():
    return '-d' in sys.argv or 'D' in os.environ


def tests(doctests, debug=_default_debug(), exclude=(), only=()):
    for doctest in doctests:
        test(doctest, debug, exclude, only)


def test(doctest, debug=_default_debug(), exclude=(), only=()):
    assert not (exclude and only)
    if only and platform.system() not in only:
        if debug:
            print("Skipping test on %s" % platform.system())
        return
    if platform.system() in exclude:
        if debug:
            print("Skipping test on %s" % platform.system())
        return
    _cd_test_dir()
    if debug:
        print(list(parse(doctest)))
    result = all(check(*pair, debug=debug)
                 for pair in parse(doctest))
    if not result:
        exit(1)


def _cd_test_dir():
    r = re.compile(r'ex\d+.*/test.py$')
    for frame in _get_frames():
        test_path = os.path.abspath(frame.f_code.co_filename)
        if r.search(test_path):
            os.chdir(os.path.dirname(test_path))
            return
    raise AssertionError("Couldn't find test's frame.  Is `_cd_test_dir` run "
                         "from a test?")


def _get_frames():
    for i in count():
        try:
            yield sys._getframe(i)
        except ValueError:
            break

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
    if debug:
        print('=' * 20)

    ellipsis = True
    normaliza_whitespace = True

    actual = check_output(input + '; exit 0', stderr=STDOUT, shell=True)
    if normaliza_whitespace:
        actual = actual.replace('\t', ' ' * 8)

    expected_re = '^%s$' % _escape(expected)
    if ellipsis:
        expected_re = _replace_ellipsis(expected_re)
    if normaliza_whitespace:
        expected_re = re.sub(r' +', r' +', expected_re)

    mismatch = not re.match(expected_re, actual, re.MULTILINE | re.DOTALL)
    if mismatch:
        C.p_red("### Mismatch ###")
    if mismatch or debug:
        C.p_yellow("Ran:")
        print(input.strip())
        C.p_yellow("Actually got:")
        print(actual)
        C.p_yellow("Expected:")
        print(expected)
        if debug:
            C.p_yellow("Expected regex:")
            print(expected_re)
    if mismatch:
        C.p_red("### Mismatch ###")
    return not mismatch


def _escape(s):
    """
    Replace special symbols in s, so that they match their literal meaning in
    regex.
    """
    for sym in '\\.^$*+?{}[]|()':
        s = s.replace(sym, '\\' + sym)
    return s


def _replace_ellipsis(expected):
    """
    Replace `...` by `.*`.

    If `...` is the only contents of the line, it may mean that the line is
    optional.  But if we just replace `...` by `.*`, `\n` before and after `...`
    will not match.  So the function is removing the line with only `...`,
    remove that line and add `.*` to the end of previous one.
    """
    lines = expected.splitlines()
    def itr():
        # `...` will be already escaped.
        ELLIPSIS = _escape('...')
        for line, next_line in zip(lines, lines[1:] + ['']):
            if line == ELLIPSIS:
                continue
            line = line.replace(ELLIPSIS, '.*')
            if next_line == ELLIPSIS:
                line += '.*'
            yield line
    return '\n'.join(itr())
