/* GIMP Plug-in Template
 * Copyright (C) 2000-2004  Michael Natterer <mitch@gimp.org> (the "Author").
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Author of the
 * Software shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the Author.
 */

#include "config.h"

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "main.h"
#include "ldt_list_view_operations.h"
#include "ldt_operation.h"
#include "ldt_operation_rotate.h"
#include "ldt_operation_translate.h"
#include "interface.h"

#include "plugin-intl.h"

/*  Constants  */
#define MAIN_BOX_SPACING        9
#define SECOND_BOX_SPACING      1
#define CONTAINER_BORDER        3
#define ALIGNMENT_PADDING       6
#define ELEMENT_PADDING         1

#define SCALE_WIDTH            120
#define SPIN_BUTTON_WIDTH     	75
#define RANDOM_SEED_WIDTH  		100


 static GtkWidget *list_ops_view = NULL;


/* Private */
void popup_action_menu (GtkMenuToolButton *button, GtkMenu *menu);

static GtkWidget 		*combo_box_cloned_types_new 				(RetultLayerType init_vals);
static GtkWidget 		*combo_box_order_types_new 				(LayerOrderType init_vals);
static GtkWidget 		*combo_box_interpolation_types_new 		(GimpInterpolationType init_vals);

static  void			append_operation_to_list_view 			(GtkWidget *widget, gpointer data);
static  void			append_operation_pack_to_list_view 		(GtkWidget *widget, gpointer data);
static  void			list_view_reorder							(GtkWidget *tool_button, gpointer updown);
static  void			list_view_operation_remove_selected		(GtkWidget *tool_button, gpointer data);

static void 			clone_run 									(GObject *object, gpointer data);

/*  Public functions  */

