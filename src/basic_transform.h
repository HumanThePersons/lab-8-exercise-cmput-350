#ifndef BASIC_TRANSFORM_H
#define BASIC_TRANSFORM_H

#include <cassert>

#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // compose(f, g) is equivalent to first applying g, then applying f.
    static BasicTransform compose(const BasicTransform& f, const BasicTransform& g) {
        // It is possible, since we can represent f as a matrix F which is the translation matrix * scale matrix
        // (this order is based on my implementation). The same can be found for G, then the question becomes:
        // is FGx = F(Gx), and since matrix multiplication is associative, the answer is yes (for both points and vectors).

        // Based on my derivation of f(gx) = f.scale (g.scale*x + g.translation) + f.translation
        // => (f.scale * g.scale)*x + (f.scale * g.translation) + f.translation
        BasicTransform composition;

        composition.scale = f.scale * g.scale;  // Set scale first

        composition.translation = (f.scale * g.translation) + f.translation;  // Set translation

        return composition;
    };

    static BasicTransform inverse(const BasicTransform& f) {
        // It is possible. f can be represented as a matrix F = translation matrix * scale matrix (order based on
        // my implementation, however either order leads to the same outcome here). For f to always have an inverse,
        // F needs to be invertible. F is invertible as long as det(F) is always nonzero. det(F) can be found as
        // scale.x * scale.y * 1, which is always nonzero if scale.x and scale.y are also always nonzero. 
        // (for both points and vectors)
        // Referenced: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

        // Scale: scale = (1/sx, 1/sy) and translation = (-tx / sx, -ty / sy)
        BasicTransform inversion;
        
        inversion.scale = sf::Vector2f((1 / f.scale.x), (1 / f.scale.y));  // Set scale

        inversion.translation = (-1.0f) * f.translation * inversion.scale;  // Set translation

        return inversion;
    };

    constexpr BasicTransform(const sf::Vector2f& scale = {1.f, 1.f},
                             const sf::Vector2f& translation = {0.f, 0.f})
        : scale{scale}, translation{translation} {}

    constexpr sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // first scale, then translate -- if you do the reverse, the translation gets messed up.
        return {p.x * scale.x + translation.x, p.y * scale.y + translation.y};
    }

    constexpr sf::Vector2f applyVector(const sf::Vector2f& p) const {
        // vectors are not translated by transformations
        return {p.x * scale.x, p.y * scale.y};
    }

    constexpr bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};

#endif