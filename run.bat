@ECHO OFF

CD /d "%~dp0"

if not EXIST ".\bin" (
    MKDIR ".\bin"
)

if not EXIST ".\logs" (
    MKDIR ".\logs"
)

gcc -I. -Iinc -c .\src\test.c -o .\bin\test.o
gcc -I. -Iinc -c .\src\merge-sort.c -o .\bin\merge-sort.o
::gcc -I. -Iinc -c .\src\merge-sort-oe.c -o .\bin\merge-sort-oe.o
gcc -I. -Iinc -c .\src\merge-sort-omp.c -o .\bin\merge-sort-omp.o -fopenmp
::gcc -I. -Iinc -c .\src\merge-sort-mpi.c -o .\bin\merge-sort-mpi.o
::gcc -I. -Iinc -c .\src\bubble-sort-eo.c -o .\bin\bubble-sort-eo.o

gcc .\bin\test.o .\bin\merge-sort.o .\bin\merge-sort-omp.o -o .\bin\test.exe -fopenmp

DEL .\bin\test.o
DEL .\bin\merge-sort.o
::DEL .\bin\merge-sort-oe.o
DEL .\bin\merge-sort-omp.o
::DEL .\bin\merge-sort-mpi.o
::DEL .\bin\bubble-sort-eo.o

for %%i in (100000 1000000 10000000) do (
    .\bin\test.exe 10 0 %%i
)

for %%i in (100000 1000000 10000000) do (
    for %%j in (2 4 6) do (
        .\bin\test.exe 10 2 %%i %%j
    )
)

if not EXIST ".\reports" (
    MKDIR ".\reports"
)

gcc -I. -Iinc -c .\src\report.c -o .\bin\report.o
gcc -I. -Iinc -c .\src\sllist.c -o .\bin\sllist.o

gcc .\bin\report.o .\bin\sllist.o -o .\bin\report.exe

DEL .\bin\report.o
DEL .\bin\sllist.o

.\bin\report.exe A2
