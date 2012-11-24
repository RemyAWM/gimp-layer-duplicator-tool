/*
 * ldt_operation_translate.c
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

#include "ldt_operation_translate.h"

#include "plugin-intl.h"


static void    ldt_operation_translate_class_init       (LdtOperationTranslateClass 		*klass);

static void    ldt_operation_translate_init             (LdtOperationTranslate      		*object);

static void    ldt_operation_translate_constructed	 (GObject         *object);

static void    ldt_operation_translate_finalize         (GObject         *object);


G_DEFINE_TYPE (LdtOperationTranslate, ldt_operation_translate, LDT_OPERATION_TYPE)

#define parent_class ldt_operation_translate_parent_class


static void    
ldt_operation_translate_class_init (LdtOperationTranslateClass *klass)
{
	GObjectClass      	*object_class      	= G_OBJECT_CLASS (klass);

	object_class->constructed	=	ldt_operation_translate_constructed;
	object_class->finalize		= 	ldt_operation_translate_finalize;

}

static void
ldt_operation_translate_init (LdtOperationTranslate *object)
{
}

static void
ldt_operation_translate_constructed (GObject *object)
{
	LdtOperation *ldt_operation = LDT_OPERATION(object);

	ldt_operation_set_name(ldt_operation,"Translate");
	ldt_operation_set_icon_stock(ldt_operation,GIMP_STOCK_TOOL_MOVE);
	ldt_operation_set_state(ldt_operation,TRUE);

	G_OBJECT_CLASS (parent_class)->constructed (object);
}

static void
ldt_operation_translate_finalize (GObject *object)
{
	G_OBJECT_CLASS (parent_class)->finalize (object);
}


/* Public functions */

LdtOperation*
ldt_operation_translate_new()
{
	LdtOperation *obj = g_object_new (LDT_OPERATION_TRANSLATE_TYPE,NULL);
	return obj;
}


