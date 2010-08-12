
cpansearch
==========

cpansearch is written in C, provides a quick interface for searching cpan
modules.

## To install

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
