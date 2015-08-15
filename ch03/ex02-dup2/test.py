from shell_doctest import test

test(
"""\
% rm -f foo bar
% {make} --no-print-directory clean all
...
% ./mydup2 mydup2.c | sed 's/\x1b\[[0-9;]*m//g'  # sed is for stripping colors.
>> Checking fd#3
current_position: 1651
FD_CLOEXEC: 0
File status: O_RDONLY
...
>> Checking fd#42
current_position: 1651
FD_CLOEXEC: 0
File status: O_RDONLY
...
""", debug=False,
)
