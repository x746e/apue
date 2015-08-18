from shell_doctest import tests

tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as normal user.
# uid is `100...` because sometimes it's 1000 (linux), and sometimes 1001 (fbsd).
"""\
% ./id
Process ID: ...
Parent PID: ...
Process group ID: ...
Session ID: ...

Real user ID: 100... (tn)
Effective user ID: 100... (tn)

Real group ID: 1000 (tn)
Effective group ID: 100... (tn)
"""
])
