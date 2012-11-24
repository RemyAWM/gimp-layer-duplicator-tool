/*
 * ldt_tree_model_operations.h
 *
 *  Created on: 19.10.2012
 *      Author: helen
 */

#ifndef LDT_TREE_MODEL_OPERATIONS_H_
#define LDT_TREE_MODEL_OPERATIONS_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define LDT_TREE_MODEL_OPERATION_TYPE           	(ldt_tree_model_operation_get_type ())
#define LDT_TREE_MODEL_OPERATION(obj)            	(G_TYPE_CHECK_INSTANCE_CAST ((obj), LDT_TREE_MODEL_OPERATION_TYPE, LdtTreeModelOperation))
#define LDT_TREE_MODEL_OPERATION_CLASS(klass)    	(G_TYPE_CHECK_CLASS_CAST ((klass),  LDT_TREE_MODEL_OPERATION_TYPE, LdtTreeModelOperationClass))
#define LDT_IS_TREE_MODEL_OPERATION(obj)         	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LDT_TREE_MODEL_OPERATION_TYPE))
#define LDT_IS_TREE_MODEL_OPERATION_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass),  LDT_TREE_MODEL_OPERATION_TYPE))
#define LDT_TREE_MODEL_OPERATION_GET_CLASS(obj)  	(G_TYPE_INSTANCE_GET_CLASS ((obj),  LDT_TREE_MODEL_OPERATION_TYPE, LdtTreeModelOperationClass))


enum
  {
    COL_ICON = 0,
    COL_NAME,
    COL_RENDER_SRC,
    COL_COUNT,
    COL_TOGGLE,
    COL_OBJECT,
    COL_SHOWS,
    NUM_COLS
  };


typedef struct _LdtTreeModelOperationClass LdtTreeModelOperationClass;

struct _LdtTreeModelOperation
{
	GtkTreeStore 			parent_instance;
};

struct _LdtTreeModelOperationClass
{
	GtkTreeStoreClass parent_class;
};

GType           ldt_tree_model_operation_get_type          (void) G_GNUC_CONST;

/*
 * public function declarations
 */
LdtTreeModelOperation* 			ldt_tree_model_operation_new				(gint n_columns, ...);

G_END_DECLS


#endif /* LDT_TREE_MODEL_OPERATIONS_H_ */
