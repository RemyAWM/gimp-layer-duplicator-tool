/*
 * ldt_operation_rotate.h
 *
 *  Created on: 09.10.2012
 *      Author: remy
 */

#ifndef LDT_OPERATION_ROTATE_H_
#define LDT_OPERATION_ROTATE_H_

#include <glib-object.h>
#include "types.h"
#include "ldt_operation.h"

G_BEGIN_DECLS

#define LDT_OPERATION_ROTATE_TYPE           	(ldt_operation_rotate_get_type ())
#define LDT_OPERATION_ROTATE(obj)            	(G_TYPE_CHECK_INSTANCE_CAST ((obj), LDT_OPERATION_ROTATE_TYPE, LdtOperationRotate))
#define LDT_OPERATION_ROTATE_CLASS(klass)    	(G_TYPE_CHECK_CLASS_CAST ((klass),  LDT_OPERATION_ROTATE_TYPE, LdtOperationRotateClass))
#define LDT_IS_OPERATION_ROTATE(obj)         	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LDT_OPERATION_ROTATE_TYPE))
#define LDT_IS_OPERATION_ROTATE_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass),  LDT_OPERATION_ROTATE_TYPE))
#define LDT_OPERATION_ROTATE_GET_CLASS(obj)  	(G_TYPE_INSTANCE_GET_CLASS ((obj),  LDT_OPERATION_ROTATE_TYPE, LdtOperationRotateClass))

typedef struct _LdtOperationRotateClass LdtOperationRotateClass;

struct _LdtOperationRotate
{
	LdtOperation parent_instance;

};

struct _LdtOperationRotateClass
{
	LdtOperationClass parent_class;

};

GType           ldt_operation_rotate_get_type          (void) G_GNUC_CONST;

/*  
 * public function declarations  
 */

LdtOperation* 			ldt_operation_rotate_new 				(void);

G_END_DECLS

#endif /* LDT_OPERATION_ROTATE_H_ */
