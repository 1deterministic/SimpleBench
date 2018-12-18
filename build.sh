mkdir build &&
gcc -c main.cpp -o build/main.o &&
gcc -c alu.cpp -o build/alu.o &&
gcc -c fpu.cpp -o build/fpu.o &&
gcc -c mem.cpp -o build/mem.o &&
gcc -c gui.cpp -o build/gui.o &&
gcc -c thread.cpp -o build/thread.o &&
gcc -c chronometer.cpp -o build/chronometer.o &&
gcc -c simplebench.cpp -o build/simplebench.o &&
gcc -o simplebench build/*.o -pthread &&
rm -r build &&
chmod +x simplebench
