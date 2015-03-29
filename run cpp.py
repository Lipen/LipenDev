import os
sys = os.system


def main():
    print('List of existing avaliable files:')
    files = []
    counter = 0
    for file in os.listdir():
        if file[-4:] == '.cpp':
            counter += 1
            files.append(file[:-4])
            print('{:->3d}) {}'.format(counter, file[:-4]))

    name = ''
    while len(name) == 0:
        name = input('Enter file <name> or <number> or #<number>: ')
    try:
        name = files[int(name)-1]
    except:
        if name[0] == '#':
            try:
                fileid = int(name[1:])
                name = files[fileid - 1]
            except:
                pass

    err = sys('g++ {0}.cpp -std=c++0x -o {0}.exe'.format(name))
    if err:
        print('Error during compiling. <{}>'.format(err))
        input('\nType enter to exit: ')
    else:
        print('Compiled succesfully. Starting:\n'+'-'*30)
        err2 = sys('{}.exe'.format(name))
        if err2:
            input('-'*30+'\nError during execution. <{}>'.format(err2))
        else:
            input('-'*30+'\nDone.\nType enter to exit: ')

main()
