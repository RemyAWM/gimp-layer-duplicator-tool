/*
 * hidden-image.h
 *
 *  Created on: 28.09.2012
 *      Author: remy
 */

#ifndef HIDDEN_IMAGE_H_
#define HIDDEN_IMAGE_H_


void            hidden_image_create               (gint32 image_id, gint32 layer_id);

gint32          hidden_image_get                  (void);

gint32          hidden_image_get_work_layer      (void);

void            hidden_image_remove               (void);

#endif /* HIDDEN_IMAGE_H_ */
