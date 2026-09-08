#pragma once

#include <campaign/api/api_util.h>
#include <campaign/api/api_layout.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct campaign_blueprint_piece_info_t
    {
        char *id;
        void *layoutPtr;
    } campaign_blueprint_piece_info_t;

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_create(int32_t sizeCount, void **outBlueprintPtr);

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_delete(void *blueprintPtr);

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_push(void *blueprintPtr, const char* id, const void *layoutPtr);

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_get_size(void *blueprintPtr, int32_t *outSize);

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_get_iterator(void *blueprintPtr, void **outIteratorPtr);

    /// @attention If the call is successful, the caller is responsible for freeing `campaign_blueprint_piece_info_t`. Use `campaign_blueprint_iterator_free_output`.
    CAMPAIGN_API campaign_api_result_t campaign_blueprint_iterator_next(void *blueprintPtr, void **refIteratorPtr, campaign_blueprint_piece_info_t *outCurrent);

    CAMPAIGN_API campaign_api_result_t campaign_blueprint_iterator_free_output(campaign_blueprint_piece_info_t *pieceInfoPtr);
#ifdef __cplusplus
}
#endif