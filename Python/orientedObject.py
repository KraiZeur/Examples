class Point:
	def __init__(self, x=0, y=0):
		self.move(x, y)

	def move(self, x, y):
		self.x = x
		self.y = y

	def reset(self):
		self.move(0)

class Contact:
	all_contacts=[]

	def __init__(self, name, email):
		self.name=name
		self.email=email
		Contact.all_contacts.append(self)

class Supplier(Contact):
	def __init__(self, name, email):
		super().__init__(name, email)
		
	def order(self, order):
		print("order")


point = Point(3, 5)
print(point.x, point.y)