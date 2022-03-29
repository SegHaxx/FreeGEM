@ECHO OFF
:CHOOSE
CLS
ECHO               **************************************************
ECHO               *                                                *
ECHO               * What would you like to do now?                 *
ECHO               *                                                *
ECHO               * 1 - Change the video driver                    *
ECHO               *                                                *
ECHO               * 2 - Change the sound driver                    *
ECHO               *                                                *
ECHO               * 3 - Change the printer driver                  *
ECHO               *                                                *
ECHO               * 4 - Repair OpenGEM                             *
ECHO               *                                                *
ECHO               *                                                *
ECHO               *                                                *
ECHO               * 5 - Get Help                                   *
ECHO               *                                                *
ECHO               * 6 - Exit and return to DOS                     *
ECHO               *                                                *
ECHO               **************************************************
ECHO.
ECHO                           Please select an option...
CD \GEMAPPS\GEMSYS
choice /c:123456
if errorlevel 6 goto 6
if errorlevel 5 goto 5
if errorlevel 4 goto 4
if errorlevel 3 goto 3
if errorlevel 2 goto 2
if errorlevel 1 goto 1
CLS

:1
REM Change the video driver
CD SETUP
VIDEO
GOTO CHOOSE
CLS

:2
CLS
REM Change the sound driver
CD SETUP
SOUND
GOTO CHOOSE
CLS

:3
CLS
REM Change the printer driver
CD SETUP
PRINTER
GOTO CHOOSE
CLS

:4
CLS
REM Repair OpenGEM
CD SETUP
REPAIR
GOTO CHOOSE
CLS

:5
CLS
ECHO               **************************************************
ECHO               *                                                *
ECHO               *               OpenGEM Setup Help               *
ECHO               *                                                *
ECHO               * OpenGEM SETUP is designed to make changing     *
ECHO               * FreeGEM sound, video and printer drivers easy. *
ECHO               *                                                *
ECHO               * Before you change sound or video drivers       *
ECHO               * please check what type of hardware you have.   *
ECHO               * Using the wrong driver can crash your system.  *
ECHO               *                                                *
ECHO               * When changing the printer drivers keep in mind *
ECHO               * that it might be impossible to get an exact    *
ECHO               * match for your printer. You are advised to     *
ECHO               * try different drivers until you find one that  *
ECHO               * works for you.                                 *
ECHO               *                                                *
ECHO               **************************************************
ECHO.
ECHO                   Press any key to return to the main menu...
PAUSE > NUL
GOTO END
CLS

:6
REM Exit OpenGEM SETUP
:END
CD \GEMAPPS
CLS
ECHO               **************************************************
ECHO               *       Thank you for using OpenGEM Setup        *
ECHO               **************************************************
ECHO.
