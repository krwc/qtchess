#ifndef COMMON_HPP
#define COMMON_HPP
#include <algorithm>

template<typename T>
struct Coord2D {
    T x, y;

    Coord2D() : x(0), y(0) {
    }

    Coord2D(T x, T y) : x(x), y(y) {
    }

    Coord2D operator-(const Coord2D& rhs) {
        return Coord2D(x - rhs.x, y - rhs.y);
    }

    Coord2D operator+(const Coord2D& rhs) {
        return Coord2D(x + rhs.x, y + rhs.y);
    }

    bool operator==(const Coord2D& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Coord2D& rhs) const {
        return x != rhs.x || y != rhs.y;
    }
};

template<typename Container, typename Needle>
bool Contains(const Container& C, const Needle& N) {
    for (const auto& Element : C)
        if (Element == N)
            return true;
    return false;
}

typedef std::vector<Coord2D<int>> CoordsVector;

#endif // COMMON_HPP

