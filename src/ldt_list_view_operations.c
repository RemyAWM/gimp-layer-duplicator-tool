/*
 * ldt_list_view_operations.c
 *
 *  Created on: 07.10.2012
 *      Author: remy
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimpbase/gimpbase.h>
#include <libgimp/gimpui.h>

#include "types.h"

#include "ldt_tree_model_operations.h"
#include "ldt_list_view_operations.h"
#include "ldt_operation.h"

#include "plugin-intl.h"

enum
{
  	LAST_SIGNAL
};

/*
 * список свойств
 */
enum
{
	PROP_0,
};

/*
 * Структура приватного объекта.
 */


typedef struct _LdtListViewOperationsPrivate LdtListViewOperationsPrivate;

struct _LdtListViewOperationsPrivate
{
	gchar *name;
};

#define GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE (obj, \
                                                       LDT_LIST_VIEW_OPERATIONS_TYPE, \
                                                       LdtListViewOperationsPrivate)


static void    ldt_list_view_operations_class_init       (LdtListViewOperationsClass 		*klass);

static void    ldt_list_view_operations_init             (LdtListViewOperations      		*object);

static void    ldt_list_view_operations_constructed     (GObject         *object);

static void    ldt_list_view_operations_finalize         (GObject         *object);

static void    ldt_list_view_operations_set_property     (GObject          *object,
																 guint        		property_id,
																 const GValue  		*value,
																 GParamSpec     	*pspec);

static void    ldt_list_view_operations_get_property     (GObject            	*object,
																 guint              property_id,
																 GValue             *value,
																 GParamSpec         *pspec);


static void 	ldt_list_view_operations_state_toggled		(GtkCellRendererToggle 	*cell,
																gchar                 	*path_str,
																gpointer               	data);

static GtkTreeIter ldt_list_view_operations_last_pack_iter (LdtListViewOperations *list_view);

G_DEFINE_TYPE (LdtListViewOperations, ldt_list_view_operations, GTK_TYPE_TREE_VIEW)

/*
 * Сылка на родительский класс. Измените на нужный вам класс:
 */
#define parent_class ldt_list_view_operations_parent_class

//static guint ldt_list_view_operations_signals[LAST_SIGNAL] = { 0 };

static gint _itr = 0;

static void
ldt_list_view_operations_class_init (LdtListViewOperationsClass *klass)
{
	GObjectClass      	*object_class      	= G_OBJECT_CLASS (klass);

	object_class->constructed	=	ldt_list_view_operations_constructed;
	object_class->finalize		= 	ldt_list_view_operations_finalize;
	object_class->set_property 	= 	ldt_list_view_operations_set_property;
	object_class->get_property 	= 	ldt_list_view_operations_get_property;

	g_type_class_add_private (klass, sizeof (LdtListViewOperationsPrivate));
}

static void
ldt_list_view_operations_init (LdtListViewOperations *object)
{
	LdtListViewOperationsPrivate *private = GET_PRIVATE (object);
	private->name = NULL;
	object->layer_id = -1;
	object->layer_name = "None";
	object->store = NULL;
}

