#include "GameObjectTypes.h"
#include <unordered_set>

bool IsPixelShaderApplicable(GameObjectTypes type)
{
    static const std::unordered_set<GameObjectTypes> pixelShaderTypes = {
        //GameObjectTypes::Reef,
        //GameObjectTypes::ReefBoundary
    };
    return pixelShaderTypes.find(type) != pixelShaderTypes.end();
}
