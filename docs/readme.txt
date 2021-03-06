***************
* First Steps *
***************

Please read the introduction (at least "Getting Started") of the manual.


************
* Controls *
************

- Use any real gamepad or the keyboard:

	Button  Player 1    Player 2
	-------+-----------+--------
	UP      Arrow Up    E
	DOWN    Arrow Down  D
	LEFT    Arrow Left  S
	RIGHT   Arrow Right F
	A       Z/N         Q/Tab
	B       X/M         A/Shift

- More keys:
	Dev Menu     Esc
	Pause        Return/P
	Fullscreen   Ctrl+f
	Zoom Mode    Ctrl+z
	Screenshot
	   large     Ctrl+s
	   original  Ctrl+Shift+s
	Debug        Ctrl+d
	Reload/Run   Ctrl+r
	Eject        Ctrl+e
	Volume up    Ctrl+Plus
	Volume down  Ctrl+Minus
	Quit         Esc (if disabledev)


************
* Settings *
************

- Settings file:
	A default settings file is created on application start, if none
	exists yet. Available options are the same as for command line
	arguments, but each one is written in its own line and without
	the leading "-" character.
	macOS: /Users/YourName/Library/Application Support/Inutilis Software/LowRes NX/settings.txt
	Windows: C:\Users\YourName\AppData\Roaming\Inutilis Software\LowRes NX\settings.txt

- Command line arguments:
	These override the options from the settings file.

	"LowRes NX" [-option value] [program.nx]

	-fullscreen yes/no
	Start the application in fullscreen mode

	-zoom 0-3
	Start the application in zoom mode: 0 = pixel perfect, 1 = large, 2 = overscan, 3 = squeeze.

	-disabledev yes/no
	Disable the Development Menu, Esc key quits LowRes NX

	-mapping 0-1
	Set the key mapping. 0 is standard, 1 is GameShell.

	-disabledelay yes/no
	Disable the delay for too short frames.

	program.nx
	Name of the program to run


*********
* Notes *
*********

- Share programs and discuss on: https://lowresnx.inutilis.com

- Development news on Twitter: @timo_inutilis

- Tweet with #LowResNX.

- To-do and bug list on:
  https://github.com/timoinutilis/lowres-nx/issues

- Write to timo@inutilis.com.
