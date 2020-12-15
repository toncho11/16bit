A small editor that is to be used on old and slow computers ex. 16 bit 8086.

Base ideology:
- low amount of functionality (no commands, no search, ...)
- performance is priority (clicking a key should not take an eternity to output the character)
- first version will be able to edit only a file that fits on the screen (not bad for many small config files and your own scripts)

Features:

- a working backspace (yes, it is not a small thing)
- show position of cursor (option enabled from command line before start)
- ctrl+c does exit the program without saving

What is working now:

- load file
- arrows (but they should not go everywhere)
- backspace for 1 line file
- spacebar and new characters move right characters further right (instead of overwrite)

What is missing:

- correct handling of text that is more than one line
- savefile
- ctrl-S, ctrl-X