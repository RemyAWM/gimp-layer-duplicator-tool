/*
 * types.h
 *
 *  Created on: 28.09.2012
 *      Author: remy
 */

#ifndef TYPES_H_
#define TYPES_H_

/*Define an object structs */
typedef struct _LdtOperation LdtOperation;
typedef struct _LdtOperationTranslate LdtOperationTranslate;
typedef struct _LdtOperationRotate LdtOperationRotate;

typedef struct _LdtTreeModelOperation LdtTreeModelOperation;
typedef struct _LdtListViewOperations LdtListViewOperations;

typedef struct _LdtPackRecord LdtPackRecord;

typedef struct _PlugInVals PlugInVals;

typedef struct _PlugInUIVals PlugInUIVals;

typedef struct _PlugInPreviewData PlugInPreviewData;

typedef gint32 (* RenderFunc) (gint32 image_id,
                                gint32 drawable_id,
                                PlugInVals *values);

#endif /* TYPES_H_ */
