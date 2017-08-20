class MyBreakpoint (gdb.Breakpoint):
    def stop (self):
        if gdb.selected_frame().older().name()=="b":
            if self.hit_count < 3:
                print("Parent Frame is : ", gdb.selected_frame().older().name())
            else:
                self.enabled = False # Readable / Writable

            print('BreakPoint Enabled : ', self.enabled)
            print('BreakPoint identifier : ', self.number)
            print('BreakPoint hit_count : ', self.hit_count)
            return True
        else:
            return False

# MyBreakpoint("a")

gdb.execute("file stack_conditionnal")

MyBreakpoint("6")

gdb.execute("start")
gdb.execute("continue")
gdb.execute("continue")
#gdb.execute("quit")


# gdb.newest_frame ()
# Frame.name ()
# Frame.architecture ()
# Frame.type ()
  #
