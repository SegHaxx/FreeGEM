@ECHO OFF
SET OPTIM=-O -Zg
ECHO Making Pacific C Example programs ...
ECHO This may take some time on slower machines.
ECHO If compilation takes too long, change the
ECHO set OPTIM statement at the top of this batch
ECHO file.  Current optimization flags: %OPTIM%
ECHO ON
PACC %OPTIM% ARGS.C
PACC %OPTIM% CHK.C
PACC %OPTIM% EXITFUNC.C
PACC %OPTIM% -Lf FCALC.C
PACC %OPTIM% -Bl FULLPATH.C
PACC %OPTIM% HELLO.C
PACC %OPTIM% HEXD.C
PACC %OPTIM% KEYRATE.C
PACC %OPTIM% MOUSEX.C MOUSE.C
PACC %OPTIM% PRINTENV.C
PACC %OPTIM% QSORTEX.C
PACC %OPTIM% READDIR.C
PACC %OPTIM% READDIR2.C
PACC %OPTIM% SIEVE.C
PACC %OPTIM% TALK.C
PACC %OPTIM% U2D.C
PACC %OPTIM% USAGE.C
@ECHO OFF
SET OPTIM=
ECHO Done ...
ECHO Try some of the following commands:
ECHO ARGS.C program:     ARGS a b c d e f
ECHO CHK.C program:      CHK ARGS.EXE
ECHO FULLPATH.C program: FULLPATH
ECHO HEXD.C program:     HEXD HELLO.C
ECHO KEYRATE.C program:  KEYRATE 1 30
ECHO PRINTENV.C program: PRINTENV COMSPEC
