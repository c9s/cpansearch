
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## To install

**REQUIREMENT**
*curl
*glib

    $ make install

## Usage


--init [uri] | --fetch [uri] | -i [uri]

> initialize a source list from a specified uri:

--update | -u
    
> update source list data.

-i
    
> ignore case

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

    $ cpans "Server$"

    $ cpans [A-C][A-C]

    # to ignore case
    $ cpans -i server


## PLATFORM

    * unix-like
    * win32   (thanks to @mattn)

## TODO

    search and install.

        $ cpans -i AnyEvent::HTTP    # call cpanminus to install the only matched one.

    open browser to search

        $ cpans -w AnyEvent

    open document in browser

        $ cpans -d AnyEvent

    ncurses UI support

