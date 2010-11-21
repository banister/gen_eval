/* gen_eval.c */
/* (C) John Mair 2009
 * This program is distributed under the terms of the MIT License
 *                                                                */

#include <ruby.h>
#include "compat.h"

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
  rb_iv_set(obj, "__force_iv_tbl_5__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_6__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_7__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_8__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_9__", Qtrue);
  rb_iv_set(obj, "__force_iv_tbl_10__", Qtrue);
}

#ifdef RUBY_19
static void
redirect_iv_for_object(VALUE obj, VALUE dest)
{
  if(TYPE(obj) != T_OBJECT)
    rb_raise(rb_eArgError, "must provide a T_OBJECT");

  /* ensure ivars are stored on the heap and not embedded */
  force_iv_tbl(obj);

  if (RBASIC(obj)->flags & ROBJECT_EMBED) {
    rb_raise(rb_eArgError, "im sorry gen_eval does not yet work with R_OBJECT_EMBED types");
  }
  else {
    ROBJECT(dest)->as.heap.ivptr = ROBJECT(obj)->as.heap.ivptr;
    ROBJECT(dest)->as.heap.numiv = ROBJECT(obj)->as.heap.numiv;
    ROBJECT(dest)->as.heap.iv_index_tbl = ROBJECT(obj)->as.heap.iv_index_tbl;
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
  if(TYPE(context) == T_OBJECT) {
    NEWOBJ(duped_context, struct RObject);
    OBJSETUP(duped_context, rb_cObject, T_OBJECT);
  }
  else  {
    //    duped_context = create_class(T_ICLASS, rb_cClass);
    duped_context = rb_funcall(rb_cClass, rb_intern("new"), 0);
  }
#else
  duped_context = rb_funcall(rb_cClass, rb_intern("new"), 0); //create_class(T_ICLASS, rb_cClass);
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
      //save_m_tbl(duped_context);
      //  RCLASS_M_TBL(duped_context) = (struct st_table *) RCLASS_M_TBL(context);
      RCLASS_IV_TBL(duped_context) = (struct st_table *) RCLASS_IV_TBL(context);
    }
#else
    //save_m_tbl(duped_context);
    //RCLASS_M_TBL(duped_context) = (struct st_table *) RCLASS_M_TBL(context);
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
  rb_define_method(rb_cObject, "mirror", rb_mirror_object, 0);
  rb_define_method(rb_cObject, "unmirror", rb_unmirror_object, 0);
}

    
    

    
