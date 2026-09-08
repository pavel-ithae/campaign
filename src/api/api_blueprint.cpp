#include <campaign/api/api_blueprint.h>

#include "api_except_internal.hpp"
#include "api_string_internal.hpp"

#include <campaign/blueprint.hpp>
#include <campaign/api/api_string.h>

#include <string>
#include <cstring>
#include <stdexcept>

using namespace campaign;

#define TO_BLUEPRINT_PTR(ptr) static_cast<Blueprint *>(ptr)
#define TO_VOID_PTR(ptr) static_cast<void *>(ptr)

void GetBlueprintPieceInIteration(const BlueprintPiece *iteratorPtr, campaign_blueprint_piece_info_t *outBlueprintPiece)
{
    // Currently doesn't deallocate string and layout if excepts, but we shouldn't worry about that.

    outBlueprintPiece->id = AllocateString(iteratorPtr->id.length());
    std::memcpy(outBlueprintPiece->id, iteratorPtr->id.begin().base(), iteratorPtr->id.length() + 1);

    outBlueprintPiece->layoutPtr = static_cast<void *>(new Layout(iteratorPtr->layout));
}

campaign_api_result_t campaign_blueprint_create(int32_t sizeCount, void **outBlueprintPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(outBlueprintPtr);

    CAMPAIGN_API_BEGIN

    *outBlueprintPtr = TO_VOID_PTR(new Blueprint(sizeCount));

    CAMPAIGN_API_END_WITH_SAFETY(*outBlueprintPtr = nullptr)
}

campaign_api_result_t campaign_blueprint_delete(void *blueprintPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(blueprintPtr);

    CAMPAIGN_API_BEGIN

    delete TO_BLUEPRINT_PTR(blueprintPtr);

    CAMPAIGN_API_END
}

campaign_api_result_t campaign_blueprint_push(void *blueprintPtr, const char *id, const void *layoutPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(blueprintPtr);
    CAMPAIGN_API_VALIDATE_PTR(id);
    CAMPAIGN_API_VALIDATE_PTR(layoutPtr);

    CAMPAIGN_API_BEGIN

    TO_BLUEPRINT_PTR(blueprintPtr)->push(BlueprintPiece(std::string(id), *static_cast<const Layout *>(layoutPtr)));

    CAMPAIGN_API_END
}

campaign_api_result_t campaign_blueprint_get_size(void *blueprintPtr, int32_t *outSize)
{
    CAMPAIGN_API_VALIDATE_PTR(blueprintPtr);
    CAMPAIGN_API_VALIDATE_PTR(outSize);

    CAMPAIGN_API_BEGIN

    *outSize = TO_BLUEPRINT_PTR(blueprintPtr)->size();

    CAMPAIGN_API_END_WITH_SAFETY(*outSize = 0)
}

campaign_api_result_t campaign_blueprint_get_iterator(void *blueprintPtr, void **outIteratorPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(blueprintPtr);
    CAMPAIGN_API_VALIDATE_PTR(outIteratorPtr);

    CAMPAIGN_API_BEGIN

    *outIteratorPtr = static_cast<void *>(const_cast<BlueprintPiece *>(TO_BLUEPRINT_PTR(blueprintPtr)->begin()));

    CAMPAIGN_API_END_WITH_SAFETY({
        *outIteratorPtr = nullptr;
    })
}

campaign_api_result_t campaign_blueprint_iterator_next(void *blueprintPtr, void **refIteratorPtr, campaign_blueprint_piece_info_t *outCurrent)
{
    CAMPAIGN_API_VALIDATE_PTR(blueprintPtr);
    CAMPAIGN_API_VALIDATE_PTR(refIteratorPtr);
    CAMPAIGN_API_VALIDATE_PTR(outCurrent);

    bool blueprintPieceInfoAllocated = false;

    CAMPAIGN_API_BEGIN

    const BlueprintPiece *iteratorPtr = const_cast<const BlueprintPiece *>(static_cast<BlueprintPiece *>(*refIteratorPtr));

    if (iteratorPtr == TO_BLUEPRINT_PTR(blueprintPtr)->end())
    {
        throw std::out_of_range("Reached end of iteration.");
    }

    GetBlueprintPieceInIteration(iteratorPtr, outCurrent);
    bool blueprintPieceInfoAllocated = true;

    *refIteratorPtr = static_cast<void *>(const_cast<BlueprintPiece *>(std::next(iteratorPtr)));

    CAMPAIGN_API_END_WITH_SAFETY({
        if (blueprintPieceInfoAllocated)
        {
            campaign_blueprint_iterator_free_output(outCurrent);
        }

        outCurrent->id = nullptr;
        outCurrent->layoutPtr = nullptr;
    })
}

campaign_api_result_t campaign_blueprint_iterator_free_output(campaign_blueprint_piece_info_t *pieceInfoPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(pieceInfoPtr);

    CAMPAIGN_API_BEGIN

    campaign_string_free(pieceInfoPtr->id);
    campaign_layout_delete(pieceInfoPtr->layoutPtr);

    CAMPAIGN_API_END
}