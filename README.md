
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## To install

**REQUIREMENT**

* curl

* glib

* ncurses

* menu

### Binary

    $ wget http://github.com/c9s/cpansearch/raw/master/binary/cpans-linux-i386 -O cpans
    $ chmod +x cpans
    $ cp cpans ~/bin/

### Build - For debian-based system:

    $ sudo apt-get install libcurl4-openssl-dev libglib2.0-dev
    $ make install

### Build - For mac users:

install from macport:

    $ sudo port install curl glib2
    $ perl Makefile.PL
    $ make install

## Usage

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

# Example

## To initialize source list datafile.

    $ cpans --fetch http://cpan.nctu.edu.tw/

    # this will autodetect local mirror from .minicpanrc
    $ cpans --fetch

    # the same
    $ cpans -f

## To update list

    $ cpans --update

    $ cpans -u

## To search modules

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

    # launch ncurses ui
    $ cpans --nc Any::

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
