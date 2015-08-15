from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% ./filetype ../*
../4.3: regular
../4.7: regular
../broken: stat error: No such file or directory
../dir3: directory
../ex01-filetype: directory
../ex04-umask: directory
../ex06-holes: directory
../ex09: directory
../ex11-ftw: directory
../ex16-deep-tree: directory
../readdir: directory
../root: directory
""", debug=False,
)
