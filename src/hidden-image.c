/*
 * hidden-image.c
 *
 *  Created on: 29.09.2012
 *      Author: remy
 */

#include "config.h"

#include "main.h"
#include "hidden-image.h"

#include "plugin-intl.h"


#define HIDDEN_LAYER_NAME         "hidden-layer"

static gint hidden_image = -1;


void
hidden_image_create (gint32 image_id,
                        gint32 layer_id)
{
    gint32  image      =       -1;
    gint32  layer      =       -1;
    gint    width           =       gimp_image_width (image_id);
    gint    height          =       gimp_image_height (image_id);

    if(!gimp_image_is_valid(image_id) || !gimp_item_is_valid(layer_id))
          return;

        image      = gimp_image_new (width,
                                       height,
                                       gimp_image_base_type(image_id));

        DISABLE_UNDO (image)

        layer = gimp_layer_new_from_drawable (layer_id, image);

        gimp_item_set_name(layer, HIDDEN_LAYER_NAME);

        gimp_image_insert_layer(image, layer,0,-1);

        crop_layer(image, layer);

         hidden_image = image;
}

gint32
hidden_image_get (void)
{
  return hidden_image;
}

gint32
hidden_image_get_work_layer (void)
{
        gint32 work_layer = -1;

        if(gimp_image_is_valid(hidden_image))
                work_layer = gimp_image_get_layer_by_name (hidden_image, HIDDEN_LAYER_NAME);

        return work_layer;
}


void
hidden_image_remove (void)
{
  if (gimp_image_is_valid(hidden_image))
    gimp_image_delete(hidden_image);

  hidden_image = -1;
}
