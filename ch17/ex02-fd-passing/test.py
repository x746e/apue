from shell_doctest import tests, test

test(
"""\
% {make} --no-print-directory clean all
...
% ./fd-passing
Parent: got fd: 5, seek: 0
Parent: seek after child changed it: 10 (should be 10)
Parent: seek after child changed it second time: 20 (should be 20)
"""
)
