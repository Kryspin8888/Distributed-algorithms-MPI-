set terminal png
set output "wykres.png"
f(x)=a*x+b
fit f(x) "czasy.txt" via a, b
plot "czasy.txt" with points, f(x) with lines
quit