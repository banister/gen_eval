require 'mkmf'

# let's use c99
$CFLAGS += " -std=c99"

create_makefile('gen_eval')
