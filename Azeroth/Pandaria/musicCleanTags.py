import os
import sys
import re
from multiprocessing import Pool
from mutagen.easyid3 import EasyID3

library = "C:/Data/Media/Music/VK"
rex = re.compile(r'^(.*) - (.*)$')


class cd:
    """Context manager for changing the current working directory"""

    def __init__(self, new_path):
        self.new_path = os.path.expanduser(new_path)

    def __enter__(self):
        self.saved_path = os.getcwd()
        os.chdir(self.new_path)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.saved_path)


def work(path):
    folder, filename = os.path.split(path)
    name, ext = os.path.splitext(filename)
    # print('[{} / {}] {}'.format(files.index(path) + 1, len(files), filename))

    if ext == '.mp3':
        m = rex.match(name)

        if m:
            try:
                audio = EasyID3(path)
            except:
                audio = EasyID3()

            artist, title = m.groups()
            album = audio.get('album', artist)

            audio.delete(path)

            audio['artist'] = artist
            audio['title'] = title
            audio['album'] = album

            audio.save(path)
            print('[+] Tags cleaned for \'{} - {}\''.format(artist, title))
        else:
            print('[-] File doesn`t match pattern: \'{}\''.format(path))
    else:
        print('[-] File is not a mp3: \'{}\''.format(path))


if __name__ == '__main__':
    if len(sys.argv) > 1:
        START = int(sys.argv[1])
    else:
        START = 0

    files = []
    dirs = [library]

    while dirs:
        with cd(dirs.pop()):
            for entry in os.listdir():
                if os.path.isdir(entry):
                    dirs.append(os.path.abspath(entry))
                else:
                    files.append(os.path.abspath(entry))

    files.sort()
    print('Total {} files'.format(len(files)))

    pool = Pool(8)
    res = pool.map_async(work, files[START:])
    pool.close()
    pool.join()

    # while True:
    #     if res.ready():
    #         break

    #     remaining = res._number_left
    #     print('{} files left...'.format(remaining))
    #     time.sleep(1)
