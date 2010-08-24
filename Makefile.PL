#!/usr/bin/env perl
use inc::Module::Install;
use Config;
my $binpath = $Config{installbin};

# vim:noet:sw=4:
if(-e 'Makefile') {
    warn "Original Makefile exists , Move to Makefile.old";
    rename 'Makefile' , 'Makefile.old';
}

my $ret;
print STDERR "Checking library...\n";
$ret = qx(pkg-config --cflags glib-2.0 libcurl);
unless( $ret ) {
	die $ret;
}

$ret = qx(pkg-config --libs glib-2.0 libcurl);
unless( $ret ) {
	die $ret;
}



name     'App-cpansearch';
all_from 'lib/App/cpansearch.pm';
author   q{Cornelius <cornelius.howl@gmail.com>};
license  'perl';

# use Data::Dumper; warn Dumper( \%Config );
WriteAll;

exit if $ENV{DIST};
open FH , ">" , 'Makefile';
print FH <<"END";

# THIS MAKEFILE IS AUTO-GENERATED , PLEASE DONT EDIT THIS DIRECTLY.
# PLEASE EDIT Makefile.PL.  THANKS

PROG=cpans

CFLAGS= -I. -Iinclude/ -Wall -Werror -std=c99 -g \$\$(pkg-config --cflags glib-2.0 libcurl)

LDFLAGS= \$\$(pkg-config --libs  glib-2.0 libcurl)

SRCS= src/cpans.c \\
	src/membuf.c \\
	src/help.c \\
	src/utils.c \\
	src/search.c \\
	src/init.c \\
	src/update.c

OBJS=\$(SRCS:.c=.o)

all: \$(PROG)

\$(PROG): \$(OBJS)
		\$(CC) \$(CFLAGS) \$(LDFLAGS) \$(OBJS) -o \$(PROG)
		\@echo "----------------"
		\@echo "When you upgrade cpans, please update the source data again with -i option:"
		\@echo "   cpans -f http://cpan.perl.org/"
		\@echo "----------------"

install: \$(PROG)
		./cpans --fetch http://cpan.perl.org/
		mkdir -p    $binpath
		cp \$(PROG) $binpath

test:
		true

clean:  
		rm -rf \$(PROG) \$(OBJS)

END
close FH;
