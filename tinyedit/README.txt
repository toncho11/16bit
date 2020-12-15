A small editor that is to be used on olw and slow computers ex. 16 bit 8086.

Base idelology:
- low amount of functionality (no commands, no search, ...)
- performance is priority (clicking a key should not take an eternity to output the character)
- first version will be able to edit only a file that fits on the screen (not bad for many small config files and your own scripts)

Features:

- a working backspace (yes, it is not a small thing)
- show position of cursor (option enabled from command line before start)

What is working now:

- load file
- arrows
- backspace for 1 line file

What is missing:

- correct handling of text that is more than one line
- spacebar and characters moves text right (instead of overwrite)
- savefile
- ctrl-S, ctrl-X