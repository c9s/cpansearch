
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## MAIN FEATURES

* ncurses UI support.
* very fast.
* parallel module installation.
* doesn't require root permission.

## TO INSTALL

### REQUIREMENT

* curl
* glib
* ncurses
* menu

### BINARY

    $ wget http://github.com/c9s/cpansearch/raw/master/binary/cpans-linux-i386 -O cpans
    $ chmod +x cpans
    $ cp cpans ~/bin/

### BUILD - For debian-based system:

    $ sudo apt-get install libcurl4-openssl-dev libglib2.0-dev
    $ make install

### BUILD - For mac users:

install from macport:

    $ sudo port install curl glib2
    $ perl Makefile.PL
    $ make install

## USAGE

--fetch [uri] | -f [uri]

> initialize a source list from a specified uri:

--update | -u
    
> update source list data.

-i
    
> ignore case

-v

> verbose

-vv 

> very verbose

-vvv

> very verbose with long module url.

# EXAMPLE

## To initialize source list datafile.

    $ cpans -f http://cpan.nctu.edu.tw/

    # this will autodetect local mirror from .minicpanrc
    $ cpans -f

    # the same
    $ cpans --fetch

## To update list

    $ cpans -u

    $ cpans --update

## To search modules

Launch ncurses ui with search result:

    $ cpans --nc Any::

Supports posix extended regular expression:

    $ cpans App::

    $ cpans "^Any"

Show names only:

    $ cpans -n App::

Pipe to cpanminus to install:

    $ cpans -n App:: | cpanm

Show full url:

    $ cpans -nl App::

    $ cpans "Server$"

    $ cpans [A-C][A-C]

    # to ignore case
    $ cpans -i server


## AUTHORS

c9s <cornelius.howl@gmail.com>
Shellingford

## Contributors

Mattn

## PLATFORM

    * unix-like
    * win32   (thanks to @mattn)

## TODO

    search installed packages.

    search out-dated packages.

    search and install.

        $ cpans -i AnyEvent::HTTP    # call cpanminus to install the only matched one.

    open browser to search

        $ cpans -w AnyEvent

    open document in browser

        $ cpans -d AnyEvent