static void
ldt_list_view_operations_constructed (GObject         *object)
{
	LdtListViewOperations			*list_view	=	LDT_LIST_VIEW_OPERATIONS(object);
	GtkTreeView						*tree_view	=	GTK_TREE_VIEW (list_view);
	GtkCellRenderer 				*renderer 	=	NULL;

	/*Sets up widget list */
	//gtk_tree_view_set_headers_visible (tree_view, FALSE );
	gtk_tree_view_set_reorderable (tree_view, TRUE );
	gtk_tree_view_expand_all (tree_view);

	/* Create model */
    list_view->store = ldt_tree_model_operation_new (NUM_COLS,
    		GDK_TYPE_PIXBUF,
    		G_TYPE_STRING,
    		G_TYPE_STRING,
    		GTK_TYPE_INT,
    		G_TYPE_BOOLEAN,
    		G_TYPE_OBJECT,
    		G_TYPE_BOOLEAN);

    gtk_tree_view_set_model(GTK_TREE_VIEW (list_view), GTK_TREE_MODEL (list_view->store));

    /* Cteate column */
    list_view->column_icon = gtk_tree_view_column_new ();
    list_view->column_op_name = gtk_tree_view_column_new ();
    list_view->column_count = gtk_tree_view_column_new ();
    list_view->column_state = gtk_tree_view_column_new ();

    renderer = gtk_cell_renderer_pixbuf_new ();
    g_object_set (renderer, "xalign", 0.0, NULL);
    gtk_tree_view_column_pack_start(list_view->column_icon, renderer, FALSE);
    gtk_tree_view_column_set_attributes (list_view->column_icon,
                                                         renderer,
                                                         "pixbuf",
                                                         COL_ICON,
                                                         NULL);
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_column_pack_start(list_view->column_op_name, renderer, TRUE);
    gtk_tree_view_column_set_attributes (list_view->column_op_name,
                                                         renderer,
                                                         "text",
                                                         COL_NAME,
                                                         NULL);
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_column_pack_start(list_view->column_op_name, renderer, TRUE);
    gtk_tree_view_column_set_attributes (list_view->column_op_name,
                                                         renderer,
                                                         "text",
                                                         COL_RENDER_SRC,
                                                         "visible",
                                                         COL_SHOWS,
                                                         NULL);

    GtkObject *adj = gtk_adjustment_new (10.0, 2.0, 100.0, 1.0, 2.0, 0);
    renderer = gtk_cell_renderer_spin_new ();
    g_object_set (renderer,
    				"editable", TRUE,
    				"digits", 1,
    				"adjustment",
    				 GTK_ADJUSTMENT (adj),
    				NULL);
    gtk_tree_view_column_pack_start(list_view->column_count, renderer, TRUE);
    gtk_tree_view_column_set_attributes (list_view->column_count,
                                                         renderer,
                                                         "text",
                                                         COL_COUNT,
                                                         NULL);
    renderer = gtk_cell_renderer_toggle_new ();
    gtk_tree_view_column_pack_start(list_view->column_state, renderer, TRUE);
    gtk_tree_view_column_set_attributes (list_view->column_state,
                                                         renderer,
                                                         "active",
                                                         COL_TOGGLE,
                                                         NULL);
    gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (list_view->column_state),
    				   	   	   	   	   	   	   	   	   	   	   GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (list_view->column_state), 50);
    gtk_tree_view_column_set_clickable (GTK_TREE_VIEW_COLUMN (list_view->column_state), TRUE);

    g_signal_connect (renderer, "toggled",
                      G_CALLBACK (ldt_list_view_operations_state_toggled), list_view->store);

    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), list_view->column_icon);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), list_view->column_op_name);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), list_view->column_count);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), list_view->column_state);

    G_OBJECT_CLASS (parent_class)->constructed (object);
}

