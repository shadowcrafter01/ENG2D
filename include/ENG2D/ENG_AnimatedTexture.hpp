#ifndef ENG_ANIMATEDTEXTURE_HPP
#define ENG_ANIMATEDTEXTURE_HPP

#include "ENG_Atlas.hpp"
#include <vector>

class ENG_AnimatedTexture
{
private:
public:
    ENG_AnimatedTexture(ENG_Atlas *atlas, Vector2<int> frame_size, double framerate) : frameSize{frame_size},
                                                                                       atlas{atlas}
    {
        frameCount = atlas->texture.size.x / frame_size.x;
    }

    int frameCount;
    ENG_Atlas *atlas;
    double FPS;
    Vector2<int> frameSize;
    int currentFrame = 0;
};

#endif