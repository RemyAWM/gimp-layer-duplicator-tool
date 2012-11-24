/*
 * ldt_list_view_operations.h
 *
 *  Created on: 07.10.2012
 *      Author: remy
 */

#ifndef LDT_LIST_VIEW_OPERATIONS_H_
#define LDT_LIST_VIEW_OPERATIONS_H_

#include <glib-object.h>

G_BEGIN_DECLS

enum
{
  GO_UP = 0,
  GO_DOWN
};

#define LDT_LIST_VIEW_OPERATIONS_TYPE           	(ldt_list_view_operations_get_type ())
#define LDT_LIST_VIEW_OPERATIONS(obj)            	(G_TYPE_CHECK_INSTANCE_CAST ((obj), LDT_LIST_VIEW_OPERATIONS_TYPE, LdtListViewOperations))
#define LDT_LIST_VIEW_OPERATIONS_CLASS(klass)    	(G_TYPE_CHECK_CLASS_CAST ((klass), LDT_LIST_VIEW_OPERATIONS_TYPE, LdtListViewOperationsClass))
#define LDT_IS_LIST_VIEW_OPERATIONS(obj)         	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LDT_LIST_VIEW_OPERATIONS_TYPE))
#define LDT_IS_LIST_VIEW_OPERATIONS_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass), LDT_LIST_VIEW_OPERATIONS_TYPE))
#define LDT_LIST_VIEW_OPERATIONS_GET_CLASS(obj)  	(G_TYPE_INSTANCE_GET_CLASS ((obj), LDT_LIST_VIEW_OPERATIONS_TYPE, LdtListViewOperationsClass))


typedef struct _LdtListViewOperationsClass LdtListViewOperationsClass;

struct _LdtListViewOperations
{
	GtkTreeView parent_instance;

	LdtTreeModelOperation *store;
    GtkTreeViewColumn *column_op_name;
    GtkTreeViewColumn *column_icon;
    GtkTreeViewColumn *column_count;
    GtkTreeViewColumn *column_state;

    gint32 layer_id;
    const gchar *layer_name;

    GtkTreeIter last_pack_iter;
};

struct _LdtListViewOperationsClass
{
	GtkTreeViewClass parent_class;

};

struct _LdtPackRecord
{
	gchar *pack_name;
	gchar *clone_src;
	gint count;

	GList *ops_list;
};

GType           ldt_list_view_operations_get_type          (void) G_GNUC_CONST;

/*
 * public function declarations
 */

GtkWidget* 			ldt_list_view_operations_new 				(void);

void 				ldt_list_view_operations_append				(LdtListViewOperations *list_view, LdtOperation *op);
void 				ldt_list_view_operations_pack_append		(LdtListViewOperations *list_view);
void 				ldt_list_view_operations_reorder			(LdtListViewOperations *list_view, gint updown);

void 				ldt_list_view_operations_remove				(LdtListViewOperations *list_view, GtkTreeSelection *selection);

GList*				ldt_list_view_operations_list				(LdtListViewOperations *list_view);
gboolean 			ldt_list_view_pack_is_valid						(LdtListViewOperations *list_view, const gchar *pack_name);
G_END_DECLS

#endif /* LDT_LIST_VIEW_OPERATIONS_H_ */
