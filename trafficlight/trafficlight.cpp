#include "tbitfield.h"
#include "trafficlight.h"
#include <iostream>


pi_Light::pi_Light() : state(1) {
    state.SetBit(0); // старт - зеленый
}

void pi_Light::switch_light() {
    if (state.GetBit(0)) {
        state.ClrBit(0);
    }
    else {
        state.SetBit(0);
    }
}

bool pi_Light::isGreen() const {
    return state.GetBit(0);
}

bool pi_Light::isYellow() const {
    return false;
}

bool pi_Light::isRed() const {
    return !state.GetBit(0);
}


car_Light::car_Light() : state(2) {
    // 00 - красный, 01 - желтый, 10 - зеленый
    state.SetBit(1); // старт - зеленый (10)
}

void car_Light::switch_light() {
    if (state.GetBit(1) && !state.GetBit(0)) { // зеленый (10)
        state.ClrBit(1);
        state.SetBit(0); // желтый (01)
    }
    else if (state.GetBit(0) && !state.GetBit(1)) { // желтый (01)
        state.ClrBit(0); // красный (00)
    }
    else { // красный (00)
        state.SetBit(1); // зеленый (10)
    }
}

bool car_Light::isGreen() const {
    return state.GetBit(1) && !state.GetBit(0); // 10
}

bool car_Light::isYellow() const {
    return !state.GetBit(1) && state.GetBit(0); // 01
}

bool car_Light::isRed() const {
    return !state.GetBit(1) && !state.GetBit(0); // 00
}

Road::Road() {
    if (pl.isGreen()) {
        while (!cl.isRed()) {
            cl.switch_light();
        }
    }
}

void Road::switch_light() {
    bool carWasRed = cl.isRed();
    bool carWasYellow = cl.isYellow();

    cl.switch_light();

    // Синхроним с пещшеходным
    if (carWasYellow && cl.isRed()) {
        // Автомобильный перешел с желтого на красный - зеленый пешеходный
        if (!pl.isGreen()) {
            pl.switch_light();
        }
    }
    else if (carWasRed && cl.isGreen()) {
        // Автомобильный красный зеленый - красный пешеходный
        if (pl.isGreen()) {
            pl.switch_light();
        }
    }
}

void Road::printl(std::ostream& out, const Light& l) const {
    if (l.isGreen()) out << "Green";
    else if (l.isYellow()) out << "Yellow";
    else if (l.isRed()) out << "Red";
}

std::ostream& operator<<(std::ostream& out, const Road& r) {
    out << "P: ";
    r.printl(out, r.pl);
    out << " | C: ";
    r.printl(out, r.cl);
    return out;
}