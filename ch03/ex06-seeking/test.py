from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% ./seeking
File contents (26 bytes): abcdefghijklmnopqrstuvwxyz
current_position: 0
Setting position in the middle of the file
current_position: 10
Writing 11 symbols
current_position: 37
File contents (37 bytes): abcdefghijklmnopqrstuvwxyzhello there
current_position: 37
"""
)
