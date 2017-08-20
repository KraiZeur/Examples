# gdb -tui -q ./a.out
# source debugSimple.gdb

file a.out

set pagination off
set logging file gdb.txt
set logging on
break 16
commands
	p position->x
	printf "position : %d", position->x
	continue
end
start
continue

set logging off
quit

# http://users.ece.utexas.edu/~adnan/gdb-refcard.pdf
