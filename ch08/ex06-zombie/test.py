from shell_doctest import tests, test


test("""\
% {make} --no-print-directory clean all
...
% ./zombie
child pid: ...
Child status (first letter should be `Z'): Z...

""")
