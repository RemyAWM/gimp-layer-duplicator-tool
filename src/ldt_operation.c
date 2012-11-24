/*
 * ldt_operation.c
 *
 *  Created on: 08.10.2012
 *      Author: remy
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimpbase/gimpbase.h>
#include <libgimp/gimpui.h>

#include "types.h"

#include "ldt_operation.h"

#include "plugin-intl.h"


enum
{
  	NAME_CHANGED,
  	ICON_STOCK_CHANGED,
  	STATE_CHANGED,
  	LAST_SIGNAL
};

enum
{
	PROP_0,
	PROP_NAME,
	PROP_ICON_STOCK,
	PROP_STATE
};


typedef struct _LdtOperationPrivate LdtOperationPrivate;

struct _LdtOperationPrivate
{
	gchar *name;
	gchar *icon_stock;
	gboolean state;
};

#define GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE (obj, \
                                                       LDT_OPERATION_TYPE, \
                                                       LdtOperationPrivate)


static void    ldt_operation_class_init       	(LdtOperationClass 		*klass);

static void    ldt_operation_init             	(LdtOperation      		*object);

static void 	ldt_operation_constructed	  	(GObject         *object);

static void    ldt_operation_finalize         	(GObject         *object);

static void    ldt_operation_set_property     	(GObject          *object,
                                                 	 guint        		property_id,
                                                 	 const GValue  		*value,
                                                 	 GParamSpec     	*pspec);

static void    ldt_operation_get_property     	(GObject            *object,
                                                 	 guint               property_id,
                                                 	 GValue             *value,
                                                 	 GParamSpec         *pspec);


G_DEFINE_TYPE (LdtOperation, ldt_operation, G_TYPE_OBJECT)

#define parent_class ldt_operation_parent_class

static guint ldt_operation_signals[LAST_SIGNAL] = { 0 };



static void    
ldt_operation_class_init (LdtOperationClass *klass)
{
	GObjectClass      	*object_class      	= G_OBJECT_CLASS (klass);

	
	/* Регистрируем сигналы */
	ldt_operation_signals[NAME_CHANGED]=
		g_signal_new("name_changed",
			G_TYPE_FROM_CLASS(klass),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET (LdtOperationClass, name_changed),
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0);
	ldt_operation_signals[ICON_STOCK_CHANGED]=
		g_signal_new("icon_stock_changed",
			G_TYPE_FROM_CLASS(klass),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET (LdtOperationClass, icon_stock_changed),
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0);
	ldt_operation_signals[STATE_CHANGED]=
		g_signal_new("state_changed",
			G_TYPE_FROM_CLASS(klass),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET (LdtOperationClass, state_changed),
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0);

	object_class->constructed	=	ldt_operation_constructed;
	object_class->finalize		= 	ldt_operation_finalize;
	object_class->set_property 	= 	ldt_operation_set_property;
	object_class->get_property 	= 	ldt_operation_get_property;

	/* Регистрируем параметры объекта */
  	g_object_class_install_property (object_class, PROP_NAME,
                                   g_param_spec_string ("name", NULL, NULL,
                                                        NULL,
                                                        (GParamFlags)(G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT)));

  	g_object_class_install_property (object_class, PROP_ICON_STOCK,
                                   g_param_spec_string ("icon_stock", NULL, NULL,
                                                        NULL,
                                                        (GParamFlags)(G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT)));

  	g_object_class_install_property (object_class, PROP_STATE,
                                   g_param_spec_boolean ("state", NULL, NULL,
                                                        TRUE,
                                                        (GParamFlags)(G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT)));

	g_type_class_add_private (klass, sizeof (LdtOperationPrivate));
}

static void
ldt_operation_init (LdtOperation *object)
{
	LdtOperationPrivate *private = GET_PRIVATE (object);

	private->name = NULL;
	private->icon_stock = NULL;
	private->state = TRUE;
}

