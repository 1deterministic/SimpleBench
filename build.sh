#gcc -m32 -march=native -pthread *.cpp -o simplebench &&
#gcc -march=native -pthread *.cpp -o simplebench &&
gcc -O0 -march=native -pthread *.cpp -o simplebench &&
chmod +x simplebench
