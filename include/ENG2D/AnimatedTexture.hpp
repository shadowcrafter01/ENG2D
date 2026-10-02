#ifndef ENG_ANIMATEDTEXTURE_HPP
#define ENG_ANIMATEDTEXTURE_HPP

#include "ENG2D/Atlas.hpp"
#include <vector>

namespace ENG
{

    class AnimatedTexture
    {
    private:
    public:
        AnimatedTexture(Atlas *atlas, Vector2<int> frame_size, double framerate) : frameSize{frame_size},
                                                                                   atlas{atlas}
        {
            frameCount = atlas->texture.size.x / frame_size.x;
        }

        int frameCount;
        Atlas *atlas;
        double FPS;
        Vector2<int> frameSize;
        int currentFrame = 0;
    };
};
#endif