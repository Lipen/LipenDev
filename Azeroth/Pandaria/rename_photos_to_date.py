from PIL import Image, ExifTags
import os
import re

path = 'C:/Data/MEGA/Photos'
pattern_okfile = r'^\d{4}-\d\d-\d\d \d\d-\d\d-\d\d\.jpe?g$'
pattern_filematch = r'^(?P<basename>.*)\.(?P<ext>.*)'
pattern_datematch = r'.*?(?P<year>\d{4})[:/\. -] *(?P<month>\d{1,2})[:/\. -] *(?P<day>\d{1,2})[:/\. -] *(?P<hours>\d{1,2})[:/\. -] *(?P<minutes>\d{1,2})[:/\. -] *(?P<seconds>\d{1,2}).*?'
format_newname = '{year:04d}-{month:02d}-{day:02d} {hours:02d}-{minutes:02d}-{seconds:02d}.{ext}'


def process(subdir, file):
    filepath = os.path.join(subdir, file)
    filematch = re.match(pattern_filematch, file)

    if re.match(pattern_okfile, file):
        print('[*] OK: {}'.format(file))
        pass
    elif filematch and filematch.group('ext').lower() in ['jpg', 'jpeg']:
        with Image.open(filepath) as img:
            exif_ = img._getexif()

        if exif_:
            exif = {ExifTags.TAGS[k]: v for k, v in exif_.items() if k in ExifTags.TAGS}

            try:
                date = exif['DateTimeOriginal']
            except KeyError:
                date = filematch.group('basename')
                print('[*] No DateTimeOriginal tag for {}. Trying to accomplish from file name...'.format(filepath))

            rename_photo(subdir, file, filematch, date)
        else:
            print('[*] Skipped {} without valid exif tags'.format(filepath))


def rename_photo(subdir, file, filematch, date):
    filepath = os.path.join(subdir, file)
    datematch = re.match(pattern_datematch, date)

    if datematch:
        newname = format_newname.format(**{k: int(v) for k, v in datematch.groupdict().items()}, ext=filematch.group('ext').lower())
        newpath = os.path.join(subdir, newname)

        # print('[*] Renaming \'{}\' to \'{}\''.format(file, newname, subdir))
        try:
            os.rename(filepath, newpath)
            pass
        except FileExistsError:
            print('[-] Destination file already exists ({} -> {})'.format(filepath, newname))
        else:
            print('[+] Renamed {} -> {}'.format(filepath, newname))
            pass
    else:
        print('[-] Couldn`t match date for {}'.format(filepath))


def main():
    for subdir, dirs, files in os.walk(path):
        for file in files:
            try:
                process(subdir, file)
            except:
                print('[!] Exception caught on {}/{}'.format(subdir, file))
                raise

if __name__ == '__main__':
    main()
