import os
sys = os.system

def main():
        isExitWait = True
        
        print('List of existing avaliable files:')
        for file in os.listdir():
                print('{:>20}'.format(file))
                
        s = input()
        if s[:4]=='open':
                if s[-2:] == '/e':
                        isExitWait = False
                        s = s[:-2]
                sys('start ' + s[5:])

        if isExitWait:        
                input('-'*30+'\nDone.\nType enter to exit: ')

main()
