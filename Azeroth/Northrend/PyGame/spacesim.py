from tkinter import *
from common import *
from Events import *
from EventManager import EventManager
from Game import Game  # from Model import Model
from View import View
from Controller import Controller
from threading import Thread

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 60
_DELAY = round(1000. / _FPS)


def main():
	eventManager = EventManager()

	root = Tk()
	root.title(_TITLE)
	canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
	canv.pack()

	controller = Controller(eventManager)
	view = View(eventManager, canv)
	game = Game(eventManager)

	for c in 'wasdzxuhjknm':
		root.bind('<{}>'.format(c), lambda e, c=c: eventManager.Post(KeyPressedEvent(c)))  # c=c IS IMPORTANT!
	root.bind('<Escape>', lambda e: eventManager.Post(QuitEvent(root)))
	root.bind('<Control-c>', lambda e: eventManager.Post(QuitEvent(root)))

	thread = Thread(target=controller.Run, args=(_FPS,))
	thread.daemon = True
	thread.start()

	print('Going tk.mainloop...')
	root.mainloop()
	eventManager.Post(QuitEvent(None))

main()

# try:
# 	main()
# except Exception as ex:
# 	print('Quit with exception:\n\t{}'.format(ex))
# else:
# 	print('Successfully quit.')
