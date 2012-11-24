/*
 * ldt_tree_model_operations.c
 *
 *  Created on: 19.10.2012
 *      Author: helen
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimpbase/gimpbase.h>
#include <libgimp/gimpui.h>

#include "gtk/gtktreestore.h"

#include "types.h"

#include "ldt_tree_model_operations.h"

#include "plugin-intl.h"


static void       ldt_tree_model_operation_class_init    		(LdtTreeModelOperationClass	*klass);
static void       ldt_tree_model_operation_init           		(LdtTreeModelOperation		*object);
static void       ldt_tree_model_operation_finalize         	(GObject         			*object);

static void       ldt_tree_model_operation_drag_source_init	(GtkTreeDragSourceIface 	*iface);
static gboolean   ldt_tree_model_operation_drag_data_get 		(GtkTreeDragSource   		*drag_source,
																	 GtkTreePath         		*path,
																	 GtkSelectionData    		*selection_data);

static void       ldt_tree_model_operation_drag_dest_init		(GtkTreeDragDestIface		*iface);
static gboolean   ldt_tree_model_operation_row_drop_possible 	(GtkTreeDragDest   			*drag_dest,
																     GtkTreePath       			*dest_path,
																     GtkSelectionData  			*selection_data);

#define GTK_TREE_STORE_IS_SORTED(tree) (((GtkTreeStore*)(tree))->sort_column_id != GTK_TREE_SORTABLE_UNSORTED_SORT_COLUMN_ID)

G_DEFINE_TYPE_WITH_CODE (LdtTreeModelOperation, ldt_tree_model_operation, GTK_TYPE_TREE_STORE,
		G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_DRAG_SOURCE,
								ldt_tree_model_operation_drag_source_init)
		G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_DRAG_DEST,
								ldt_tree_model_operation_drag_dest_init))

#define parent_class ldt_tree_model_operation_parent_class

static void
ldt_tree_model_operation_class_init (LdtTreeModelOperationClass *klass)
{
	  GObjectClass *object_class 	= 	G_OBJECT_CLASS (klass);

	  object_class->finalize 		= 	ldt_tree_model_operation_finalize;
}

static void
ldt_tree_model_operation_init (LdtTreeModelOperation *tree_model)
{
}

static void
ldt_tree_model_operation_finalize (GObject *object)
{
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
ldt_tree_model_operation_drag_source_init (GtkTreeDragSourceIface 	*iface)
{
	iface->drag_data_get = ldt_tree_model_operation_drag_data_get;
}

static gboolean
ldt_tree_model_operation_drag_data_get (GtkTreeDragSource   *drag_source,
											 GtkTreePath         *path,
											 GtkSelectionData    *selection_data)
{
  if (gtk_tree_set_row_drag_data (selection_data,
				  GTK_TREE_MODEL (drag_source),
				  path))
	{
	  return TRUE;
	}
  else
	{
	  /* FIXME handle text targets at least. */
	}

  return FALSE;
}

static void
ldt_tree_model_operation_drag_dest_init (GtkTreeDragDestIface		*iface)
{
	iface->row_drop_possible = ldt_tree_model_operation_row_drop_possible;
}

static gboolean
ldt_tree_model_operation_row_drop_possible (GtkTreeDragDest   *drag_dest,
												  GtkTreePath       *dest_path,
												  GtkSelectionData  *selection_data)
{
	GtkTreeModel 	*src_model 	= NULL;
	GtkTreePath 	*src_path 	= NULL;
	GtkTreePath 	*tmp 		= NULL;
	gboolean 		is_dest_pack= FALSE,
					is_src_pack	= FALSE,
					is_dest		= FALSE,
					id_src		= FALSE,
					toplevel	= FALSE;

	GtkTreeIter 	dest_iter, src_iter,dest_iter_parent;
	gboolean 		retval 		= FALSE;


	/* don't accept drops if the tree has been sorted */
	if (GTK_TREE_STORE_IS_SORTED (drag_dest))
		return FALSE;

	/* get data row */
	if (!gtk_tree_get_row_drag_data (selection_data,
										   &src_model,
										   &src_path))
		goto out;

	/* can only drag to ourselves */
	if (src_model != GTK_TREE_MODEL (drag_dest))
		goto out;

	/* Can't drop into ourself. */
	if (gtk_tree_path_is_ancestor (src_path,
							 dest_path))
		goto out;


	is_dest = gtk_tree_model_get_iter (src_model, &dest_iter, dest_path);
	id_src 	= gtk_tree_model_get_iter (src_model, &src_iter, src_path);

	if (is_dest && id_src)
	{
		gtk_tree_model_get (src_model, &src_iter, COL_SHOWS, &is_src_pack, -1);
		gtk_tree_model_get (src_model, &dest_iter, COL_SHOWS, &is_dest_pack, -1);
		toplevel = !gtk_tree_model_iter_parent (src_model,&dest_iter_parent,&dest_iter);
	}

	if(is_src_pack && (!is_dest_pack || !toplevel))
		goto out;

	if(!is_src_pack)
	{
		if(!is_dest || (is_dest && is_dest_pack && toplevel))
			goto out;
	}

	/* Can otherwise drop anywhere. */
	retval = TRUE;

	out:

	if (src_path)
		gtk_tree_path_free (src_path);

	if (tmp)
		gtk_tree_path_free (tmp);

	return retval;
}

/*
 * Public functions
 * */

LdtTreeModelOperation*
ldt_tree_model_operation_new (gint n_columns, ...)
{
	LdtTreeModelOperation *retval;
	va_list args;
	gint i;
	GType type[n_columns];

	g_return_val_if_fail (n_columns > 0, NULL);

	retval = g_object_new (LDT_TREE_MODEL_OPERATION_TYPE, NULL);

	va_start (args, n_columns);
	for (i = 0; i < n_columns; i++)
	{
	  type[i] = va_arg (args, GType);
	}
	va_end (args);
	
	gtk_tree_store_set_column_types (GTK_TREE_STORE (retval), n_columns, type);

	return retval;
}

