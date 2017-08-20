import gdb

# Type
#   Fields() -> Field ('artificial', 'bitpos', 'bitsize', 'is_base_class', 'name', 'parent_type', 'type')

class Offsets(gdb.Command):
    def __init__(self):
        super (Offsets, self).__init__ ('offsets-of', gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        argv = gdb.string_to_argv(arg)
        if len(argv) != 1:
            raise gdb.GdbError('offsets-of takes exactly 1 argument.')

        stype = gdb.lookup_type(argv[0])

        #print(dir(stype))

        # , stype.__class__.__name__
        print (argv[0], '{')
        for field in stype.fields():
            print ('    %s => %d - %d - %s' % (field.name, field.bitpos//8, field.type.sizeof, field.type)) # // integer division % must mean formmating
            print ('Code : %d' % (field.type.code))
            print ('    %0*d' % (field.type.sizeof, 0))
        print ('}')

        frame = gdb.selected_frame()

        print ('Called from : %s' % (frame.name()))

Offsets()

class CustomBreakPoint(gdb.Breakpoint):
    def stop (self):
        print(gdb.parse_and_eval('a'))
        val = gdb.parse_and_eval('b')
        print('End')
        return False

gdb.execute("file struct")

CustomBreakPoint("14")
gdb.execute("start")
gdb.execute("offsets-of \"struct A\"")
