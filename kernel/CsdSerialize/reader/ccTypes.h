//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_CCTYPES_H
#define TESTCSD_CCTYPES_H

class Vec2 {
public:
    float x;
    float y;

    Vec2() {
        this->x = 0;
        this->y = 0;
    }

    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

struct Color4B {
    Color4B();

    Color4B(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);

    inline void set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    bool operator==(const Color4B &right) const;

    bool operator!=(const Color4B &right) const;


    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    static const Color4B WHITE;
    static const Color4B YELLOW;
    static const Color4B BLUE;
    static const Color4B GREEN;
    static const Color4B RED;
    static const Color4B MAGENTA;
    static const Color4B BLACK;
    static const Color4B ORANGE;
    static const Color4B GRAY;
};

class Color3B {
public:
    Color3B();

    Color3B(unsigned char _r, unsigned char _g, unsigned char _b);

    explicit Color3B(const Color4B &color);

    bool operator==(const Color3B &right) const;

    bool operator==(const Color4B &right) const;

    bool operator!=(const Color3B &right) const;

    bool operator!=(const Color4B &right) const;


    bool equals(const Color3B &other) const {
        return (*this == other);
    }

    unsigned char r;
    unsigned char g;
    unsigned char b;

    static const Color3B WHITE;
    static const Color3B YELLOW;
    static const Color3B BLUE;
    static const Color3B GREEN;
    static const Color3B RED;
    static const Color3B MAGENTA;
    static const Color3B BLACK;
    static const Color3B ORANGE;
    static const Color3B GRAY;
};


class Size {
public:
    float width;
    float height;

    Size() {
        this->height = 0;
        this->width = 0;
    }

    Size(float width, float height) {
        this->height = height;
        this->width = width;
    }
};

class Rect {
public:
    Vec2 origin;
    Size size;

    Rect() {

    }
};


namespace cocosstudio {
    struct BlendFunc {
        /** source blend function */
        unsigned int src;
        /** destination blend function */
        unsigned int dst;

        /** Blending disabled. Uses {GL_ONE, GL_ZERO} */
        static const BlendFunc DISABLE;
        /** Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA} */
        static const BlendFunc ALPHA_PREMULTIPLIED;
        /** Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} */
        static const BlendFunc ALPHA_NON_PREMULTIPLIED;
        /** Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE} */
        static const BlendFunc ADDITIVE;

        bool operator==(const BlendFunc &a) const {
            return src == a.src && dst == a.dst;
        }

        bool operator!=(const BlendFunc &a) const {
            return src != a.src || dst != a.dst;
        }

        bool operator<(const BlendFunc &a) const {
            return src < a.src || (src == a.src && dst < a.dst);
        }
    };
}




#endif //TESTCSD_CCTYPES_H
