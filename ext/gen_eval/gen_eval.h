/* gen_eval.h */

#ifndef GUARD_GEN_EVAL_H
#define GUARD_GEN_EVAL_H

#include <ruby.h>

VALUE retrieve_hidden_self(VALUE duped_context);
void set_hidden_self(VALUE duped_context, VALUE hidden_self);
VALUE remove_hidden_self(VALUE duped_context);
VALUE rb_mirror_object(VALUE context);
VALUE rb_unmirror_object(VALUE duped_context);



/* change self to hidden self if __hidden_self__ defined */
#define ADJUST_SELF(X)                                                    \
  do {                                                                    \
      if(!NIL_P(retrieve_hidden_self((X))))                               \
          (X) = retrieve_hidden_self((X));                                \
  } while(0)
        
#endif
