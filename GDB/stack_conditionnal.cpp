/**
 * @run    gdb -q -x stack_conditionnal.py ./stack_conditionnal
 * @source http://stackoverflow.com/questions/5336403/is-there-any-way-to-set-a-breakpoint-in-gdb-that-is-conditional-on-the-call-stac
 */

 int a()
 {
   int p = 0;
   p = p +1;
   return  p;
 }

 int b()
 {
   return a();
 }

 int c()
 {
   return a();
 }

 int main()
 {
   c();
   for (int i = 0; i < 5; i++) {
     b();
   }
   a();
   return 0;
 }
