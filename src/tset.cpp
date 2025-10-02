// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp > 0 ? mp : 1)
{
    // Макс мощность множества
    if (mp <= 0) {
        MaxPower = 1;  
    }
    else {
        MaxPower = mp;
    }
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;  
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower; 
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower) {
        return 0; 
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    // Проверяем границы элемента
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Error element. Out of range";  
    }

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    // Проверяем границы элемента
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Error element. Out of range";
    }

    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;    // мощность
        BitField = s.BitField;    // битовое поле
    }
    return *this;  
}

int TSet::operator==(const TSet &s) const // сравнение
{
    // равны мощности и битовое поле
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    // Максимум из мощностей
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;

    TSet result(maxPower);
    
    // или
    result.BitField = BitField | s.BitField;

    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    // Проверяем допустимость элемента
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Error element. Out of range";
    }

    // копируем
    TSet result(*this);

    // добавляем
    result.InsElem(Elem);

    // возвращаем
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Error element. Out of range";
    }


    TSet result(*this);

    result.DelElem(Elem);

    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    // мощность - максимум
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;

    TSet result(maxPower);

    // И битовых полей
    result.BitField = BitField & s.BitField;

    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);

    // Инверсия
    result.BitField = ~BitField;

	// Обнуляем биты, выходящие за пределы MaxPower
    for (int i = MaxPower; i < result.BitField.GetLength(); i++) {
        result.BitField.ClrBit(i);
    }

    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int elem;

    while (istr >> elem) {
        if (elem >= 0 && elem < s.MaxPower) {
            s.InsElem(elem);
        }
    }

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{ ";
	bool first = true;  // флаг для запятых

    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }

    ostr << " }";
    return ostr;
}
