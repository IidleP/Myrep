// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "iostream"
#include "string"
#include "stdexcept"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    // исключение не работает, крашит код
    if (len <= 0) {
        throw "Error field len";
    }

    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;  
    pMem = nullptr;  // обнуление указателя на всякий случай
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    // n / бит в  TELEM
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	// 1 на позицию бита в TELEM
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    // Исключение тут работает
    if (n < 0 || n >= BitLen) {
        throw "Error bit index";
    }

    int element_index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);

    // или с маской
    pMem[element_index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Error bit index";
    }

    int element_index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);

	// и с отрицанием маски
    pMem[element_index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Error bit index";
    }

    int element_index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);

    // Бит есть? 0 или 1
    return (pMem[element_index] & mask) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    
    if (this != &bf) {
        delete[] pMem;

        BitLen = bf.BitLen;
        MemLen = bf.MemLen;

        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;  
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    // Длины равны?
    if (BitLen != bf.BitLen) return 0;

	// Данные равны?
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }

    return 1; // Все ок
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    // через ==
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{

	// макс длина для result
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

	// 1 обьект
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }

	// 2 обьет
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] |= bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	// макс длина для result
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    // мин длина 
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

	// И для общих элементов
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);

    // Инвертируем все биты
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }

	// обнуление битов, выходящих за пределы BitLen
    int bitsInLastElement = BitLen % (sizeof(TELEM) * 8);
    if (bitsInLastElement != 0) {
		// Маска для значимых битов
        TELEM mask = (TELEM(1) << bitsInLastElement) - 1;
		// Маска для последнего элемента
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string input;
    istr >> input;  // Читаем строку из потока

    // Проверяем соответствие длины строки и битового поля
    if (input.length() != bf.BitLen) {
        throw "Error str input";
    }

    // посимвольно
    for (int i = 0; i < bf.BitLen; i++) {
        char c = input[i];
        if (c == '1') {
            bf.SetBit(i);      
        }
        else if (c == '0') {
            bf.ClrBit(i);      
        }
        else {
            throw "Error char";
        }
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    // как строка вывод
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
