#ifndef ENG_DRAWTOOLS_HPP
#define ENG_DRAWTOOLS_HPP

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "ENG_Camera.hpp"
#include "ENG_Texture.hpp"
#include "ENG_Atlas.hpp"
#include "ENG_AnimatedTexture.hpp"
#include "ENG_Font.hpp"

#include "Vector2.hpp"
#include "colorRGBA.hpp"

class ENG_DrawTools
{
private:
    inline static Vector2<double> projectToCamera(ENG_Camera *camera, Vector2<double> vector)
    {
        Vector2<double> out = vector;

        out.x -= camera->position.x;
        out.y -= camera->position.y;

        out *= camera->zoom;

        out.Rotate(-camera->angle);
        out.y *= -1;

        out.x += camera->window->center.x;
        out.y += camera->window->center.y;

        return out;
    }
    inline static bool onscreen(ENG_Camera *camera, Vector2<double> *vector, double size)
    {
        if (vector->x + 2 * size < 0 || vector->y + 2 * size < 0 || vector->x - size > camera->window->size.x || vector->y - size > camera->window->size.y)
        {
            return false;
        }
        return true;
    }

public:
    inline static void DrawTexture(ENG_Camera *camera, ENG_Texture *texture, Vector2<double> position, double size = 1, double angle = 0, colorRGBA color = colorRGBA(255, 255, 255, 255))
    {

        Vector2<double> pos = projectToCamera(camera, position);
        SDL_FRect r;
        r.h = size * camera->zoom * texture->size.y;
        r.w = size * camera->zoom * texture->size.x;
        r.x = pos.x - (r.w / 2);
        r.y = pos.y - (r.h / 2);
        Vector2<double> vr(r.x, r.y);

        if (onscreen(camera, &vr, sqrt((r.w / 2 * r.w / 2) + (r.h / 2 * r.h / 2))))
        {
            // texture->pointer = SDL_CreateTextureFromSurface(camera->window->renderer, texture->surface);
            SDL_SetTextureBlendMode(texture->pointer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(texture->pointer, color.red, color.green, color.blue);
            SDL_SetTextureAlphaMod(texture->pointer, color.alpha);
            SDL_RenderTextureRotated(texture->renderer, texture->pointer, NULL, &r, angle + camera->angle, NULL, SDL_FLIP_NONE);
            textureDrawCount++;
            // SDL_DestroyTexture(texture->pointer);
        }
    }
    inline static void DrawAtlas(ENG_Camera *camera, ENG_Atlas *atlas, Vector2<double> position, double size = 1, double angle = 0, colorRGBA color = colorRGBA(255, 255, 255, 255))
    {

        Vector2<double> pos = projectToCamera(camera, position);
        SDL_FRect r;
        r.h = size * camera->zoom * atlas->texture.size.y;
        r.w = size * camera->zoom * atlas->texture.size.x;
        r.x = pos.x - (r.w / 2);
        r.y = pos.y - (r.h / 2);
        Vector2<double> vr(r.x, r.y);

        if (onscreen(camera, &vr, sqrt((r.w / 2 * r.w / 2) + (r.h / 2 * r.h / 2))))
        {
            SDL_FRect s;
            s.h = atlas->rect.h;
            s.w = atlas->rect.w;
            s.x = atlas->rect.x + (atlas->texture.size.x / 2) - (s.w / 2);
            s.y = -atlas->rect.y + (atlas->texture.size.y / 2) - (s.h / 2);

            SDL_SetTextureBlendMode(atlas->texture.pointer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(atlas->texture.pointer, color.red, color.green, color.blue);
            SDL_SetTextureAlphaMod(atlas->texture.pointer, color.alpha);
            SDL_RenderTextureRotated(atlas->texture.renderer, atlas->texture.pointer, &s, &r, angle + camera->angle, NULL, SDL_FLIP_NONE);
            textureDrawCount++;
        }
    }
    inline static void DrawAnimatedTexture(ENG_Camera *camera, ENG_AnimatedTexture *animatedTexture, Vector2<double> position, int frame = -1, double size = 1, double angle = 0, colorRGBA color = colorRGBA(255, 255, 255, 255))
    {
        if (frame == -1)
        {
            frame = animatedTexture->currentFrame;
        }
        else
        {
            frame = frame % animatedTexture->frameCount;
        }

        Vector2<double> pos = projectToCamera(camera, position);
        SDL_FRect r;
        r.w = size * camera->zoom * animatedTexture->frameSize.x;
        r.h = size * camera->zoom * animatedTexture->frameSize.y;
        r.x = pos.x - (r.w / 2);
        r.y = pos.y - (r.h / 2);
        Vector2<double> vr(r.x, r.y);

        if (onscreen(camera, &vr, sqrt((r.w / 2 * r.w / 2) + (r.h / 2 * r.h / 2))))
        {
            SDL_FRect s;
            s.w = animatedTexture->frameSize.x;
            s.h = animatedTexture->frameSize.y;
            s.x = animatedTexture->frameSize.x * frame;
            s.y = 0;

            SDL_SetTextureBlendMode(animatedTexture->atlas->texture.pointer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(animatedTexture->atlas->texture.pointer, color.red, color.green, color.blue);
            SDL_SetTextureAlphaMod(animatedTexture->atlas->texture.pointer, color.alpha);
            SDL_RenderTextureRotated(animatedTexture->atlas->texture.renderer, animatedTexture->atlas->texture.pointer, &s, &r, angle + camera->angle, NULL, SDL_FLIP_NONE);
            textureDrawCount++;
        }
    }

    inline static void DrawFont(ENG_Camera *camera, ENG_Font *font, std::string message, Vector2<double> position, double size = 1, double angle = 0, colorRGBA color = colorRGBA(255, 255, 255, 255), Vector2<double> scale = Vector2<double>(1, 1))
    {
        if (message == "" || !font->state)
        {
            return;
        }

        SDL_Surface *textSurface = TTF_RenderText_Solid(font->font, message.c_str(), 0, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(camera->window->renderer, textSurface);
        Vector2<double> pos = projectToCamera(camera, position);
        SDL_FRect r;
        r.h = size * scale.x * camera->zoom * textSurface->h;
        r.w = size * scale.y * camera->zoom * textSurface->w;
        r.x = pos.x;
        r.y = pos.y;
        Vector2<double> vr(r.x, r.y);

        if (onscreen(camera, &vr, sqrt((r.w / 2 * r.w / 2) + (r.h / 2 * r.h / 2))))
        {
            if (!SDL_RenderTextureRotated(camera->window->renderer, texture, NULL, &r, angle + camera->angle, NULL, SDL_FLIP_NONE))
            {
                ENG_Console::LogError("failed to draw text: " + message);
            }
        }
        SDL_DestroySurface(textSurface);
        SDL_DestroyTexture(texture);
    }
    inline static int textureDrawCount = 0;

    inline static void DrawLine(ENG_Camera *camera, Vector2<double> from, Vector2<double> to, colorRGBA color = colorRGBA(255, 255, 255, 255))
    {
        from = projectToCamera(camera, from);
        to = projectToCamera(camera, to);
        SDL_SetRenderDrawColor(camera->window->renderer, color.red, color.green, color.blue, color.alpha);
        SDL_RenderLine(camera->window->renderer, from.x, from.y, to.x, to.y);
    }

    inline static void DrawTri(ENG_Camera *camera, Vector2<double> corner1, Vector2<double> corner2, Vector2<double> corner3, colorRGBA color = colorRGBA(255, 255, 255, 255))
    {

        corner1 = projectToCamera(camera, corner1);
        corner2 = projectToCamera(camera, corner2);
        corner3 = projectToCamera(camera, corner3);

        SDL_Vertex vertices[3];
        // SDL_zeroa(vertices);
        vertices[0].position.x = corner1.x;
        vertices[0].position.y = corner1.y;
        vertices[0].color.r = color.red / 255.0f;
        vertices[0].color.g = color.green / 255.0f;
        vertices[0].color.b = color.blue / 255.0f;
        vertices[0].color.a = color.alpha / 255.0f;
        vertices[1].position.x = corner2.x;
        vertices[1].position.y = corner2.y;
        vertices[1].color.r = color.red / 255.0f;
        vertices[1].color.g = color.green / 255.0f;
        vertices[1].color.b = color.blue / 255.0f;
        vertices[1].color.a = color.alpha / 255.0f;
        vertices[2].position.x = corner3.x;
        vertices[2].position.y = corner3.y;
        vertices[2].color.r = color.red / 255.0f;
        vertices[2].color.g = color.green / 255.0f;
        vertices[2].color.b = color.blue / 255.0f;
        vertices[2].color.a = color.alpha / 255.0f;

        SDL_RenderGeometry(camera->window->renderer, NULL, vertices, 3, NULL, 0);
    }
};

#endif