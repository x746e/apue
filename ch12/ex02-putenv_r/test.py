from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% ./putenv_r
HELLO=world
"""
)
