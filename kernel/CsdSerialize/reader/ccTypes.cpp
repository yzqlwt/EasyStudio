//
// Created by yzqlwt on 2020/12/2.
//
#include "ccTypes.h"

const cocosstudio::BlendFunc cocosstudio::BlendFunc::DISABLE = {1, 0};
const cocosstudio::BlendFunc cocosstudio::BlendFunc::ALPHA_PREMULTIPLIED = {1, 0x0303};
const cocosstudio::BlendFunc cocosstudio::BlendFunc::ALPHA_NON_PREMULTIPLIED = {0x0302, 0x0303};
const cocosstudio::BlendFunc cocosstudio::BlendFunc::ADDITIVE = {0x0302, 1};

const Color3B Color3B::WHITE(255, 255, 255);
const Color3B Color3B::YELLOW(255, 255, 0);
const Color3B Color3B::GREEN(0, 255, 0);
const Color3B Color3B::BLUE(0, 0, 255);
const Color3B Color3B::RED(255, 0, 0);
const Color3B Color3B::MAGENTA(255, 0, 255);
const Color3B Color3B::BLACK(0, 0, 0);
const Color3B Color3B::ORANGE(255, 127, 0);
const Color3B Color3B::GRAY(166, 166, 166);

Color3B::Color3B() {

}

Color3B::Color3B(unsigned char _r, unsigned char _g, unsigned char _b) {
    this->r = _r;
    this->g = _g;
    this->b = _b;
}

Color3B::Color3B(const Color4B &color) {

}

bool Color3B::operator==(const Color3B &right) const {
    return false;
}

bool Color3B::operator==(const Color4B &right) const {
    return false;
}

bool Color3B::operator!=(const Color3B &right) const {
    return false;
}

bool Color3B::operator!=(const Color4B &right) const {
    return false;
}

const Color4B Color4B::WHITE  (255, 255, 255, 255);
const Color4B Color4B::YELLOW (255, 255,   0, 255);
const Color4B Color4B::GREEN  (  0, 255,   0, 255);
const Color4B Color4B::BLUE   (  0,   0, 255, 255);
const Color4B Color4B::RED    (255,   0,   0, 255);
const Color4B Color4B::MAGENTA(255,   0, 255, 255);
const Color4B Color4B::BLACK  (  0,   0,   0, 255);
const Color4B Color4B::ORANGE (255, 127,   0, 255);
const Color4B Color4B::GRAY   (166, 166, 166, 255);

Color4B::Color4B() {

}

Color4B::Color4B(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
}

bool Color4B::operator==(const Color4B &right) const {
    return false;
}

bool Color4B::operator!=(const Color4B &right) const {
    return false;
}
