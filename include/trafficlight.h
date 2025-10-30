#pragma once
#include "tbitfield.h"
#include "tset.h"
#include <iostream>

class Light {
public:
    virtual void switch_light() = 0;
    virtual bool isGreen() const = 0;
    virtual bool isYellow() const = 0;
    virtual bool isRed() const = 0;
};

class pi_Light : public Light {
private:
    TBitField state;
public:
    pi_Light();
    void switch_light() override;
    bool isGreen() const override;
    bool isYellow() const override;
    bool isRed() const override;
};

class car_Light : public Light {
private:
    TBitField state;
public:
    car_Light();
    ~car_Light() = 0;
    void switch_light() override;
    bool isGreen() const override;
    bool isYellow() const override;
    bool isRed() const override;
};

class Road {
public:
    pi_Light pl;
    car_Light cl;

    Road();
    void switch_light();
    void printl(std::ostream& out, const Light& l) const;
    friend std::ostream& operator<<(std::ostream& out, const Road& r);
};