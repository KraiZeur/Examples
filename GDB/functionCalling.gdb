# gdb -tui -q ./FunctionCalling
# source functionCalling.gdb

file FunctionCalling

break call

commands
  printf "called : %d %s", i, reference.c_str()
  continue
end

run