gboolean
dialog (gint32         image_id,
		gint32 			preview_layer_id,
		gint32 			hidden_image_id,
		RenderFunc 		render_func,
		PlugInVals 		*vals,
		GList			*ops_list,
		const gchar 	*name)
{
  GtkWidget *dlg;
  GtkWidget *main_vbox;
  GtkWidget *box;
  GtkWidget *label;
  GtkWidget *spin;
  GtkWidget *combo;
  GtkWidget *check;
  GtkWidget	*scrolledWindow, *viewport;
  GtkWidget *alignmentExpander;
  GtkWidget *hbox_content;
  GtkWidget *frame_operation_prop, *frame_operation_list;

  GtkObject *adjstment;

  gboolean   run = FALSE;

  gimp_ui_init (PLUGIN_NAME, TRUE);

  dlg = gimp_dialog_new (_(plugin_get_name (name)), PLUGIN_ROLE,
                         	 NULL, 0,
							 gimp_standard_help_func, name,
							 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							 GTK_STOCK_OK,     GTK_RESPONSE_OK,
							 NULL);
  gimp_window_set_transient (GTK_WINDOW (dlg));
  gtk_window_set_keep_above(GTK_WINDOW(dlg),TRUE);
  gtk_window_set_default_size (GTK_WINDOW (dlg), 450, 450);

  main_vbox = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), MAIN_BOX_SPACING);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dlg)->vbox), main_vbox);

  /* Top Control Panel */
  box = gtk_hbox_new(FALSE,SECOND_BOX_SPACING);
  gtk_box_pack_start(GTK_BOX(main_vbox),box,FALSE,FALSE,ELEMENT_PADDING);
  gtk_widget_show (box);

  label = gtk_label_new(_("Count:"));
  gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,ELEMENT_PADDING);
  gtk_widget_show (label);

  adjstment = gtk_adjustment_new(vals->clone_count,1,500,1,3,0);
  spin = gtk_spin_button_new(GTK_ADJUSTMENT(adjstment),0,0);
  gtk_box_pack_start(GTK_BOX(box),spin,TRUE,TRUE,ELEMENT_PADDING);
  gtk_widget_show (spin);

  /* Cloned layers type */
  combo = combo_box_cloned_types_new(vals->result_layer);
  gtk_box_pack_start(GTK_BOX(box),combo,FALSE,FALSE,ELEMENT_PADDING);
  gtk_widget_show (combo);

  /* Order cloned layers */
  combo = combo_box_order_types_new(vals->order_layer);
  gtk_box_pack_start(GTK_BOX(box),combo,FALSE,FALSE,ELEMENT_PADDING);
  gtk_widget_show (combo);

  /* Autocrop */
  check = gtk_check_button_new_with_label(_("crop"));
  gtk_box_pack_start(GTK_BOX(box),check,FALSE,FALSE,ELEMENT_PADDING);
  gtk_widget_show (check);

  /*
   * Main Content Panel
   * ************************* */

  /* Add scrolled_window for expander panels */
  scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(scrolledWindow),
									  GTK_POLICY_AUTOMATIC,
									  GTK_POLICY_AUTOMATIC);
  gtk_widget_show (scrolledWindow);
  gtk_box_pack_start (GTK_BOX(main_vbox), scrolledWindow, TRUE, TRUE, 0);

  /* Add viewport to make  a scroll effect! */
  viewport =  gtk_viewport_new (NULL, NULL);
  gtk_widget_show(viewport);
  gtk_container_add(GTK_CONTAINER(scrolledWindow), viewport);

  alignmentExpander = gtk_alignment_new(0.5,0.5,1,1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(alignmentExpander),
		  	  	  	  	  	  	  ALIGNMENT_PADDING, ALIGNMENT_PADDING,
		  	  	  	  	  	  	  ALIGNMENT_PADDING, ALIGNMENT_PADDING);
  gtk_widget_show(alignmentExpander);
  gtk_container_add(GTK_CONTAINER(viewport), alignmentExpander);

  hbox_content = gtk_hbox_new (FALSE, ELEMENT_PADDING);
  gtk_widget_show(hbox_content);
  gtk_container_add(GTK_CONTAINER(alignmentExpander), hbox_content);
  /*
   * Left Frame for operation options
   * */
  frame_operation_prop =  gtk_frame_new(_("Operation Properties:"));
  gtk_widget_show(frame_operation_prop);
  gtk_box_pack_start(GTK_BOX(hbox_content), frame_operation_prop, TRUE, TRUE, ELEMENT_PADDING);


  /*
   * Right Frame for operation list
   * */
  frame_operation_list =  gtk_frame_new(_("Operation List:"));
  gtk_widget_show(frame_operation_list);
  gtk_box_pack_start(GTK_BOX(hbox_content), frame_operation_list, FALSE, FALSE, ELEMENT_PADDING);
  GtkWidget* vbox_operation_list = gtk_vbox_new(FALSE,ELEMENT_PADDING);
  gtk_widget_show(vbox_operation_list);
  gtk_container_add (GTK_CONTAINER (frame_operation_list), vbox_operation_list);

  /* Add Operation List */
  list_ops_view = ldt_list_view_operations_new();
  gtk_widget_show (list_ops_view);
  gtk_box_pack_start (GTK_BOX (vbox_operation_list), list_ops_view, TRUE, TRUE, 0);

  /* Create operation menu items */
  GtkWidget *menu = gtk_menu_new();
  GtkWidget *menu_item = NULL;

  ops_list = g_list_first(ops_list);
  menu_item = gtk_menu_item_new_with_label(_("Add Pack"));
  gtk_menu_append (GTK_MENU (menu), menu_item);
  g_signal_connect(G_OBJECT(menu_item), "activate",
          G_CALLBACK(append_operation_pack_to_list_view), NULL);
  gtk_widget_show (menu_item);
  while(ops_list != NULL)
  {
	  LdtOperation *ops = ops_list->data;
	  menu_item = gtk_menu_item_new_with_label(ldt_operation_name(ops));
	  gtk_menu_append (GTK_MENU (menu), menu_item);
	  g_signal_connect(G_OBJECT(menu_item), "activate",
	          G_CALLBACK(append_operation_to_list_view), ops);
	  gtk_widget_show (menu_item);
	  ops_list = ops_list->next;
  }


  /* Create toolbar */
  GtkWidget *toolbar = gtk_toolbar_new();
  gtk_toolbar_set_show_arrow(GTK_TOOLBAR(toolbar),FALSE);
  gtk_widget_show (toolbar);
  gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
  gtk_container_set_border_width(GTK_CONTAINER(toolbar), 1);
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar), GTK_ICON_SIZE_MENU);
  gtk_box_pack_start (GTK_BOX (vbox_operation_list), toolbar, FALSE, FALSE, 0);

  GtkToolItem *menubutton = gtk_menu_tool_button_new_from_stock (GTK_STOCK_ADD);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), menubutton, -1);
  gtk_widget_show (GTK_WIDGET(menubutton));
  gtk_menu_tool_button_set_menu(GTK_MENU_TOOL_BUTTON(menubutton), menu);
  g_signal_connect(GTK_MENU_TOOL_BUTTON(menubutton),  "clicked", G_CALLBACK(popup_action_menu), menu);

  GtkToolItem *action_bar_sep = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), action_bar_sep, -1);
  gtk_widget_show (GTK_WIDGET(action_bar_sep));

  menubutton = gtk_tool_button_new_from_stock (GTK_STOCK_GO_UP);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), menubutton, -1);
  gtk_widget_show (GTK_WIDGET(menubutton));
  g_signal_connect(GTK_TOOL_BUTTON(menubutton),  "clicked",
		  G_CALLBACK(list_view_reorder), GINT_TO_POINTER (GO_UP));

  menubutton = gtk_tool_button_new_from_stock (GTK_STOCK_GO_DOWN);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), menubutton, -1);
  gtk_widget_show (GTK_WIDGET(menubutton));
  g_signal_connect(GTK_TOOL_BUTTON(menubutton),  "clicked",
		  G_CALLBACK(list_view_reorder), GINT_TO_POINTER (GO_DOWN));

  action_bar_sep = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), action_bar_sep, -1);
  gtk_widget_show (GTK_WIDGET(action_bar_sep));

  menubutton = gtk_tool_button_new_from_stock (GTK_STOCK_REMOVE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), menubutton, -1);
  gtk_widget_show (GTK_WIDGET(menubutton));
  g_signal_connect(GTK_TOOL_BUTTON(menubutton),  "clicked",
		  G_CALLBACK(list_view_operation_remove_selected), list_ops_view);

  /*
   * Additional Option Panel
   * *********************** */
  GtkWidget* ep_add_options = gtk_expander_new(_("<span foreground='#00438A'><b>Additional options:</b></span>"));
  gtk_expander_set_use_markup(GTK_EXPANDER(ep_add_options), TRUE);
  gtk_widget_show(ep_add_options);
  gtk_box_pack_start (GTK_BOX(main_vbox), ep_add_options, FALSE, FALSE, ELEMENT_PADDING);
  GtkWidget* alignment_add_options = gtk_alignment_new(0.5,0.5,1,1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_add_options),
		  	  	  	  	  	  	  0,0,
		  	  	  	  	  	  	  ALIGNMENT_PADDING, ALIGNMENT_PADDING);
  gtk_widget_show(alignment_add_options);
  gtk_container_add(GTK_CONTAINER(ep_add_options), alignment_add_options);
  GtkWidget* vbox_add_options = gtk_vbox_new(FALSE,ELEMENT_PADDING);
  gtk_widget_show(vbox_add_options);
  gtk_container_add(GTK_CONTAINER(alignment_add_options), vbox_add_options);

  /* Interpolation type  */
  GtkWidget* hbox_ao_interpolation = gtk_hbox_new (FALSE, ELEMENT_PADDING);
  gtk_widget_show (hbox_ao_interpolation);
  gtk_box_pack_start (GTK_BOX(vbox_add_options), hbox_ao_interpolation, FALSE, FALSE, ELEMENT_PADDING);
  label = gtk_label_new(_("Interpolation:"));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX(hbox_ao_interpolation), label, FALSE, FALSE, ELEMENT_PADDING);
  combo = combo_box_interpolation_types_new (vals->interpolation);
  gtk_widget_show (combo);
  gtk_box_pack_start (GTK_BOX(hbox_ao_interpolation), combo, TRUE, TRUE, ELEMENT_PADDING);

  /*
   * Buttons Panel
   * *********************** */
  GtkWidget* hseparator = gtk_hseparator_new();
  gtk_widget_show (hseparator);
  gtk_box_pack_start (GTK_BOX(main_vbox), hseparator, FALSE, FALSE, 0);

  GtkWidget* hbox_buttons = gtk_hbox_new(FALSE, ELEMENT_PADDING);
  gtk_widget_show (hbox_buttons);
  gtk_box_pack_start (GTK_BOX(main_vbox), hbox_buttons, FALSE, TRUE, ELEMENT_PADDING);

  GtkWidget *imgResetVals = gtk_image_new_from_stock (GIMP_STOCK_RESET, GTK_ICON_SIZE_BUTTON);
  GtkWidget *btResetVals = gtk_button_new_with_label(_("Reset Vals"));
  gtk_button_set_image (GTK_BUTTON(btResetVals), imgResetVals);
  gtk_button_set_use_stock(GTK_BUTTON(btResetVals),TRUE);

  GtkWidget *btUndo = gtk_button_new_with_label(_("Undo"));
  GtkWidget *imgUndo = gtk_image_new_from_stock (GTK_STOCK_UNDO, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image (GTK_BUTTON (btUndo), imgUndo);

  GtkWidget *btClone = gtk_button_new_with_label(_("Clone"));
  GtkWidget *imgClone = gtk_image_new_from_stock (GTK_STOCK_YES, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image (GTK_BUTTON (btClone), imgClone);
  g_signal_connect (btClone, "clicked",
                      G_CALLBACK (clone_run), NULL);

  GtkWidget *btAbout = gtk_button_new_with_label(_("About"));
  GtkWidget *imgAbout = gtk_image_new_from_stock (GTK_STOCK_ABOUT, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image (GTK_BUTTON (btAbout), imgAbout);

  gtk_widget_show(btResetVals);
  gtk_widget_show(imgResetVals);

  gtk_widget_show(btUndo);
  gtk_widget_show(imgUndo);

  gtk_widget_show(btClone);
  gtk_widget_show(imgClone);

  gtk_widget_show(btAbout);
  gtk_widget_show(imgAbout);

  gtk_box_pack_end(GTK_BOX(hbox_buttons), btClone, FALSE, TRUE, ELEMENT_PADDING);
  gtk_box_pack_end(GTK_BOX(hbox_buttons), btUndo, FALSE, TRUE, ELEMENT_PADDING);
  gtk_box_pack_end(GTK_BOX(hbox_buttons), btResetVals, FALSE, TRUE, ELEMENT_PADDING);
  gtk_box_pack_start(GTK_BOX(hbox_buttons), btAbout, FALSE, TRUE, ELEMENT_PADDING);


  hseparator = gtk_hseparator_new();
  gtk_widget_show (hseparator);
  gtk_box_pack_start (GTK_BOX(main_vbox), hseparator, FALSE, FALSE, 0);
  /*  Show the main containers  */

  gtk_widget_show (main_vbox);
  gtk_widget_show (dlg);

  run = (gimp_dialog_run (GIMP_DIALOG (dlg)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dlg);

  return run;
}

static GtkWidget *combo_box_cloned_types_new (RetultLayerType init_vals)
{
	GtkWidget *combo = NULL;
	GtkCellRenderer		*cell = NULL;
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GtkTreeIter *active_iter = NULL;

	store = gtk_list_store_new ( 2, G_TYPE_INT, G_TYPE_STRING );

	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, MERGE_LAYERS, 1, _("Merge"), -1 );
	if(init_vals == (gint)MERGE_LAYERS)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, GROUPING_LAYERS, 1, _("Group"), -1 );
	if(init_vals == (gint)GROUPING_LAYERS)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, EACH_LAYERS, 1, _("In Each"), -1 );
	if(init_vals == (gint)EACH_LAYERS)
		active_iter = &iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL( store ));
	g_object_unref(G_OBJECT( store ));

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(combo), cell, TRUE);
	gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( combo ), cell, "text", 1, NULL );

	if(active_iter)
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),active_iter);

	return combo;
}

