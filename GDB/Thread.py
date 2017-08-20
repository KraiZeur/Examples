import gdb

class MyBreakpoint (gdb.Breakpoint):
    def stop():
        gdb.lookup_type()
        gdb.execute("bt f")
        for thread in gdb.selected_inferior().threads():
            # This is equivalent to 'thread X'
            thread.switch()
            print( "XXXXXXXXXXX --- Thread %s" % thread.num)

            # Just execute a raw gdb command
            gdb.execute('bt')
            framesNames = []
            f = gdb.newest_frame()
            while f is not None:
                framesNames.append(gdb.Frame.name(f))
                f = gdb.Frame.older(f)

        return True

gdb.execute("file Thread")

MyBreakpoint("routine")

gdb.execute("run")

# This loops through all the Thread objects in the process
