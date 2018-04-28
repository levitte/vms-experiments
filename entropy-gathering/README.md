Test entropy gathering

A few programs can be built.  The most interesting is entropy-stats.exe,
which gathers raw data from a number of DVI$, JPI$, RMI$ and SYI$ items
(see dict.c for the exact list) all in all 736 bytes of data each round.
Apart from calculating an entropy estimate and displaying every item
that gave *some* entropy according to that calculation, it also dumps
all the data into a file (entropy-stats.bin) that can be fed to NISTs
minimum entropy estimation program (non-iid), found here:

https://github.com/usnistgov/SP800-90B_EntropyAssessment
