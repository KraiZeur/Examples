Compilation:

For better debugging don't forget
-g for gcc compiler
-O0 (inlining function disabled and extra optimizations)

gdb -x cmd.gdb

http://stackoverflow.com/questions/10748501/what-are-the-best-ways-to-automate-a-gdb-debugging-session

gdb --batch --command=test.gdb --args ./test.exe 5
