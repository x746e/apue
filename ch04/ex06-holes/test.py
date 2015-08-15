from shell_doctest import tests

tests([
# check-holes should report no holes in a file.nohole
"""\
% {make} --no-print-directory clean all
...
% ./check-holes file.nohole
""",
# make-file-with-holes should create file with correct number & size of holes.
"""\
% {make} --no-print-directory clean all
...
% ./make-file-with-holes file.hole
% ./check-holes file.hole
Hole from 0 to 131072
Hole from 262144 to 393216
"""
], exclude=('DragonFly',))
