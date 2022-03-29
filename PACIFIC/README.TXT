This is the Freeware Version of Pacific C. Pacific
C is a high-performance C compiler for MS-DOS. It allows
you to write ANSI-standard C programs and compile to run
under the MS-DOS operating system. Pacific C will also
operate under Windows in a DOS box, but will not generate
Windows programs.

See the end of this file for release notes.

Getting Started:

The only program you have to run is PPD. This is the
integrated development environment.

Pacific C is invoked by the PPD command. Options to the
PPD command are:

	/screen:25
	/screen:28
	/screen:43
	/screen:50

These options set the DOS screen size to the specified number
of lines. A VGA or better display is needed for 50 lines, an
EGA or better for the others.

PPD will also take a file name, which may be a project file
name or a source file name. For example:

ppd /screen:43 testing

will start up PPD in 43 line mode, and look for a file TESTING.PRJ
or a file TESTING.C. A project file will be loaded, and a source
file will be loaded into the editor.

From within PPD you can load files into the editor with the File
menu. To create a project, that allows you to save source file names
and compile options in a project file, use the Make/New project command.

When you create a project, you will be prompted to fill in dialog boxes
with compiler options, then a source file list. Enter in the source file
list the names of all the source (.C) files you want to be compiled
and linked together. Assembler (.AS) files may also go here.

You can edit the options and the source file list from the Make,
Compile and Options menus.

To compile a single file from the editor window, use the Compile menu
or the F3 key. To "make" a project, which may involve compiling several
files and linking them, use the Make menu or the F5 key.

Explore the other menus (not forgetting the system menu that looks like
<<>> on the left hand side) to discover other features of
HI-TECH Software's Pacific C compiler.

HI-TECH Software also makes a range of C cross compilers for embedded
systems which are sold as full commercial products. Contact us for
more information.

HI-TECH Software
PO Box 103, Alderley
QLD 4051 Australia.

Phone +61 7 3355 8333
Fax   +61 7 3355 8334

Web:   http://www.htsoft.com
Email: hitech@htsoft.com
Ftp:   ftp.htsoft.com



Release notes:

V7.51:	Fixed assembler problem that incorrectly assembled an fsub
	instruction as an fsubr instruction. That's what happens when
	you believe Intel's manuals!

	The editor now hides a marked block after any cursor move,
	search etc. This avoids the situation where a block was marked,
	and remained enabled even though the cursor had been moved
	elsewhere, and pressing the DEL key would delete the block
	unexpectedly (and unrecoverably!)

	The debugger has been fixed to read the new symbol table format
	correctly. This means source level debugging now works. It also
	now uses bios calls for console I/O, thus isolating it from
	the debugged program's file I/O.
