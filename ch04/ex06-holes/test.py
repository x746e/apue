from shell_doctest import tests, test

tests([
# check-holes should report no holes in a file.nohole
"""\
% {make} --no-print-directory clean all
...
% ./check-holes file.nohole
""",
# make-file-with-holes should create file with correct number & size of holes.
"""\
% ./make-file-with-holes file.hole
% ./check-holes file.hole
Hole from 0 to 131072
Hole from 262144 to 393216
""",
# Check that ./copy-holes preserve holes.
"""\
% ./copy-holes file.hole file.hole.copy
% ./check-holes file.hole.copy
Hole from 0 to 131072
Hole from 262144 to 393216
% rm file.hole.copy
"""
], exclude=('DragonFly',) # At least when using HAMMER fs.
)

test(
# Check that cp doesn't preserve holes.
"""\
% cp file.hole file.hole.cp_copy
% ./check-holes file.hole.cp_copy
% rm file.hole.cp_copy
""", exclude=('Linux', 'DragonFly')  # cp on linux actually does.
)
