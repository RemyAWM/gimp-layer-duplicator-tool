/* GIMP Plug-in Template
 * Copyright (C) 2000  Michael Natterer <mitch@gimp.org> (the "Author").
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

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimpbase/gimpbase.h>
#include <libgimp/gimpui.h>

#include "main.h"

#include "live-preview.h"
#include "hidden-image.h"
#include "state-image.h"

#include "interface.h"
#include "render.h"

#include "ldt_operation.h"
#include "ldt_operation_rotate.h"
#include "ldt_operation_translate.h"


#include "plugin-intl.h"


/*  Constants  */

#define PLUGIN_BINARY                    "gimp_plugin_ldt"
#define PLUGIN_ROLE                      "plugin-reole-ldt"
#define AUTHOR                           "RemyAWM <remy.awm.dev@gmail.com>"
#define COPYRIGHT                        "Copyright (C) 2012. All Rights Reserved"
#define DATE                             "2012"
#define ACCEPTED_COLOR_TYPE              "RGB*, GRAY*"

#define PROCEDURE_NAME_1                "plug-in-ldt-ui"
#define BLURB_NAME_1                     "Layer Duplicator Tool - 2.0.0"
#define LABEL_NAME_1                    "Layer Duplicator Tool..."
#define HELP_NAME_1                     "This is a \"Layer Duplicator Tool\""
#define MENU_PATH_1                     "<Image>/Filters/Misc"

#define DATA_KEY_VALS    "plug-in-ldt"
#define PARASITE_KEY     "plug-in-ldt-options"

/*  Local function prototypes  */

static void   query 										(void);
static void   run   										(const gchar      *name,
															 gint              nparams,
															 const GimpParam  *param,
															 gint             *nreturn_vals,
															 GimpParam       **return_vals);

static gboolean 		plugin_data_is_valid				(gint32 image_id,
															 gint32 drawable_id);

/*  Local variables  */

const PlugInVals default_vals =
{
  3,
  UP_DOWN_ORDER,
  GIMP_INTERPOLATION_CUBIC,
  MERGE_LAYERS,
  FALSE
};

const PlugInUIVals default_ui_vals =
{
  TRUE
};

static PlugInVals         vals;
static PlugInUIVals       ui_vals;

/* Register operations */
static GList *ops_list = NULL;

GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

MAIN ()

static void
query (void)
{
  gchar *help_path;
  gchar *help_uri;

  static GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    "run_mode",   "Interactive, non-interactive"    },
    { GIMP_PDB_IMAGE,    "image",      "Input image"                     },
    { GIMP_PDB_DRAWABLE, "drawable",   "Input drawable"                  }
  };

  gimp_plugin_domain_register (PLUGIN_NAME, LOCALEDIR);

  help_path = g_build_filename (DATADIR, "help", NULL);
  help_uri = g_filename_to_uri (help_path, NULL, NULL);
  g_free (help_path);

  gimp_plugin_help_register ("http://developer.gimp.org/plug-in-template/help",
                             help_uri);

  gimp_install_procedure (PROCEDURE_NAME_1,
		  	   BLURB_NAME_1,
		  	   HELP_NAME_1,
			  AUTHOR,
			  COPYRIGHT,
			  DATE,
			  N_(LABEL_NAME_1),
			  ACCEPTED_COLOR_TYPE,
			  GIMP_PLUGIN,
			  G_N_ELEMENTS (args), 0,
			  args, NULL);

  gimp_plugin_menu_register (PROCEDURE_NAME_1, MENU_PATH_1);
}

