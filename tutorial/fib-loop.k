def binary : 1 (x y) y;
def fib(n)
  if (n < 2) then
    n
  else
    var x, y in (spawn x = fib(n-1)) : y = fib(n-2) : sync : (x+y);
def fibloop(n)
  parfor i = 0, i < n in
    fib(i);
fibloop(30);
