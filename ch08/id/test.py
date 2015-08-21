from shell_doctest import tests

tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as user "daemon".
"""\
% sudo ./check_setuid 1
Calling setuid(1)
...

Real user ID: 1 (daemon)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
...
""",
# Make file setuid daemon
"""\
% sudo chown daemon check_setuid
% sudo chmod u+s check_setuid
""",
# Now setuid to daemon shoudn't change anything
"""\
% ./check_setuid 1
Calling setuid(1)
...

Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
...
""",
# And setuid to $UID should leave saved suid as daemon
"""\
% ./check_setuid $(id -ur)
Calling setuid(10...)
...

Real user ID: 10... (tn)
Effective user ID: 10... (tn)
Saved user ID: 1 (daemon)
...
"""
])
