file signal

handle SIGALRM print
handle SIGUSR1 print

catch signal SIGUSR1

command 1
	ptype $_siginfo #convenience variable
end

start
continue