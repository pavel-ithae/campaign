#include <campaign/blueprint.hpp>

using namespace campaign;

Blueprint::Blueprint(size_t pieceCount)
{
    pieces_.reserve(pieceCount);
}

void Blueprint::push(const BlueprintPiece &piece)
{
    pieces_.push_back(piece);
}

const size_t campaign::Blueprint::size() const
{
    return pieces_.size();
}

const BlueprintPiece *Blueprint::begin() const
{
    return pieces_.begin().base();
}

const BlueprintPiece *Blueprint::end() const
{
    return pieces_.end().base();
}