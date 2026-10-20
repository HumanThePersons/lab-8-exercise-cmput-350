#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr.
public:
    Sprite(const sf::Texture& texture)
        : mTexture(&texture),
          mVertexArray(sf::PrimitiveType::Triangles, 6),
          mMinPointTex{},
          mMaxPointTex(texture.getSize()),
          mOrigin{},
          mTransform() {
        recomputeVertexArray();
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint)
        : mTexture(&texture),
          mVertexArray(sf::PrimitiveType::Triangles, 6),
          mMinPointTex{texMinPoint},
          mMaxPointTex{texMaxPoint},
          mOrigin{},
          mTransform() {
        recomputeVertexArray();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (mTexture == nullptr) {
            return;
        }
        recomputeVertexArray();
        states.texture = mTexture;
        states.coordinateType = sf::CoordinateType::Pixels;
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {
        // top-left triangle
        sf::Vector2f{0.f, 0.f},  // top-left
        sf::Vector2f{0.f, 1.f},  // bottom-left
        sf::Vector2f{1.f, 0.f},  // top-right
        // bottom-right triangle
        sf::Vector2f{1.f, 0.f},  // top-right
        sf::Vector2f{0.f, 1.f},  // bottom-left
        sf::Vector2f{1.f, 1.f}   // bottom-right
    };

    void recomputeVertexArray() const {
        if (mTexture == nullptr) return;
        const sf::Vector2f size = mMaxPointTex - mMinPointTex;
        mVertexArray.resize(6);
        for (size_t i = 0; i < 6; ++i) {
            mVertexArray[i].position =
                mTransform.applyPoint(((DEFAULT_VERTICES[i] * size)) - mOrigin);
            mVertexArray[i].texCoords = (DEFAULT_VERTICES[i] * size) + mMinPointTex;
        }
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif  // SPRITE_H