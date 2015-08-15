from shell_doctest import tests, test

test(
"""\
% {make} -s -C .. clean
% {make} --no-print-directory clean all
...
% ./ftw ..
regular files  =      37, 75.51 %
directories    =       9, 18.37 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =       3,  6.12 %
sockets        =       0,  0.00 %
% ./apue-ftw ..
regular files  =      37, 75.51 %
directories    =       9, 18.37 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =       3,  6.12 %
sockets        =       0,  0.00 %
"""
)
