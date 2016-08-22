import re
import os

path = 'C:/Data/Screenshots/old'
# path = 'G:/Screenshots/old'
os.chdir(path)

files = [f for f in os.listdir(path) if os.path.isfile(f)]
impossible = []

print('Processing total {} files...'.format(len(files)))

for file in files:
    # 17-05-2015 21-17-15
    m = re.match(r'^(?P<day>\d\d)-(?P<month>\d\d)-(?P<year>\d{4}) (?P<hours>\d\d)-(?P<minutes>\d\d)-(?P<seconds>\d\d)\.(?P<ext>.*)$', file)

    if not m:
        # 2014.11.20-00.21.18
        m = re.match(r'^(?P<year>\d{4})\.(?P<month>\d\d)\.(?P<day>\d\d)-(?P<hours>\d\d)\.(?P<minutes>\d\d)\.(?P<seconds>\d\d)\.(?P<ext>.*)$', file)

    if not m:
        # 21.01.2014-14.04.13
        m = re.match(r'^(?P<day>\d\d)\.(?P<month>\d\d)\.(?P<year>\d{4})-(?P<hours>\d\d)\.(?P<minutes>\d\d)\.(?P<seconds>\d\d)\.(?P<ext>.*)$', file)

    if not m:
        # 2014-05-10_17-31-47
        m = re.match(r'^(?P<year>\d{4})-(?P<month>\d\d)-(?P<day>\d\d)_(?P<hours>\d\d)-(?P<minutes>\d\d)-(?P<seconds>\d\d)\.(?P<ext>.*)$', file)

    if m:
        newname = '{year}-{month}-{day} {hours}-{minutes}-{seconds}.{ext}'.format_map(m.groupdict())
        print('[+] Renaming \'{}\' to \'{}\''.format(file, newname))
        try:
            os.rename(file, newname)
        except FileExistsError:
            print('\t[-] Destination file already exists')
    else:
        if re.match(r'^(?P<year>\d{4})-(?P<month>\d\d)-(?P<day>\d\d) (?P<hours>\d\d)-(?P<minutes>\d\d)-(?P<seconds>\d\d)\.(?P<ext>.*)$', file):
            # Already OK filename
            pass
        else:
            impossible.append(file)

if impossible:
    print('\n[!] Impossible to rename {} files:'.format(len(impossible)))
    for file in impossible:
        print('\t\'{}\''.format(file))

print('All done')
