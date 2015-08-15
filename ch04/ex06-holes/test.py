from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% ./check-holes file.nohole
% #-----------------------------------------------#
% ./make-file-with-holes file.hole
% ./check-holes file.hole
Hole from 4096 to 8192
Hole from 12288 to 16384
% #-----------------------------------------------#
% ./make-file-with-holes-apue apue.file.hole
% ./check-holes apue.file.hole
Hole from 4096 to 16384
% #-----------------------------------------------#
% ./copy-holes file.hole file.hole.copy
% ./check-holes file.hole.copy
Hole from 4096 to 8192
Hole from 12288 to 16384
% #-----------------------------------------------#
% rm apue.file.hole file.hole file.hole.copy
"""
)
