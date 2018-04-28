use strict;
use warnings;

my %collect = (
    DVI => [],
    JPI => [],
    RMI => [],
    SYI => [],
   );
while (<STDIN>) {
    s|\R$||;
    if (/^\s*i\((\d+),\s*((\w+)\$_\w+)\),\s*$/) {
	my $len = $1 * 4;	# 32-bit words
	push @{$collect{$3}}, "{ $2, $len, \"$2\", 0 }";
    } elsif (/^\s*it\(((\w+)\$_\w+)\),\s*$/) {
	push @{$collect{$2}}, "{ $1, 8, \"$1\", 1 }";
    }
}

print << '_____';
#include <stdlib.h>
#include "dict.h"
_____
foreach (map { lc $_; } sort keys %collect) {
    print "#include <${_}def.h>\n";
}
foreach my $key (sort keys %collect) {
    print "DESC ${key}_dict[] = {\n";
    foreach (@{$collect{$key}}) {
	print "  $_,\n";
    }
    print "};\n";
}

# It is IMPORTANT that the position of each list matches the dictnum macros
# in dict.h!
print << '_____';
struct { DESC *dict; size_t dict_sz; size_t dict_membsz; } dicts[] = {
    { DVI_dict, sizeof(DVI_dict), sizeof(DVI_dict[0]) },
    { JPI_dict, sizeof(JPI_dict), sizeof(JPI_dict[0]) },
    { RMI_dict, sizeof(RMI_dict), sizeof(RMI_dict[0]) },
    { SYI_dict, sizeof(SYI_dict), sizeof(SYI_dict[0]) },
};
_____
