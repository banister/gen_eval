/* gen_eval.c */
/* (C) John Mair 2009
 * This program is distributed under the terms of the MIT License
 *                                                                */

#include <ruby.h>
#include "compat.h"

VALUE
retrieve_hidden_self(VALUE duped_context)
{
    VALUE thread_id, unique_name, hidden_self;
    
    /* retrieve hidden self (if it exists) */
    thread_id = rb_funcall(rb_obj_id(rb_thread_current()), rb_intern("to_s"), 0);
    unique_name = rb_str_plus(rb_str_new2("__hidden_self__"), thread_id);
    hidden_self = rb_ivar_get(duped_context, rb_to_id(unique_name));

    return hidden_self;
}

VALUE
remove_hidden_self(VALUE duped_context)
{
    /* retrieve hidden self (if it exists) */
    VALUE thread_id, unique_name;
    
    thread_id = rb_funcall(rb_obj_id(rb_thread_current()), rb_intern("to_s"), 0);
    unique_name = rb_str_plus(rb_str_new2("__hidden_self__"), thread_id);
    
    rb_obj_remove_instance_variable(duped_context, unique_name);

    return Qnil;
}

VALUE
set_hidden_self(VALUE duped_context, VALUE hidden_self)
{
    VALUE thread_id, unique_name;
    
    /* generate a unique (thread safe) name for the hidden self  */
    thread_id = rb_funcall(rb_obj_id(rb_thread_current()), rb_intern("to_s"), 0);
    unique_name = rb_str_plus(rb_str_new2("__hidden_self__"), thread_id);

    /* store self in hidden var in duped context */
    rb_ivar_set(duped_context, rb_to_id(unique_name), hidden_self);

    return Qnil;
}

static void
save_m_tbl(VALUE klass)
{
    rb_iv_set(klass, "__saved_m_tbl__", (VALUE)RCLASS_M_TBL(klass));
}

static void
restore_m_tbl(VALUE klass)
{
    RCLASS_M_TBL(klass) = (struct st_table *)rb_iv_get(klass, "__saved_m_tbl__");
}

/* we do not want any ROBJECT_EMBED objects, so ensure we have > 3 ivars */
static void
force_iv_tbl(VALUE obj)
{
  rb_iv_set(obj, "__force_iv_tbl_1__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_2__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_3__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_4__", Qtrue);
}

#ifdef RUBY_19
static void
redirect_iv_for_object(VALUE obj, VALUE dest)
{
    if(TYPE(obj) != T_OBJECT)
        rb_raise(rb_eArgError, "must provide a T_OBJECT");

    /* ensure ivars are stored on the heap and not embedded */
    force_iv_tbl(obj);

    if (!(RBASIC(dest)->flags & ROBJECT_EMBED) && ROBJECT_IVPTR(dest)) {
        rb_raise(rb_eArgError, "im sorry gen_eval does not yet work with this type of ROBJECT");
    }
    if (RBASIC(obj)->flags & ROBJECT_EMBED) {
        rb_raise(rb_eArgError, "im sorry gen_eval does not yet work with R_OBJECT_EMBED types");
    }
    else {
        ROBJECT(dest)->as.heap.ivptr = ROBJECT(obj)->as.heap.ivptr;
        ROBJECT(dest)->as.heap.numiv = ROBJECT(obj)->as.heap.numiv;
        ROBJECT(dest)->as.heap.iv_index_tbl = ROBJECT(obj)->as.heap.iv_index_tbl;
        RBASIC(dest)->flags &= ~ROBJECT_EMBED;
    }
}

static void
release_iv_for_object(VALUE obj)
{
    if(TYPE(obj) != T_OBJECT)
        rb_raise(rb_eArgError, "must provide a T_OBJECT");
    
    ROBJECT(obj)->as.heap.ivptr = (void *) 0;
    ROBJECT(obj)->as.heap.numiv = 0;
    ROBJECT(obj)->as.heap.iv_index_tbl = (void *) 0;
    RBASIC(obj)->flags &= ~ROBJECT_EMBED;
}
#endif
/** end of Ruby 1.9 funcs **/

VALUE
rb_mirror_object(VALUE context)
{
    VALUE duped_context;
  
#ifdef RUBY_19
    if(TYPE(context) == T_OBJECT)
        duped_context = rb_funcall(rb_cObject, rb_intern("new"), 0);
    else 
        duped_context = rb_funcall(rb_cClass, rb_intern("new"), 0);

#else
    duped_context = rb_funcall(rb_cClass, rb_intern("new"), 0);
#endif    
    
    /* the duped_context shares the context's iv_tbl.
       2 cases: (1) external iv_tbl, (2) local iv_tbl
       
       NOTE: we do not need to save original iv_tbl before replacing it, a brand new Class
       instance does not yet have an iv_tbl (the pointer is set to 0) 
    */
    if(FL_TEST(context, FL_EXIVAR)) 
        RCLASS_IV_TBL(duped_context) = (struct st_table *) rb_generic_ivar_table(context);
    else {
#ifdef RUBY_19
        if(TYPE(context) == T_OBJECT)
            redirect_iv_for_object(context, duped_context);
        else {
            save_m_tbl(duped_context);
            RCLASS_M_TBL(duped_context) = (struct st_table *) RCLASS_M_TBL(context);
            RCLASS_IV_TBL(duped_context) = (struct st_table *) RCLASS_IV_TBL(context);
        }
#else
        save_m_tbl(duped_context);
        RCLASS_M_TBL(duped_context) = (struct st_table *) RCLASS_M_TBL(context);
        RCLASS_IV_TBL(duped_context) = (struct st_table *) RCLASS_IV_TBL(context);
#endif        

        
    }

    /* ensure singleton exists */
    rb_singleton_class(context);
    
    /* set up the class hierarchy for our dup_context */
    KLASS_OF(duped_context) = rb_singleton_class_clone(context);

    return duped_context;
}

VALUE
rb_unmirror_object(VALUE duped_context)
{
#ifdef RUBY_19
    if(TYPE(duped_context) == T_OBJECT)
        release_iv_for_object(duped_context);
    else {
        RCLASS_IV_TBL(duped_context) = (struct st_table *) 0;
        restore_m_tbl(duped_context);
    }
#else
    RCLASS_IV_TBL(duped_context) = (struct st_table *) 0;
    restore_m_tbl(duped_context);
#endif

    return Qnil;
}
  
void
Init_gen_eval()
{
    rb_define_method(rb_cObject, "__mirror__", rb_mirror_object, 0);
    rb_define_method(rb_cObject, "__unmirror__", rb_unmirror_object, 0);
    rb_define_method(rb_cObject, "__set_hidden_self__", set_hidden_self, 1);
    rb_define_method(rb_cObject, "__retrieve_hidden_self__", retrieve_hidden_self, 0);
    rb_define_method(rb_cObject, "__remove_hidden_self__", retrieve_hidden_self, 0);
}

    
    

    
