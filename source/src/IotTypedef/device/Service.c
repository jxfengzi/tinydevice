/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2016-7-25
 *
 * @file   Service.h
 *
 * @remark
 *
 */

#include <tiny_malloc.h>
#include <tiny_log.h>
#include "Service.h"
#include "Property.h"

#ifdef SPEC_ACTION
#include "Action.h"
#endif

#define TAG     "Service"

TINY_LOR
static TinyRet Service_Construct(Service *thiz);

TINY_LOR
static void Service_Dispose(Service *thiz);

TINY_LOR
static void property_release_handler(void *data, void *ctx)
{
    Property_Delete((Property *)data);
}

#ifdef SPEC_ACTION
TINY_LOR
static void action_release_handler(void *data, void *ctx)
{
    Action_Delete((Action *)data);
}
#endif /* SPEC_ACTION */

TINY_LOR
Service* Service_New(void)
{
    Service *thiz = NULL;

    do
    {
        thiz = (Service *)tiny_malloc(sizeof(Service));
        if (thiz == NULL)
        {
            LOG_D(TAG, "tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Service_Construct(thiz)))
        {
            Service_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TINY_LOR
static TinyRet Service_Construct(Service *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        memset(thiz, 0, sizeof(Service));

        ret = TinyList_Construct(&thiz->properties);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "TinyList_Construct FAILED: %s", tiny_ret_to_str( ret));
            break;
        }

        thiz->properties.additionalData = thiz;
        TinyList_SetDeleteListener(&thiz->properties, property_release_handler, thiz);

#ifdef SPEC_ACTION
        ret = TinyList_Construct(&thiz->actions);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "TinyList_Construct FAILED: %s", tiny_ret_to_str( ret));
            break;
        }

        thiz->actions.additionalData = thiz;
        TinyList_SetDeleteListener(&thiz->actions, action_release_handler, thiz);
#endif
    } while (false);

    return ret;
}

TINY_LOR
static void Service_Dispose(Service *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->properties);
}

TINY_LOR
void Service_Delete(Service *thiz)
{
    RETURN_IF_FAIL(thiz);

    LOG_D(TAG, "Service_Delete (iid: %d)", thiz->iid);

    Service_Dispose(thiz);
    tiny_free(thiz);
}