
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## to initialize source list datafile.

    $ cpans --init http://cpan.nctu.edu.tw/

## to update list

    $ cpans --update

## to search modules

Supports posix extended regular expression:

    $ cpans App::

    $ cpans "^Any"

    $ cpans "Server$"

    $ cpans [A-C][A-C]
