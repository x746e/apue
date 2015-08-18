from shell_doctest import tests

tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as normal user.
"""\
% ./id
Process ID: ...
Parent PID: ...
Process group ID: ...
Session ID: ...

Real user ID: 1000 (tn)
Effective user ID: 1000 (tn)

Real group ID: 1000 (tn)
Effective group ID: 1000 (tn)
"""
])
