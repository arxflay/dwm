### I'm not creator of dwm, i just modified config, changed sel tag indicator and applied some patches from suckless page

# dwm - dynamic window manager
dwm is an extremely fast, small, and dynamic window manager for X.
## Applied patches from suckless page:
* Autostart: https://github.com/axelGschaider/dwm-patch-autostart.sh/
* bar height: https://dwm.suckless.org/patches/bar_height/  
* enabled color emoji (you have to patch libxft with BGRA glyphs patch)

Changes in config:
custom keybinds
beautiful visuals


## Requirements
In order to build dwm you need the Xlib header files.


## Installation
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


##  Running dwm
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm


Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.