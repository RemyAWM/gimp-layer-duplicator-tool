/*
 * ldt_operation_rotate.c
 *
 *  Created on: 09.10.2012
 *      Author: remy
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimpbase/gimpbase.h>
#include <libgimp/gimpui.h>
#include <libgimpwidgets/gimpwidgets.h>

#include "types.h"

#include "ldt_operation_rotate.h"

#include "plugin-intl.h"

static void    ldt_operation_rotate_class_init       	(LdtOperationRotateClass 		*klass);

static void    ldt_operation_rotate_init             	(LdtOperationRotate      		*object);

static void    ldt_operation_rotate_constructed		(GObject         *object);

static void    ldt_operation_rotate_finalize         	(GObject         *object);


G_DEFINE_TYPE (LdtOperationRotate, ldt_operation_rotate, LDT_OPERATION_TYPE)

#define parent_class ldt_operation_rotate_parent_class

static void    
ldt_operation_rotate_class_init (LdtOperationRotateClass *klass)
{
	GObjectClass      	*object_class      	= G_OBJECT_CLASS (klass);

	object_class->finalize		= 	ldt_operation_rotate_finalize;
	object_class->constructed 	= 	ldt_operation_rotate_constructed;
}

static void
ldt_operation_rotate_init (LdtOperationRotate *object)
{
}

static void
ldt_operation_rotate_constructed (GObject *object)
{
	LdtOperation *ldt_operation = LDT_OPERATION(object);

	ldt_operation_set_name(ldt_operation,"Rotate");
	ldt_operation_set_icon_stock(ldt_operation,GIMP_STOCK_TOOL_ROTATE);
	ldt_operation_set_state(ldt_operation,TRUE);

	G_OBJECT_CLASS (parent_class)->constructed (object);
}

static void
ldt_operation_rotate_finalize (GObject *object)
{
	G_OBJECT_CLASS (parent_class)->finalize (object);
}


/* Public functions */

LdtOperation*
ldt_operation_rotate_new()
{
	   LdtOperation *obj = g_object_new (LDT_OPERATION_ROTATE_TYPE,NULL);

	  return obj;
}


