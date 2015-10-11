__author__ = 'Lipen'

class TestClass:
	'''
	Class example, showing how to write classes
	'''
	x = 0

	def __init__(self, xDefault = 42):
		print('This is init method, called automatically')
		self.x = xDefault

	def addX(self, add):
		print('self.x was {}, plus {}... became '.format(self.x, add), end='')
		self.x += add
		print('{}'.format(self.x))

	def get(self):
		return self.x

def main():
	myClass1 = TestClass()
	myClass2 = TestClass(10)
	myClass3 = TestClass(10000)

	print(TestClass.x)
	TestClass.addX(myClass1, 5)

	print('myClass1.x = {}'.format(myClass1.get()))
	print('myClass2.x = {}'.format(myClass2.get()))
	print('myClass3.x = {}'.format(myClass3.x))

	myClass1.addX(9)
	myClass2.addX(74)
	myClass3.x += 3953

	print('myClass1.x = {}'.format(myClass1.get()))
	print('myClass2.x = {}'.format(myClass2.get()))
	print('myClass3.x = {}'.format(myClass3.x))

if __name__ == '__main__':
	main()
