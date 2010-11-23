/* gen_eval.c */
/* (C) John Mair 2009
 * This program is distributed under the terms of the MIT License
 *                                                                */

#include <ruby.h>
#include "compat.h"

static VALUE cRubyObjectIv;

typedef struct RubyObjectIv {
  long numiv;
  VALUE * ivptr;
  struct st_table * iv_index_tbl;
} RubyObjectIv;

VALUE
rb_mirror_object(VALUE context)
{
  VALUE duped_context;
  
  duped_context = rb_funcall(rb_cObject, rb_intern("new"), 0);

  rb_iv_set(context, "__force_iv_tbl_1__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_2__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_3__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_4__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_5__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_6__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_7__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_8__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_9__", Qtrue);
  rb_iv_set(context, "__force_iv_tbl_10__", Qtrue);
  
  rb_iv_set(context, "_", Qtrue);

  RubyObjectIv * old_iv = ALLOC(RubyObjectIv);
  old_iv->ivptr = ROBJECT(duped_context)->as.heap.ivptr;
  old_iv->numiv = ROBJECT(duped_context)->as.heap.numiv;
  old_iv->iv_index_tbl= ROBJECT(duped_context)->as.heap.iv_index_tbl;

  ROBJECT(duped_context)->as.heap.ivptr = ROBJECT(context)->as.heap.ivptr;
  ROBJECT(duped_context)->as.heap.numiv = ROBJECT(context)->as.heap.numiv;
  ROBJECT(duped_context)->as.heap.iv_index_tbl = ROBJECT(context)->as.heap.iv_index_tbl;

  rb_iv_set(duped_context, "__old_iv__", Data_Wrap_Struct(cRubyObjectIv, 0, free, old_iv));

    
  return duped_context;
}

VALUE
rb_unmirror_object(VALUE duped_context)
{
  
  RubyObjectIv * restored_iv;
  VALUE wrapped_iv = rb_iv_get(duped_context, "__old_iv__");
  
  Data_Get_Struct(wrapped_iv, RubyObjectIv, restored_iv);

  ROBJECT(duped_context)->as.heap.ivptr = restored_iv->ivptr;
  ROBJECT(duped_context)->as.heap.numiv = restored_iv->numiv;
  ROBJECT(duped_context)->as.heap.iv_index_tbl = restored_iv->iv_index_tbl;
  RBASIC(duped_context)->flags |= ROBJECT_EMBED;
  
  return Qnil;
}
  
void
Init_gen_eval()
{
  cRubyObjectIv = rb_define_class("RubyObjectIv", rb_cObject);
  
  rb_define_method(rb_cObject, "mirror", rb_mirror_object, 0);
  rb_define_method(rb_cObject, "unmirror", rb_unmirror_object, 0);
}

    
    

    






/* static void */
/* save_m_tbl(VALUE klass) */
/* { */
/*   rb_iv_set(klass, "__saved_m_tbl__", (VALUE)RCLASS_M_TBL(klass)); */
/* } */

/* static void */
/* restore_m_tbl(VALUE klass) */
/* { */
/*   RCLASS_M_TBL(klass) = (struct st_table *)rb_iv_get(klass, "__saved_m_tbl__"); */
/* } */

/* /\* we do not want any ROBJECT_EMBED objects, so ensure we have > 3 ivars *\/ */
/* static void */
/* force_iv_tbl(VALUE obj) */
/* { */
/*   rb_iv_set(obj, "__force_iv_tbl_1__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_2__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_3__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_4__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_5__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_6__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_7__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_8__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_9__", Qtrue); */
/*   rb_iv_set(obj, "__force_iv_tbl_10__", Qtrue); */
/* } */

/* #ifdef RUBY_19 */
/* static void */
/* redirect_iv_for_object(VALUE obj, VALUE dest) */
/* { */
/*   if(TYPE(obj) != T_OBJECT) */
/*     rb_raise(rb_eArgError, "must provide a T_OBJECT"); */

/*   /\* ensure ivars are stored on the heap and not embedded *\/ */
/*   force_iv_tbl(obj); */

/*   if (RBASIC(obj)->flags & ROBJECT_EMBED) { */
/*     rb_raise(rb_eArgError, "im sorry gen_eval does not yet work with R_OBJECT_EMBED types"); */
/*   } */
/*   else { */
/*     ROBJECT(dest)->as.heap.ivptr = ROBJECT(obj)->as.heap.ivptr; */
/*     ROBJECT(dest)->as.heap.numiv = ROBJECT(obj)->as.heap.numiv; */
/*     ROBJECT(dest)->as.heap.iv_index_tbl = ROBJECT(obj)->as.heap.iv_index_tbl; */
/*   } */
/* } */

/* static void */
/* release_iv_for_object(VALUE obj) */
/* { */
/*   if(TYPE(obj) != T_OBJECT) */
/*     rb_raise(rb_eArgError, "must provide a T_OBJECT"); */
    
/*   ROBJECT(obj)->as.heap.ivptr = (void *) 0; */
/*   ROBJECT(obj)->as.heap.numiv = 0; */
/*   ROBJECT(obj)->as.heap.iv_index_tbl = (void *) 0; */
/*   RBASIC(obj)->flags &= ~ROBJECT_EMBED; */
/* } */
/* #endif */
/* /\** end of Ruby 1.9 funcs **\/ */
