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

#ifndef __MAIN_H__
#define __MAIN_H__

#include <libgimp/gimp.h>

#include "enum.h"

/*  Setup plug-in datas   */

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


/* Undo macros */
#define DISABLE_UNDO(IMAGE)    if(gimp_image_undo_is_enabled(IMAGE))    \
                                    gimp_image_undo_disable(IMAGE);


#define ENABLE_UNDO(IMAGE)    if(!gimp_image_undo_is_enabled(IMAGE))    \
                                    gimp_image_undo_enable(IMAGE);


#define FREEZE_UNDO(IMAGE)     if(gimp_image_undo_is_enabled(IMAGE))        \
                                    gimp_image_undo_freeze (IMAGE);


#define THAW_UNDO(IMAGE)       if(!gimp_image_undo_is_enabled(IMAGE))      \
                                      gimp_image_undo_thaw(IMAGE);


#define START_UNDO_GROUP(IMAGE)  if(!gimp_image_undo_is_enabled(IMAGE))      \
                                      gimp_image_undo_thaw(IMAGE);            \
                                  gimp_image_undo_group_start(IMAGE);

#define STOP_UNDO_GROUP(IMAGE)   gimp_image_undo_group_end(IMAGE);          \
                                  if(gimp_image_undo_is_enabled(IMAGE))      \
                                  gimp_image_undo_freeze(IMAGE);


struct _PlugInVals
{
  gint      			clone_count;
  LayerOrderType 		order_layer;
  GimpInterpolationType interpolation;
  RetultLayerType		result_layer;
  gboolean 				autocrop;
};

struct _PlugInUIVals
{
  gboolean  chain_active;
};



/* Public */
const gchar*            plugin_get_name                  (const gchar *proc_name);
void                    crop_layer                        (gint32 image_id,
                                                             gint32 layer_id);

void                    drawable_copy_tiles              (gint32 src_drawable_id,
                                                             gint32 dest_drawable_id,
                                                             gboolean undo);
#include "types.h"

#endif /* __MAIN_H__ */
