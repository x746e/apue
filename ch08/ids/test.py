from shell_doctest import tests

# Setuid on sunos and linux
tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as user "daemon".
"""\
% sudo ./check_setuid 1
Real user ID: 0 (root)
Effective user ID: 0 (root)
Saved user ID: 0 (root)

Calling setuid(1)

Real user ID: 1 (daemon)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
""",
# Make file setuid daemon
"""\
% sudo chown daemon check_setuid
% sudo chmod u+s check_setuid
""",
# Now setuid to daemon shoudn't change anything
"""\
% ./check_setuid 1
Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)

Calling setuid(1)

Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
""",
# And setuid to $UID should leave saved suid as daemon
"""\
% ./check_setuid $(id -ur)
Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)

Calling setuid(10...)

Real user ID: 10... (tn)
Effective user ID: 10... (tn)
Saved user ID: 1 (daemon)
...
"""
], only=("SunOS", "Linux"))


# Setuid on freebsd and dragonflybsd
tests([
# Rebuild.
"""\
% {make} --no-print-directory clean all
...
""",
# Run as user "daemon".
"""\
% sudo ./check_setuid 1
Real user ID: 0 (root)
Effective user ID: 0 (root)
Saved user ID: 0 (root)

Calling setuid(1)

Real user ID: 1 (daemon)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
""",
# Make file setuid daemon
"""\
% sudo chown daemon check_setuid
% sudo chmod u+s check_setuid
""",
# Now setuid to daemon should set all ids to 'daemon'
"""\
% ./check_setuid 1
Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)

Calling setuid(1)

Real user ID: 1 (daemon)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)
""",
# And setuid to $UID should set all ids to $UID
"""\
% ./check_setuid $(id -ur)
Real user ID: 10... (tn)
Effective user ID: 1 (daemon)
Saved user ID: 1 (daemon)

Calling setuid(10...)

Real user ID: 10... (tn)
Effective user ID: 10... (tn)
Saved user ID: 10... (tn)
...
"""
], only=("FreeBSD", "DragonFly"))