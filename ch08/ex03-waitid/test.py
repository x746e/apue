from shell_doctest import tests, test


test("""\
% {make} --no-print-directory clean all
...
% ./wait1
normal termination, exit status = 7
abnormal termination, signal number = 6
abnormal termination, signal number = 8
""")
