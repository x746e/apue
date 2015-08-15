from shell_doctest import test

test(
"""\
% rm -f foo bar
% {make} --no-print-directory clean all
...
% ./test-umask
% pwd
% ls -l foo bar
-rw------- 1 tn ... bar
-rw-rw-rw- 1 tn ... foo
""", debug=False,
)
