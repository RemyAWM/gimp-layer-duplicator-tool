/*
 * ldt_operation.h
 *
 *  Created on: 08.10.2012
 *      Author: remy
 */

#ifndef LDT_OPERATION_H_
#define LDT_OPERATION_H_


#include <glib-object.h>

G_BEGIN_DECLS

#define LDT_OPERATION_TYPE           	(ldt_operation_get_type ())
#define LDT_OPERATION(obj)            	(G_TYPE_CHECK_INSTANCE_CAST ((obj), LDT_OPERATION_TYPE, LdtOperation))
#define LDT_OPERATION_CLASS(klass)    	(G_TYPE_CHECK_CLASS_CAST ((klass), LDT_OPERATION_TYPE, LdtOperationClass))
#define LDT_IS_OPERATION(obj)         	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LDT_OPERATION_TYPE))
#define LDT_IS_OPERATION_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass), LDT_OPERATION_TYPE))
#define LDT_OPERATION_GET_CLASS(obj)  	(G_TYPE_INSTANCE_GET_CLASS ((obj), LDT_OPERATION_TYPE, LdtOperationClass))

typedef struct _LdtOperationClass LdtOperationClass;

struct _LdtOperation
{
	GObject parent_instance;
};

struct _LdtOperationClass
{
	GObjectClass 	parent_class;

	void 			(* name_changed)			(LdtOperation* obj);
	void 			(* icon_stock_changed)		(LdtOperation* obj);
	void 			(* state_changed)			(LdtOperation* obj);

};

GType           ldt_operation_get_type          (void) G_GNUC_CONST;

/*  
 * public function declarations  
 */

LdtOperation* 			ldt_operation_new 				(void);

void			ldt_operation_set_name 		(LdtOperation  *object,
                      	  	  	  	  	 	 	 const gchar *name);

void			ldt_operation_set_icon_stock (LdtOperation  *object,
                      	  	  	  	  	 	 	 const gchar *icon_stock);

void			ldt_operation_set_state 		(LdtOperation  *object,
                      	  	  	  	  	 	 	 gboolean state);

const gchar 	*ldt_operation_name 			(LdtOperation  *object);

const gchar 	*ldt_operation_icon_stock 	(LdtOperation  *object);

gboolean 		ldt_operation_state 			(LdtOperation  *object);

G_END_DECLS

#endif /* LDT_OPERATION_H_ */
