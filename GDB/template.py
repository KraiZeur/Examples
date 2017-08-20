gdb.execute('file template', to_string=True)
gdb.execute('break smart_pointer<int>::get()', to_string=True)
gdb.execute('run', to_string=True)

frame = gdb.selected_frame()
block = frame.block()
names = set()
while block:
    if(block.is_global):
        print()
        print('global vars')
    for symbol in block:
        if (symbol.is_argument or symbol.is_variable):
            name = symbol.name
            if not name in names:
                print('{} = {}'.format(name, symbol.value(frame)))
                names.add(name)
    block = block.superblock
