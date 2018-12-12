mkdir build
gcc -c main.cpp -o build/main.o &&
gcc -c simplebench.cpp -o build/simplebench.o &&
gcc -o simplebench build/main.o build/simplebench.o -pthread &&
rm -r build &&
chmod +x simplebench
