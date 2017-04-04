import os
import sys
import argparse
import datetime
from subprocess import Popen, PIPE, STDOUT
from collections.abc import Mapping
from abc import abstractmethod

DATE = '{:%Y-%m-%d}'.format(datetime.datetime.now())
LOCAL_BACKUP_DIR = 'C:/Data/OneDrive/BACKUPS'
HDD_DISK = 'G'  # just letter


class DataItem(Mapping):

    def __init__(self, name, *, src, dest, cmd=''):
        # Wrap src (if it is a single str) with ""
        if isinstance(src, str):
            src = '"{}"'.format(src.strip())
        # (if not) Wrap every item of src with "" and join them by space
        else:
            src = ' '.join(map(lambda x: '"{}"'.format(x), src))

        # Wrap dest with ""
        dest = '"{}"'.format(dest)

        self.name = name
        self.source = src
        self.destination = dest
        self.cmd = cmd

    def __iter__(self):
        return self.name, self.src, self.dest, self.cmd

    def __getitem__(self, key):
        return self.__dict__[key]

    def __len__(self):
        return len(self.__dict__)

    def __missing__(self, key):
        return '<...>'

    @abstractmethod
    def backup(self):
        command = self.cmd.format_map(self)

        proc = Popen(command, shell=True, stdout=PIPE, stderr=STDOUT)

        return proc


class LocalDataItem(DataItem):

    def __init__(self, name, *, src, dest, cmd=None, flags=None, extra=''):
        # Default cmd:
        if cmd is None:
            cmd = '7za {flags} {destination} {source} {extraflags} | findstr /R /I /V /C:"^[\s]" /C:"^$" /C:"^Compressing" /C:"^7-Zip"'

        # Default flags:
        if flags is None:
            flags = 'a -t7z -mx9 -ssw -uq0'

        self.flags = flags
        self.extraflags = extra

        super().__init__(name, src=src, dest=dest, cmd=cmd)

    def __str__(self):
        return '{name} {{\n   from: {source}\n     to: {destination}\n  flags: {flags}'.format_map(self) + ('\n  extra: {extraflags}'.format_map(self) if self.extraflags else '') + '\n    cmd: {cmd}'.format_map(self) + '\ncommand: {}'.format(self.cmd.format_map(self)) + '\n}'

    def __repr__(self):
        return 'LocalDataItem <{name}> {{from=\'{source}\', dest=\'{destination}\', cmd={cmd}, flags={flags}, extraflags={extraflags}}'.format_map(self)

    def backup(self):
        proc = super().backup()

        for line in proc.stdout:
            print('    ' + line.decode(errors='replace'), end='')

        proc.wait()


class HDDDataItem(DataItem):

    def __init__(self, name, *, src, dest, cmd=None, files='*.*', flags=None, default_flags=True):
        # If disk is already specified it does nothing, everything ok
        dest = os.path.join(HDD_DISK + ':/', dest)

        # Default cmd
        if cmd is None:
            cmd = 'robocopy {source} {destination} {files} {flags}'

        # Default flags
        if flags is None:
            flags = '/E'

        if default_flags:
            flags += ' /ZB /R:1 /W:1 '

        self.files = files
        self.flags = flags

        super().__init__(name, src=src, dest=dest, cmd=cmd)

    def __str__(self):
        return '{name} {{\n   from: {source}\n     to: {destination}\n  flags: {flags}'.format_map(self) + '\n    cmd: {cmd}'.format_map(self) + '\ncommand: {}'.format(self.cmd.format_map(self)) + '\n}'

    def __repr__(self):
        return 'HDDDataItem <{name}> {{from=\'{source}\', dest=\'{destination}\', cmd={cmd}, flags={flags}}'.format_map(self)

    def backup(self):
        proc = super().backup()

        for line in proc.stdout:
            print(line.decode(errors='replace'), end='')

        proc.wait()


def backup_local(args):
    Item = LocalDataItem  # alias

    data = [
        Item('Sublime Text Installed Packages',
             src='C:/Users/Lipen/AppData/Roaming/Sublime Text 3/Installed Packages/*',
             dest='Sublime Text/Installed Packages/Installed Packages {}'.format(DATE)),
        Item('Sublime Text Local',
             src='C:/Users/Lipen/AppData/Roaming/Sublime Text 3/Local/*',
             dest='Sublime Text/Local/Local {}'.format(DATE)),
        Item('Sublime Text Packages',
             src='C:/Users/Lipen/AppData/Roaming/Sublime Text 3/Packages/*',
             dest='Sublime Text/Packages/Packages {}'.format(DATE)),
        Item('dev',
             src='C:/Data/Programming/dev/*',
             dest='dev/dev {}'.format(DATE)),
        Item('Programming',
             src='C:/Data/Programming/*',
             dest='Programming/Programming {}'.format(DATE),
             extra='-xr"!"dev'),
        Item('AIMP',
             src='C:/Users/Lipen/AppData/Roaming/AIMP/*',
             dest='AIMP/AIMP {}'.format(DATE)),
    ]

    if not args.dryrun:
        if args.verbose:
            print('Today is {}\n  Backing up {} items to \'{}\''.format(DATE, len(data), args.dest) + '\n' + ' '.join('-' * 40))
        elif not args.quiet:
            print('{} :: Backing up {} items to \'{}\''.format(DATE, len(data), args.dest))

        os.chdir(args.dest)

        for item in data:
            if args.verbose:
                print('> Backing up {}'.format(item))
            elif not args.quiet:
                print('> Backing up {}...'.format(item.name))
            item.backup()

        if args.verbose:
            print('=================\nLocal backup done.')
        elif not args.quiet:
            print('Local backup done.')
    else:
        if args.verbose:
            print('Today is {}\n  Starting dry-run on backup to \'{}\''.format(DATE, args.dest) + '\n' + ' '.join('-' * 40))
        elif not args.quiet:
            print('{} :: Dry-run on backup to \'{}\''.format(DATE, args.dest))

        for item in data:
            if args.verbose:
                print('> Dry backing up {}'.format(item))
            elif not args.quiet:
                print('> Dry backing up {}'.format(item.name))

        if args.verbose:
            print('= = = = = = = = = = = = =\nLocal dry-run backup done.')
        elif not args.quiet:
            print('Local dry-run backup done.')


