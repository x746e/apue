from shell_doctest import tests, test


test("""\
% {make} --no-print-directory clean all
...
% ./zombie
child pid: ...
Child status (first letter should be `Z'): Z...
""",
# `ps' seems to be broken on DragonFly, ps -p doesn't select zombie processes.
# On FreeBSD it semi-broken -- it selects zombie processes if you pass `-a'
# flag.
exclude=('DragonFly',))
