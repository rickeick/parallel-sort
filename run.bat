@ECHO OFF

CD /d "%~dp0"

:: Settings

SET "EXECS=10"
SET "CORES=2 4 8"
SET "SIZES=16777216 134217728 1073741824"

:: Build

if not EXIST ".\bin" (
    MKDIR ".\bin"
)

gcc .\source\merge-sort.c -o .\bin\test-A0.exe
mpicc .\source\merge-sort-mpi.c -o .\bin\test-A1.exe
gcc .\source\merge-sort-omp.c -o .\bin\test-A2.exe -fopenmp
gcc .\source\merge-sort-oe.c -o .\bin\test-A3.exe -fopenmp
gcc .\source\bubble-sort-oe.c -o .\bin\test-A4.exe -fopenmp
gcc .\source\bubble-sort.c -o .\bin\test-A5.exe

gcc .\source\report.c .\source\sllist.c -o .\bin\report.exe

:: Tests

if not EXIST ".\logs" (
    MKDIR ".\logs"
)

for %%I in (%SIZES%) do (
    .\bin\test-A0.exe %EXECS% %%I
)

for %%I in (%SIZES%) do (
    for %%J in (%CORES%) do (
        for /l %%K in (1, 1, %EXECS%) do (
            mpiexec -n %%J .\bin\test-A1.exe %EXECS% %%K %%I
        )
    )
)

for %%I in (%SIZES%) do (
    for %%J in (%CORES%) do (
        .\bin\test-A2.exe %EXECS% %%I %%J
    )
)

for %%I in (%SIZES%) do (
    for %%J in (%CORES%) do (
        .\bin\test-A3.exe %EXECS% %%I %%J
    )
)

@REM for %%I in (%SIZES%) do (
@REM     for %%J in (%CORES%) do (
@REM         .\bin\test-A4.exe %EXECS% %%I %%J
@REM     )
@REM )

@REM for %%I in (%SIZES%) do (
@REM     for %%J in (%CORES%) do (
@REM         .\bin\test-A5.exe %EXECS% %%I %%J
@REM     )
@REM )

:: Reports

if not EXIST ".\reports" (
    MKDIR ".\reports"
)

.\bin\report.exe A0 A1
.\bin\report.exe A0 A2
.\bin\report.exe A0 A3
