class MyBreakpoint (gdb.Breakpoint):
    def stop():
        gdb.lookup_type()
        gdb.execute("bt f")
        return False # Do not stop execution


gdb.execute("file inheritance")

MyBreakpoint("Base::Base")
MyBreakpoint("ConcreteA::ConcreteA")
MyBreakpoint("Base::~Base")
MyBreakpoint("ConcreteA::~ConcreteA")

gdb.execute("start")

################################################################################
import gdb

# This loops through all the Thread objects in the process
for thread in gdb.selected_inferior().threads():

    # This is equivalent to 'thread X'
    thread.switch()

    print "Thread %s" % thread.num

    # Just execute a raw gdb command
    gdb.execute('bt')

    framesNames = []
    f = gdb.newest_frame()
    while f is not None:
        framesNames.append(gdb.Frame.name(f))
        f = gdb.Frame.older(f)

    # do something with the name of each frame
################################################################################
import sys
import gdb
import os

def logExecCapture(instr):
  # /dev/shm - save file in RAM
  ltxname="/dev/shm/c.log"

  gdb.execute("set logging file "+ltxname) # lpfname
  gdb.execute("set logging redirect on") # By default, gdb output will go to both the terminal and the logfile. Set redirect if you want output to go only to the log file.
  gdb.execute("set logging overwrite on")
  gdb.execute("set logging on")
  gdb.execute(instr)
  gdb.execute("set logging off")

  replyContents = open(ltxname, 'r').read() # read entire file
  return replyContents

# next until breakpoint
def nextUntilBreakpoint():
  isInBreakpoint = -1;
  # as long as we don't find "Breakpoint" in report:
  while isInBreakpoint == -1:
    REP=logExecCapture("n")
    isInBreakpoint = REP.find("Breakpoint")
    print "LOOP:: ", isInBreakpoint, "\n", REP
################################################################################

class QStringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        if self.val['d'] == self.val['shared_null'].address:
            return 0

        dataptr = self.val['d']['data'].cast(gdb.lookup_type('char').pointer())
        size = self.val['d']['size']

        if sys.byteorder == 'little':
            enc = 'utf_16_le'
        else:
            enc = 'utf_16_be'
        return dataptr.string(enc, 'ignore', size * 2)

    def display_hint(self):
        return 'string'

################################################################################

import gdb

class Offsets(gdb.Command):
    def __init__(self):
        super (Offsets, self).__init__ ('offsets-of', gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        argv = gdb.string_to_argv(arg)
        if len(argv) != 1:
            raise gdb.GdbError('offsets-of takes exactly 1 argument.')

        stype = gdb.lookup_type(argv[0])

        print argv[0], '{'
        for field in stype.fields():
            print '    %s => %d' % (field.name, field.bitpos//8)
        print '}'

Offsets()

(gdb) offsets-of "struct A"

################################################################################
