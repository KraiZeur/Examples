file Inheritance

define printConstructor
	bt
	continue
end

define printDestructor
	echo Destructor called
	continue
end

b Base::Base
commands
	printConstructor
end

b ConcreteA::ConcreteA
commands
	set $this = this
	printConstructor
end

b Base::~Base
commands
	printDestructor
end

b ConcreteA::~ConcreteA
commands
	print $this
	printDestructor
end

b 34
#commands
#	printf "Vtable is"
#	info vtbl base
#	continue
#end

start
continue

#quit