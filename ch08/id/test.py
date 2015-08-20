from shell_doctest import tests

tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as user "daemon".
# uid is `10...` because sometimes it's 1000 (linux), sometimes 1001 (fbsd),
# sometimes 101 (sunos).
"""\
% sudo ./check_setuid 1
Calling setuid(1)
Process ID: ...
Parent PID: ...
Process group ID: ...
Session ID: ...

Real user ID: 1 (daemon)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
...
"""
])
