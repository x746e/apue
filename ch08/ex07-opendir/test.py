from shell_doctest import tests, test


test("""\
% {make} --no-print-directory clean all
...
% ./opendir-close-on-exec
opendir("/"): FD_CLOEXEC is set
open("/"): FD_CLOEXEC is not set
""")
