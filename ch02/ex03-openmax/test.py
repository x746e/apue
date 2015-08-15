from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% ./pr02-17
open_max: 65536
"""
)