static GtkWidget *combo_box_order_types_new (LayerOrderType init_vals)
{
	GtkWidget *combo = NULL;
	GtkCellRenderer		*cell = NULL;
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GtkTreeIter *active_iter = &iter;

	store = gtk_list_store_new ( 2, G_TYPE_INT, G_TYPE_STRING );

	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, UP_DOWN_ORDER, 1, _("UP"), -1 );
	if(init_vals == (gint)UP_DOWN_ORDER)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, DOWN_UP_ORDER, 1, _("DOWN"), -1 );
	if(init_vals == (gint)DOWN_UP_ORDER)
		active_iter = &iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL( store ));
	g_object_unref(G_OBJECT( store ));

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(combo), cell, TRUE);
	gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( combo ), cell, "text", 0, NULL );

	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),active_iter);

	return combo;
}

static GtkWidget *combo_box_interpolation_types_new (GimpInterpolationType init_vals)
{
	GtkWidget *combo = NULL;
	GtkCellRenderer		*cell = NULL;
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GtkTreeIter *active_iter = &iter;

	store = gtk_list_store_new ( 2, G_TYPE_INT, G_TYPE_STRING );

	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, GIMP_INTERPOLATION_NONE, 1, _("None"), -1 );
	if(init_vals == (gint)GIMP_INTERPOLATION_NONE)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, GIMP_INTERPOLATION_LINEAR, 1, _("Linear"), -1 );
	if(init_vals == (gint)GIMP_INTERPOLATION_LINEAR)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, GIMP_INTERPOLATION_CUBIC, 1, _("Cubic"), -1 );
	if(init_vals == (gint)GIMP_INTERPOLATION_CUBIC)
		active_iter = &iter;
	gtk_list_store_append( store, &iter );
	gtk_list_store_set( store, &iter, 0, GIMP_INTERPOLATION_LANCZOS, 1, _("Lanczos"), -1 );
	if(init_vals == (gint)GIMP_INTERPOLATION_LANCZOS)
		active_iter = &iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL( store ));
	g_object_unref(G_OBJECT( store ));

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(combo), cell, TRUE);
	gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( combo ), cell, "text", 1, NULL );

	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),active_iter);

	return combo;
}

