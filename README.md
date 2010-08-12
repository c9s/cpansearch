
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## To install

    **REQUIREMENT**
        curl
        glib

    $ make install

## To initialize source list datafile.

    $ cpans --init http://cpan.nctu.edu.tw/

## To update list

    $ cpans --update

## To search modules

Supports posix extended regular expression:

    $ cpans App::

    $ cpans "^Any"

    $ cpans "Server$"

    $ cpans [A-C][A-C]


## TODO

    search and install.

        $ cpans -i AnyEvent::HTTP    # call cpanminus to install the only matched one.

    open browser to search

        $ cpans -w AnyEvent

    open document in browser

        $ cpans -d AnyEvent

    ncurses UI support

