import re
from pprint import pprint
from subprocess import check_output



def get_macroses(includes=['signal.h']):
    macroses = {}
    include_section = '\n'.join(
        '#include <%s>' % inc for inc in includes
    )
    for line in check_output("echo '%s' | cc -dM -E -" % include_section,
                             shell=True).splitlines():
        m = re.search(r'^#define (?P<name>\w+) (?P<value>.*)$', line)
        if m:
            macroses[m.group('name')] = m.group('value')
    return macroses




def make_sys_signame(macroses):
    NSIG = int(macroses['_NSIG'])


    sys_signame = [None] * NSIG


    for name, value in macroses.iteritems():
        name_match = re.search(r'^SIG([A-Z0-9]+)$', name)
        if not name_match:
            continue

        sig_name = name_match.group(1)

        try:
            sigvalue = int(value)
        except ValueError:
            # Can be alias to other signal.
            # For SIGRTMIN / SIGRTMAX it's a call somewhere into libc.
            # We ignoring are ignoring both cases.
            continue
        else:
            # It can be large integer > NSIG (SIGSTKSZ, 8192), not a signal,
            # ignoring.
            if sigvalue > NSIG:
                continue
            sys_signame[sigvalue] = sig_name

    return sys_signame


def render_sys_signame(sys_signame):
    return (
        'const char * const sys_signame[] = {' +
        ", ".join(
            'NULL' if name is None else '"%s"' % name
            for name in sys_signame
        ) +
        "};"
    )



def main():
    macroses = get_macroses()
    sys_signame = make_sys_signame(macroses)
    print render_sys_signame(sys_signame)


if __name__ == '__main__':
    main()