void popup_action_menu (GtkMenuToolButton *button, GtkMenu *menu)
{
	gtk_menu_popup(menu, NULL, NULL, NULL, NULL, 1, gtk_get_current_event_time());
}

static  void	append_operation_pack_to_list_view (GtkWidget *widget, gpointer data)
{
	ldt_list_view_operations_pack_append(LDT_LIST_VIEW_OPERATIONS (list_ops_view));
}

static  void	append_operation_to_list_view (GtkWidget *widget, gpointer data)
{
	ldt_list_view_operations_append (LDT_LIST_VIEW_OPERATIONS (list_ops_view), data);
}


static  void
list_view_reorder	(GtkWidget *tool_button, gpointer updown)
{
	g_return_if_fail(list_ops_view);

	ldt_list_view_operations_reorder (
			LDT_LIST_VIEW_OPERATIONS (list_ops_view),
			GPOINTER_TO_INT (updown));
}

static  void
list_view_operation_remove_selected	 (GtkWidget *tool_button, gpointer data)
{
	  GtkTreeView *treeview = (GtkTreeView *)data;
	  GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

	  ldt_list_view_operations_remove (LDT_LIST_VIEW_OPERATIONS(treeview),selection);
}

/*
 * Выполняем клоирование заданное в списке операций  выводит резуьтат в предосмотр
 *
 * */
static void clone_run (GObject *object, gpointer data)
{
	GList *list = NULL;
	g_return_if_fail(list_ops_view);

	/*Обрабатываем доступные для обработки пакеты трансформаци */
	for (list = ldt_list_view_operations_list (LDT_LIST_VIEW_OPERATIONS (list_ops_view));
			list; list = list->next)
	{
		LdtPackRecord *pack_rec = (LdtPackRecord*)list->data;

		/* проверяем валидность параметров пакета */
		if (!ldt_list_view_pack_is_valid(LDT_LIST_VIEW_OPERATIONS (list_ops_view), pack_rec->pack_name))
		{
			printf("Pack: %s isn't valid :((\n", pack_rec->pack_name);
			return;
		}
		printf("Pack: %s\n", pack_rec->pack_name);


		/* получаем список операций пакета и выполняем их */
		GList *ops_list = NULL;
		for (ops_list = g_list_first(pack_rec->ops_list); ops_list; ops_list = ops_list->next)
		{
			LdtOperation *op_obj = ops_list->data;

			printf("\t-> %s\n",ldt_operation_name(op_obj));
		}
	}

}
