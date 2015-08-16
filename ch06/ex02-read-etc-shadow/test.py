from shell_doctest import test

test(
"""\
% {make} --no-print-directory clean all
...
% sudo ./print_enc_pass
                root    $...
...
                  tn    $...
... entries overall in /etc/shadow
""",
# Only some systems have spwd APIs.
only=('SunOS', 'Linux')
)
