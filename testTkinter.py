from tkinter import *

title = 'testTkinter'
w=800
h=600

root = Tk()
root.title(title)
canv = Canvas(root, width=w, height=h, bg='#ffffff')

##

canv.pack
root.mainloop()