def backup_hdd(args):
    global HDD_DISK

    # Before data list!
    if args.disk:
        HDD_DISK = args.disk

    Item = HDDDataItem  # alias

    data = [
        Item('Music',
             src='C:/Data/Media/Music',
             dest='Media/Music',
             flags='/MIR /NDL /MT'),
        Item('Photos',
             src='C:/Data/MEGA/Photos',
             dest='Photos',
             flags='/E /NDL /MT'),
        Item('OneDrive',
             src='C:/Data/OneDrive',
             dest='OneDrive',
             flags='/MIR /XA:SHT /NDL /MT'),
        Item('Dropbox',
             src='C:/Data/Dropbox',
             dest='Dropbox',
             flags='/MIR /XA:SHT /NDL /MT'),
        Item('Screenshots',
             src='C:/Data/Misc/Screenshots',
             dest='Misc/Screenshots'),
        Item('Shortcuts',
             src='C:/Data/Misc/Shortcuts',
             dest='Misc/Shortcuts',
             flags='/MIR'),
        Item('Warcraft III',
             src='C:/Data/Games/Warcraft III',
             dest='Games/Warcraft III',
             flags='/MIR /NDL')
    ]

    if not args.dryrun:
        if args.verbose:
            print('Today is {}\n  Backing up {} items to disk {}'.format(DATE, len(data), HDD_DISK) + '\n' + ' '.join('-' * 40))
        elif not args.quiet:
            print('{} :: Backing up {} items to disk {}'.format(DATE, len(data), HDD_DISK))

        for item in data:
            if args.verbose:
                print('> Backing up {}'.format(item))
            elif not args.quiet:
                print('> Backing up {}...'.format(item.name))
            item.backup()

        if args.verbose:
            print('=================\nHDD backup done.')
        elif not args.quiet:
            print('HDD backup done.')
    else:
        if args.verbose:
            print('Today is {}\n  Starting dry-run on backup to disk {}'.format(DATE, HDD_DISK) + '\n' + ' '.join('-' * 40))
        elif not args.quiet:
            print('{} :: Dry-run on backup to disk {}'.format(DATE, HDD_DISK))

        for item in data:
            if args.verbose:
                print('> Dry backing up {}'.format(item))
            elif not args.quiet:
                print('> Dry backing up {}'.format(item.name))

        if args.verbose:
            print('= = = = = = = = = = = = =\nHDD dry-run backup done.')
        elif not args.quiet:
            print('HDD dry-run backup done.')


def set_default_subparser(self, name, args=None):
    """default subparser selection. Call after setup, just before parse_args()
    name: is the name of the subparser to call by default
    args: if set is the argument list handed to parse_args()
    """
    subparser_found = False
    for arg in sys.argv[1:]:
        if arg in ['-h', '--help']:  # global help if no subparser
            break
    else:
        for x in self._subparsers._actions:
            if not isinstance(x, argparse._SubParsersAction):
                continue
            for sp_name in x._name_parser_map.keys():
                if sp_name in sys.argv[1:]:
                    subparser_found = True
        if not subparser_found:
            # insert default in first position, this implies no
            # global options without a sub_parsers specified
            if args is None:
                sys.argv.insert(1, name)
            else:
                args.insert(0, name)

argparse.ArgumentParser.set_default_subparser = set_default_subparser


def parse_args():
    parser = argparse.ArgumentParser(description='Backup tool')
    subparsers = parser.add_subparsers(title='available methods')

    parser_local = subparsers.add_parser('local', description='Backup local files', help='backup local files')
    parser_local.set_defaults(func=backup_local)
    parser_local.add_argument('-o', '--destination', dest='dest', default=LOCAL_BACKUP_DIR, help='folder to backup to')
    parser_local.add_argument('-n', '--dry-run', dest='dryrun', action='store_true', help='do not backup anything actually')
    verbose_quiet_local = parser_local.add_mutually_exclusive_group()
    verbose_quiet_local.add_argument('-v', '--verbose', action='store_true', help='be verbose')
    verbose_quiet_local.add_argument('-q', '--quiet', action='store_true', help='be quite')

    parser_hdd = subparsers.add_parser('hdd', description='Backup files to HDD', help='backup files to HDD')
    parser_hdd.set_defaults(func=backup_hdd)
    parser_hdd.add_argument('-d', '--disk', dest='disk', help='folder to backup to')
    parser_hdd.add_argument('-n', '--dry-run', dest='dryrun', action='store_true', help='do not backup anything actually')
    verbose_quiet_hdd = parser_hdd.add_mutually_exclusive_group()
    verbose_quiet_hdd.add_argument('-v', '--verbose', action='store_true', help='be verbose')
    verbose_quiet_hdd.add_argument('-q', '--quiet', action='store_true', help='be quite')

    parser.set_default_subparser('local')

    return parser.parse_args()


def main():
    args = parse_args()
    args.func(args)

if __name__ == '__main__':
    main()