static void
run (const gchar      *name,
     gint              n_params,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam   values[1];
  GimpDrawable      *drawable;
  gint32             image_id;
  gint32			 drawable_id;
  GimpRunMode        run_mode;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;

  static RenderFunc render_func;

  *nreturn_vals = 1;
  *return_vals  = values;

  /*  Initialize i18n support  */
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif
  textdomain (GETTEXT_PACKAGE);

  run_mode = param[0].data.d_int32;
  image_id = param[1].data.d_int32;
  drawable_id = param[2].data.d_int32;
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  /*  Initialize with default values  */
  vals          = default_vals;
  ui_vals       = default_ui_vals;

  /* Install operations */

  LdtOperation *ops = ldt_operation_translate_new();
  ops_list = g_list_append (ops_list, ops);
  ops = ldt_operation_rotate_new();
  ops_list = g_list_append (ops_list, ops);


  if (! plugin_data_is_valid (image_id, drawable_id))
    status = GIMP_PDB_EXECUTION_ERROR;

  if (strcmp (name, PROCEDURE_NAME_1) == 0)
	  render_func = render;
  else
    status = GIMP_PDB_CALLING_ERROR;

  if(status == GIMP_PDB_SUCCESS)
  {
	  FREEZE_UNDO (image_id)

	  state_image_save (image_id, drawable_id);

	  hidden_image_create(image_id, drawable_id);

	  switch (run_mode)
	  {
	  case GIMP_RUN_NONINTERACTIVE:
		  if (n_params != 3)
		    {
		      status = GIMP_PDB_CALLING_ERROR;
		    }
		  else
		  {
			  //vals.dummy1      = param[3].data.d_int32;
		  }
		  break;
	  case GIMP_RUN_INTERACTIVE:
		  gimp_get_data (DATA_KEY_VALS,    &vals);

		  live_preview_init (image_id, drawable_id);

		  if (! dialog (image_id,
                  live_preview_get_layer (),
                  hidden_image_get (),
                  render_func,
                  &vals,
                  ops_list,
                  name))
		    {
		      status = GIMP_PDB_CANCEL;
		    }

          /* Отключаем предосмотр */
          live_preview_destore (image_id);
		  break;
	  case GIMP_RUN_WITH_LAST_VALS:
		  gimp_get_data (DATA_KEY_VALS, &vals);
		  break;

	  default:
		  break;
	  }
  }

  if (status == GIMP_PDB_SUCCESS)
    {
      gint32 image_work = hidden_image_get ();
      gint32 layer_work = hidden_image_get_work_layer ();

      gint32 render_layer = render (image_work, layer_work, &vals);

      if(render_layer != -1)
      {
          START_UNDO_GROUP (image_id)

          drawable_copy_tiles (render_layer,
                               drawable_id, TRUE);

          STOP_UNDO_GROUP (image_id)
      }

      state_image_restore (image_id, drawable_id, FALSE);

      if (run_mode != GIMP_RUN_NONINTERACTIVE)
	gimp_displays_flush ();

      if (run_mode == GIMP_RUN_INTERACTIVE)
	{
	  gimp_set_data (DATA_KEY_VALS,    &vals,    sizeof (vals));
	}
      THAW_UNDO(image_id);


    }
  else
  {
      if (status == GIMP_PDB_CANCEL)
        {
          state_image_restore(image_id, drawable_id, TRUE);
          THAW_UNDO(image_id)
        }
  }

  state_image_remove ();
  hidden_image_remove ();

  gimp_drawable_detach (drawable);
  gimp_displays_flush ();

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
}

static gboolean plugin_data_is_valid(gint32 image_id, gint32 drawable_id)
{
  if(gimp_image_is_valid(image_id) &&
	  gimp_item_is_valid(drawable_id) &&
	  gimp_item_is_layer(drawable_id))
	return TRUE;

  return FALSE;
}

const gchar*
plugin_get_name (const gchar *proc_name)
{
  if (g_strcmp0(PROCEDURE_NAME_1, proc_name) == 0)
    return BLURB_NAME_1;
  return "";
}

void
crop_layer (gint32 image_id,
             gint32 layer_id)
{
        gimp_layer_resize_to_image_size (layer_id);
        gimp_image_select_item (image_id,
                                   GIMP_CHANNEL_OP_REPLACE,
                                   layer_id);

        gint x1, x2, y1, y2;
        gboolean non_empty = TRUE;
        gimp_selection_bounds (image_id,
                                                          &non_empty,
                                                          &x1, &y1,
                                                          &x2, &y2);

        gimp_selection_none (image_id);

        gimp_layer_resize (layer_id,
                             x2 - x1, y2 - y1,
                             x1 * (-1), y1 * (-1));
}

void
drawable_copy_tiles (gint32 src_drawable_id,
                        gint32 dest_drawable_id,
                        gboolean undo)
{
        if(!gimp_item_is_valid(src_drawable_id) || !gimp_item_is_valid (dest_drawable_id))
          return;

        GimpDrawable *srcDrawable = NULL;
        GimpDrawable *destDrawable = NULL;

        gint x =0, y=0, w=0, h =0;
        if(!gimp_drawable_mask_intersect(src_drawable_id,
                                                                 &x,
                                                                 &y,
                                                                 &w,&h))
          return;

        gint src_width = gimp_drawable_width(src_drawable_id);
        gint src_height = gimp_drawable_height(src_drawable_id);
        gimp_layer_resize(dest_drawable_id,
                            src_width,
                            src_height,
                            0,
                            0);
        gint offset_x = 0, offset_y=0;
        gimp_drawable_offsets(src_drawable_id,&offset_x, &offset_y);
        gimp_layer_set_offsets(dest_drawable_id,offset_x,offset_y);

        srcDrawable = gimp_drawable_get(src_drawable_id);
        destDrawable = gimp_drawable_get(dest_drawable_id);

        GimpPixelRgn  srcPR, destPR;
        gint          width = srcDrawable->width, height = srcDrawable->height;
        guchar       *buffer;
        gint bytes  = srcDrawable->bpp;

        gimp_pixel_rgn_init(&srcPR, srcDrawable, x, y, w, h, FALSE, FALSE);
        gimp_pixel_rgn_init(&destPR,destDrawable, x, y, w, h,TRUE,TRUE);

        buffer = g_malloc (width * height * bytes);
                gimp_pixel_rgn_get_rect(&srcPR, buffer, x, y, w, h);
                gimp_pixel_rgn_set_rect(&destPR,buffer, x, y,w, h);
        g_free (buffer);

       gimp_drawable_flush(destDrawable);
       gimp_drawable_merge_shadow (destDrawable->drawable_id, undo);
       gimp_drawable_update(destDrawable->drawable_id, x, y, w, h);
}