static void
ldt_operation_constructed (GObject *object)
{
	G_OBJECT_CLASS (parent_class)->constructed (object);
}


static void
ldt_operation_finalize (GObject *object)
{
	LdtOperationPrivate *private = GET_PRIVATE (object);

	if (private->name)
	    {
	      g_object_unref (private->name);
	      private->name = NULL;
	    }

	if (private->icon_stock)
	    {
	      g_object_unref (private->icon_stock);
	      private->icon_stock = NULL;
	    }

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
ldt_operation_set_property (GObject *object,
                       	   guint 		property_id,
                       	   const 		GValue *value,
                       	   GParamSpec 	*pspec)
{
	  LdtOperation *ldt_operation = LDT_OPERATION (object);

	  switch (property_id)
	    {
	    case PROP_NAME:
	    	ldt_operation_set_name (ldt_operation, g_value_get_string (value));
	      break;
	    case PROP_ICON_STOCK:
	    	ldt_operation_set_icon_stock (ldt_operation, g_value_get_string (value));
	      break;
	    case PROP_STATE:
	    	ldt_operation_set_state (ldt_operation, g_value_get_boolean (value));
	      break;
	    default:
	      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	      break;
	    }
}

static void
ldt_operation_get_property (GObject *object,
                           guint         property_id,
                           GValue        *value,
                           GParamSpec    *pspec)
{

	LdtOperationPrivate *private = GET_PRIVATE (object);

	  switch (property_id)
	    {
	    case PROP_NAME:
	      if (private->name)
	        g_value_set_string (value, private->name);
	      break;
	    case PROP_ICON_STOCK:
	      if (private->icon_stock)
	        g_value_set_string (value, private->icon_stock);
	      break;
	    case PROP_STATE:
	      if (private->state)
	        g_value_set_boolean (value, private->state);
	      break;
	    default:
	      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	      break;
	    }
}


/* Public functions */

LdtOperation*
ldt_operation_new()
{
	  return LDT_OPERATION (g_object_new (LDT_OPERATION_TYPE,NULL));
}

void
ldt_operation_set_name (LdtOperation  *object,
                      const gchar *name)
{
  g_return_if_fail (LDT_IS_OPERATION (object));

  LdtOperationPrivate *private = GET_PRIVATE (object);

  if (! g_strcmp0 (private->name, name))
    return;


  private->name = g_strdup (name);

  g_signal_emit (object, ldt_operation_signals[NAME_CHANGED], 0);

  g_object_notify (G_OBJECT (object), "name");
}

void
ldt_operation_set_icon_stock (LdtOperation  *object,
                      const gchar *icon_stock)
{
  g_return_if_fail (LDT_IS_OPERATION (object));

  LdtOperationPrivate *private = GET_PRIVATE (object);

  if (! g_strcmp0 (private->icon_stock, icon_stock))
    return;


  private->icon_stock = g_strdup (icon_stock);

  g_signal_emit (object, ldt_operation_signals[ICON_STOCK_CHANGED], 0);

  g_object_notify (G_OBJECT (object), "icon_stock");
}

void
ldt_operation_set_state (LdtOperation  *object,
                      gboolean state)
{
  g_return_if_fail (LDT_IS_OPERATION (object));

  LdtOperationPrivate *private = GET_PRIVATE (object);

  if (private->state == state)
    return;

  private->state = state;

  g_signal_emit (object, ldt_operation_signals[STATE_CHANGED], 0);

  g_object_notify (G_OBJECT (object), "state");
}

const gchar 	*ldt_operation_name (LdtOperation  *object)
{
	return GET_PRIVATE(object)->name;
}

const gchar 	*ldt_operation_icon_stock 	(LdtOperation  *object)
{
	return GET_PRIVATE(object)->icon_stock;
}

gboolean 		ldt_operation_state 			(LdtOperation  *object)
{
	return GET_PRIVATE(object)->state;
}