static void
ldt_list_view_operations_finalize (GObject *object)
{
	LdtListViewOperations *list_view = LDT_LIST_VIEW_OPERATIONS(object);

	gtk_tree_store_clear (GTK_TREE_STORE (list_view->store));
	g_object_unref (list_view->store);

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
ldt_list_view_operations_set_property (GObject *object,
                       	   guint 		property_id,
                       	   const 		GValue *value,
                       	   GParamSpec 	*pspec)
{
	  switch (property_id)
	    {
	    default:
	      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	      break;
	    }
}

static void
ldt_list_view_operations_get_property (GObject *object,
                           guint         property_id,
                           GValue        *value,
                           GParamSpec    *pspec)
{
	  switch (property_id)
	    {
	    default:
	      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	      break;
	    }
}


/* Public functions */

GtkWidget*
ldt_list_view_operations_new()
{
	  return g_object_new (LDT_LIST_VIEW_OPERATIONS_TYPE,NULL);
}

void
ldt_list_view_operations_pack_append	(LdtListViewOperations *list_view)
{
	GtkTreeIter iter;
	GtkWidget *image = gtk_image_new ();
    GdkPixbuf *pixbuf = gtk_widget_render_icon (image,
    												GIMP_STOCK_LAYERS,
													GTK_ICON_SIZE_LARGE_TOOLBAR, NULL);

    /*
     * Добавляем пакет операций - всегда в конец списка и вверх иерархии!
     * (пакеты не могут быть вложенными!)
     * */

    gtk_tree_store_append (GTK_TREE_STORE (list_view->store), &iter,NULL);
    gtk_tree_store_set (GTK_TREE_STORE (list_view->store), &iter,
    						COL_ICON,
    						pixbuf,
    						COL_NAME,
    						g_strdup_printf("P:%i",_itr++),
    						COL_RENDER_SRC,
    						list_view->layer_name,
    						COL_COUNT,
    						12,
    						COL_TOGGLE,
    						TRUE,
    						COL_OBJECT,
    						NULL,
    						COL_SHOWS,
    						TRUE,
    						-1);
    list_view->last_pack_iter = iter;
}

static GtkTreeIter ldt_list_view_operations_last_pack_iter (LdtListViewOperations *list_view)
{
	return list_view->last_pack_iter;
}

void
ldt_list_view_operations_append	(LdtListViewOperations *list_view, LdtOperation *op)
{
	g_return_if_fail(LDT_IS_LIST_VIEW_OPERATIONS (list_view));

	GtkTreeView *treeview = GTK_TREE_VIEW (list_view);
	GtkTreeModel *model = gtk_tree_view_get_model (treeview);
	GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);


	GtkTreeIter iter;
	GtkTreeIter iter_pack;

	GtkWidget *image = gtk_image_new ();
	GdkPixbuf *pixbuf = gtk_widget_render_icon (image,
													ldt_operation_icon_stock(op),
													GTK_ICON_SIZE_SMALL_TOOLBAR, NULL);

	    /*
	     * Добавляем операцию.
	     * Если что-то выделенно и это пакет, тогда добавить в него операцию,
	     * иначе если выделенна операция, тогда получить родителя и добавить в него операцию.
	     * Иначе если нет выделения, то
	     * Если модель пустая - добавить пакет и в него операцию
	     * Иначе (есть модель) - получить последний в списке пакет и добавить в него операцию.
	     * */

	    if (gtk_tree_selection_get_selected (selection, &model, &iter))
	    {
	    	gboolean is_pack;
	    	GtkTreePath *path = gtk_tree_model_get_path (model,&iter);
			gtk_tree_model_get_iter (model, &iter, path);
			gtk_tree_model_get (model, &iter, COL_SHOWS, &is_pack, -1);

			if(is_pack)
			{
				GtkTreeIter iter_tmp;
				gtk_tree_store_append (GTK_TREE_STORE (list_view->store), &iter_tmp,&iter);
				iter_pack = iter;
				iter = iter_tmp;
			}
			else
			{
				gtk_tree_model_iter_parent (model,&iter_pack,&iter);
				gtk_tree_store_append (GTK_TREE_STORE (list_view->store), &iter,&iter_pack);
			}
	    }
	    else
	    {
	    	gboolean has_items =  gtk_tree_model_get_iter_first(model, &iter);
	    	if(has_items)
	    	{
	    		gboolean has_childe;
		    	GtkTreePath *path = gtk_tree_model_get_path (model,&iter);
				gtk_tree_model_get_iter (model, &iter, path);
				gtk_tree_model_get (model, &iter, COL_SHOWS, &has_childe, -1);
		    	if(has_childe)
		    	{
		    		GtkTreeIter iter_last = iter;
		    		while (has_items)
		    		{
		    			has_items = gtk_tree_model_iter_next(model,&iter);
		    			if(has_items)
		    			{
		    		    	path = gtk_tree_model_get_path (model,&iter);
		    				gtk_tree_model_get_iter (model, &iter, path);
		    				gtk_tree_model_get (model, &iter, COL_SHOWS, &has_childe, -1);
		    				if(has_childe)
		    					iter_last = iter;
		    			}
		    		}

					gtk_tree_store_append (GTK_TREE_STORE (list_view->store), &iter,&iter_last);
					iter_pack = iter_last;
		    	}
		    	else
		    	{
		    		printf ("It is item!\n");
		    		return;
		    	}
	    	}
	    	else
	    	{
	    		ldt_list_view_operations_pack_append (list_view);
	    		iter_pack = ldt_list_view_operations_last_pack_iter(list_view);
	    		gtk_tree_store_append (GTK_TREE_STORE (list_view->store), &iter,&iter_pack);
	    	}
	    }

		    gtk_tree_store_set (GTK_TREE_STORE (list_view->store), &iter,
		    						COL_ICON,
		    						pixbuf,
		    						COL_NAME,
		    						ldt_operation_name(op),
		    						COL_RENDER_SRC,
		    						NULL,
		    						COL_COUNT,
		    						12,
		    						COL_TOGGLE,
		    						ldt_operation_state(op),
		    						COL_OBJECT,
		    						op,
		    						COL_SHOWS,
		    						FALSE,
		    						-1);
}

void
ldt_list_view_operations_reorder (LdtListViewOperations *list_view, gint updown)
{
	g_return_if_fail(LDT_IS_LIST_VIEW_OPERATIONS (list_view));

	GtkTreeView *treeview = GTK_TREE_VIEW (list_view);

	gboolean valid = FALSE;
	GtkTreeIter iter;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
	  GtkTreePath *path = gtk_tree_model_get_path(model,&iter);
	  GtkTreeIter iter_2;

	  if(updown == GO_UP)
		  gtk_tree_path_prev (path);
	  else if(updown == GO_DOWN)
		  gtk_tree_path_next (path);
	  else
	  {
		  gtk_tree_path_free (path);
		  return;
	  }

	  valid = gtk_tree_model_get_iter (model, &iter_2, path);

	  if(valid)
	  {
		  	if(updown == 0)
		  		gtk_tree_store_move_before(GTK_TREE_STORE(model), &iter, &iter_2);
		  	else
		  		gtk_tree_store_move_after(GTK_TREE_STORE(model), &iter, &iter_2);
	  }
	  else
		  gtk_tree_path_free (path);
	}
}

