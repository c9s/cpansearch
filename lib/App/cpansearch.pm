package App::cpansearch;
use warnings;
use strict;

our $VERSION = 0.021;

=head1 NAME

App::cpansearch - A lightweight fast cpan module search utility written in C.

=head1 DESCRIPTION

A lightweight fast cpan module search utility written in C.

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


=head1 AUTHOR

Cornelius, C<< <cornelius.howl at gmail.com> >>

=head1 BUGS

Please report any bugs or feature requests to C<bug-app-gh at rt.cpan.org>, or through
the web interface at L<http://rt.cpan.org/NoAuth/ReportBug.html?Queue=App-gh>.  I will be notified, and then you'll
automatically be notified of progress on your bug as I make changes.

=head1 SUPPORT

You can find documentation for this module with the perldoc command.

    perldoc App::gh

You can also look for information at:

=over 4

=item * RT: CPAN's request tracker

L<http://rt.cpan.org/NoAuth/Bugs.html?Dist=App-gh>

=item * AnnoCPAN: Annotated CPAN documentation

L<http://annocpan.org/dist/App-gh>

=item * CPAN Ratings

L<http://cpanratings.perl.org/d/App-gh>

=item * Search CPAN

L<http://search.cpan.org/dist/App-gh/>

=back

=head1 ACKNOWLEDGEMENTS


=head1 LICENSE AND COPYRIGHT

Copyright 2010 Cornelius.

This program is free software; you can redistribute it and/or modify it
under the terms of either: the GNU General Public License as published
by the Free Software Foundation; or the Artistic License.

See http://dev.perl.org/licenses/ for more information.


=cut

1;
