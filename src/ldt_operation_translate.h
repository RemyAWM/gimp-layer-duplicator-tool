/*
 * ldt_operation_translate.h
 *
 *  Created on: 09.10.2012
 *      Author: remy
 */

#ifndef LDT_OPERATION_TRANSLATE_H_
#define LDT_OPERATION_TRANSLATE_H_


#include <glib-object.h>

#include "types.h"

#include "ldt_operation.h"

G_BEGIN_DECLS

#define LDT_OPERATION_TRANSLATE_TYPE           		(ldt_operation_translate_get_type ())
#define LDT_OPERATION_TRANSLATE(obj)            	(G_TYPE_CHECK_INSTANCE_CAST ((obj), LDT_OPERATION_TRANSLATE_TYPE, LdtOperationTranslate))
#define LDT_OPERATION_TRANSLATE_CLASS(klass)    	(G_TYPE_CHECK_CLASS_CAST ((klass), LDT_OPERATION_TRANSLATE_TYPE, LdtOperationTranslateClass))
#define LDT_IS_OPERATION_TRANSLATE(obj)         	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LDT_OPERATION_TRANSLATE_TYPE))
#define LDT_IS_OPERATION_TRANSLATE_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass), LDT_OPERATION_TRANSLATE_TYPE))
#define LDT_OPERATION_TRANSLATE_GET_CLASS(obj)  	(G_TYPE_INSTANCE_GET_CLASS ((obj), LDT_OPERATION_TRANSLATE_TYPE, LdtOperationTranslateClass))


typedef struct _LdtOperationTranslateClass LdtOperationTranslateClass;

struct _LdtOperationTranslate
{
	LdtOperation parent_instance;

};

struct _LdtOperationTranslateClass
{
	LdtOperationClass parent_class;

};

GType           ldt_operation_translate_get_type          (void) G_GNUC_CONST;

/*  
 * public function declarations  
 */

LdtOperation* 			ldt_operation_translate_new 				(void);

G_END_DECLS

#endif /* LDT_OPERATION_TRANSLATE_H_ */