void
ldt_list_view_operations_remove	(LdtListViewOperations *list_view, GtkTreeSelection *selection)
{
	g_return_if_fail(LDT_LIST_VIEW_OPERATIONS (list_view));
	g_return_if_fail(GTK_TREE_SELECTION (selection));

	  GtkTreeIter iter;
	  GtkTreeView *treeview = (GtkTreeView *)list_view;
	  GtkTreeModel *model = gtk_tree_view_get_model (treeview);

	  if (gtk_tree_selection_get_selected (selection, &model, &iter))
	    {
	      GtkTreePath *path;
	      path = gtk_tree_model_get_path (model, &iter);
	      gtk_tree_store_remove (GTK_TREE_STORE (model), &iter);
	      gtk_tree_path_free (path);
	    }
}

GList*
ldt_list_view_operations_list (LdtListViewOperations *list_view)
{
	/*
	 * Возвращает список всех пакетов и входящих в них операций
	 * */
	GList *list = NULL;
	GtkTreeView *treeview = GTK_TREE_VIEW (list_view);
	GtkTreeModel *model = gtk_tree_view_get_model (treeview);
	GtkTreeIter iter_pack;

	gboolean iter_pack_next =  gtk_tree_model_get_iter_first(model, &iter_pack);

	while (iter_pack_next)
	{
		gboolean state;
		gboolean is_pack;
		gchar *pack_name;
		gchar *clone_src;
		gint count;
		GtkTreePath *path_pack = gtk_tree_model_get_path (model,&iter_pack);

		gtk_tree_model_get_iter (model, &iter_pack, path_pack);
		gtk_tree_model_get (model, &iter_pack, COL_TOGGLE, &state, -1);
		gtk_tree_model_get (model, &iter_pack, COL_SHOWS, &is_pack, -1);
		gtk_tree_model_get (model, &iter_pack, COL_NAME, &pack_name, -1);
		gtk_tree_model_get (model, &iter_pack, COL_RENDER_SRC, &clone_src, -1);
		gtk_tree_model_get (model, &iter_pack, COL_COUNT, &count, -1);

		if(state && is_pack)
		{
			GList *ops_list = NULL;
			GtkTreeIter iter_op;
			gboolean has_children = gtk_tree_model_iter_children (model,&iter_op,&iter_pack);
			gboolean op_found = FALSE;

			while (has_children)
			{
				gboolean op_state = FALSE;
				GObject *ldt_op=NULL;
				GtkTreePath *path_op = gtk_tree_model_get_path (model,&iter_op);

				gtk_tree_model_get_iter (model, &iter_op, path_op);
				gtk_tree_model_get (model, &iter_op, COL_OBJECT, &ldt_op, -1);
				gtk_tree_model_get (model, &iter_op, COL_TOGGLE, &op_state, -1);

				if(op_state){
					ops_list = g_list_append(ops_list, ldt_op);
					op_found=TRUE;
				}
				gtk_tree_path_free (path_op);
				has_children = gtk_tree_model_iter_next(model,&iter_op);
			}

			if (op_found)
			{
				LdtPackRecord *pack_rec = g_new(LdtPackRecord, 1);
				pack_rec->pack_name = g_strdup(pack_name);
				pack_rec->clone_src = g_strdup(clone_src);
				pack_rec->count = count;
				pack_rec->ops_list = ops_list;

				list = g_list_append(list, pack_rec);
			}

		}

		gtk_tree_path_free (path_pack);
		iter_pack_next = gtk_tree_model_iter_next(model,&iter_pack);
	}

	return list;
}


static void
ldt_list_view_operations_state_toggled	(GtkCellRendererToggle 	*cell,
											gchar                 	*path_str,
											gpointer               	 data)
{
	GtkTreeModel *model = (GtkTreeModel *)data;
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean state;

	/* get toggled iter */
	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, COL_TOGGLE, &state, -1);

	/* do something with the value */
	state ^= 1;

	/* set new value */
	gtk_tree_store_set (GTK_TREE_STORE (model), &iter, COL_TOGGLE, state, -1);

	/* clean up */
	gtk_tree_path_free (path);
}

/*
 * проверяем правильность параметров пакета -
 *     мин. кол-во клонов > 0
 *     источник для трансформации:
 *        если это слой - валидный ли он?
 *        если пакет или составвной источник - доступен ли пакет или
 *                               пакеты (если составной) для использования в данном пакете?
 *                               (пакеты должны быть перечисленны в вышестоящих пакетах!)
 * */
gboolean
ldt_list_view_pack_is_valid	(LdtListViewOperations 	*list_view,
								 const gchar 			*pack_name)
{
	g_return_val_if_fail(LDT_LIST_VIEW_OPERATIONS (list_view), FALSE);



	return FALSE;
}
