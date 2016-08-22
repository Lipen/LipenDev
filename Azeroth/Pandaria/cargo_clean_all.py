import os

path = 'C:/Data/Programming/'


def process(subdir, files):
    if 'Cargo.toml' in files:
        os.chdir(subdir)
        os.system('cargo clean')
        print('[+] Cleaned {}'.format(subdir))


def main():
    for subdir, dirs, files in os.walk(path):
        try:
            process(subdir, files)
        except:
            print('[!] Exception caught on {}/{}'.format(subdir))
            raise

if __name__ == '__main__':
    main()
