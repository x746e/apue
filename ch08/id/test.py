from shell_doctest import tests

tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as normal user.
# uid is `10...` because sometimes it's 1000 (linux), sometimes 1001 (fbsd),
# sometimes 101 (sunos).
"""\
% ./id
Process ID: ...
Parent PID: ...
Process group ID: ...
Session ID: ...

Real user ID: 10... (tn)
Effective user ID: 10... (tn)

Real group ID: 10... (...)
Effective group ID: 10... (...)
"""
])
