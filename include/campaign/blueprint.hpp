#pragma once

#include <campaign/layout.hpp>
#include <string>
#include <vector>

namespace campaign
{
    class BlueprintPiece
    {
    public:
        std::string id;
        Layout layout;

        BlueprintPiece(std::string id, Layout layout) : id(id), layout(layout) {}
    };

    class Blueprint
    {
    public:
        Blueprint(size_t pieceCount);

        void push(const BlueprintPiece &piece);

        const size_t size() const;

        const BlueprintPiece *begin() const;
        const BlueprintPiece *end() const;

    private:
        std::vector<BlueprintPiece> pieces_;
    };
}