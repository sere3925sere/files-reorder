Download: files-reorder.exe
Source code: https://github.com/sere3925sere/files-reorder

    The javascript userscript I use now to download images from derart
downloads them out of order for some reason, so I wrote this program 
to correct that.
    It lists all files in your download folder, and if you click
on one of them, it searches for all similarly named files 
(files with names like <anything>_p<number>.<extension>), 
and changes their creation time, modification time, 
and last accessed time.
    Also, as long as this program is open, it will keep refreshing 
that folder and do this sorting with the file at the top of the list.

Controls:
ESC to go back.
UP and DOWN to go up and down that list.
RIGHT, LEFT, PAGE DOWN and PAGE UP to flip pages.
ENTER or mouse click to sort selected file.
R to refresh file list.
UP when on the first page and on top of the list to also refresh file list.
O for options.
F1 for help.

To use this, upon first start, open options with key O (not zero),
and change folder path to your download path.
