REM Compile main.cpp to main.o
g++ -c main.cpp -o main.o -Winvalid-pch

REM Compile picasso.c to picasso.o
gcc -c picasso.c -o picasso.o

REM Link object files to create the final executable
g++ main.o picasso.o -o my_program

REM Optionally, you can remove the object files after linking
del main.o picasso.o

REM Run the executable
my_program

pause