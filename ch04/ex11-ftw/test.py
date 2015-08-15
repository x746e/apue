from shell_doctest import tests, test

test(
"""\
% {make} --no-print-directory clean all
...
% rm -f output.ftw output.apue-ftw
% touch output.ftw output.apue-ftw
% sudo ./ftw / >| output.ftw
% sudo ./apue-ftw / >| output.apue-ftw
% # Sometimes outputs differ the first time the test run.
% diff output.ftw output.apue-ftw
% rm output.ftw output.apue-ftw
"""
)
