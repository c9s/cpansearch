package App::cpansearch;
use warnings;
use strict;

our $VERSION = 0.01;

=head1 NAME

App::cpansearch - A lightweight fast cpan module search utility written in C.

=head1 SYNOPSIS

To initialize source list datafile.

    $ cpans --fetch http://cpan.nctu.edu.tw/

    # this will autodetect local mirror from .minicpanrc
    $ cpans --fetch

    # the same
    $ cpans -f

To update list:

    $ cpans --update

    $ cpans -u

To search modules:

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

=cut

1;
