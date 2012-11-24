/*
 * live-preview.h
 *
 *  Created on: 28.09.2012
 *      Author: remy
 */

#ifndef LIVE_PREVIEW_H_
#define LIVE_PREVIEW_H_



struct _PlugInPreviewData
{
    /* Изображение (как правило скрытое) в котором создается эффект */
    gint32          image_work;

    /* Изображение содержащее слой предосмотра.
    * (слой предосмотра - как правило слой созданный live_preview_init)
    * */
    gint32          image_preview;

    /* Собственно - слой предосмотра */
    gint32          layer_preview;

    /* Указатель на структуру содержащую настройки (свойства) плагина */
    PlugInVals      *values;

    /* Указатель на функцию создающею эффект */
    gpointer        effect_func;
};



gboolean        live_preview_init               (gint32 image_id,
                                                   gint32 drawable_id);
gint32          live_preview_get_layer          (void);
void            live_preview_destore            (gint32 image_id);
void            live_preview_update             (GtkWidget *widget,
                                                    gpointer data);

#endif /* LIVE_PREVIEW_H_ */
