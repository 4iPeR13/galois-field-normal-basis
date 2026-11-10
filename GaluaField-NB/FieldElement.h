#pragma once
#include<iostream>
#include<vector>

const int FIELD_SIZE = 191;
const int ARRAY_SIZE = FIELD_SIZE / 64 + 1;

const int POLYNOMIAL_DEGREES[5] = { 0, 9, 191 };



class FieldElement {
public:
	uint64_t coefficients[FIELD_SIZE / 64 + 1] = {0};

	FieldElement();

	FieldElement(const FieldElement& other);

	FieldElement(std::string binary);

	FieldElement operator + (const FieldElement& other); 

	FieldElement operator * (const FieldElement& other);

	FieldElement RightCycleShift();

	FieldElement LeftCycleShift();

	FieldElement square();

	FieldElement trace();

	bool operator == (const FieldElement& other);

	FieldElement power(FieldElement& other);

	FieldElement inv();

	FieldElement operator & (const FieldElement& other);

	void set_bit(int index);
	const int get_bit(int index);

	int weight();

	friend std::ostream& operator<<(std::ostream& os, const FieldElement& element);
};

FieldElement* calculateMultiplicativeMatrix();


FieldElement& one();

FieldElement& zero();

static FieldElement* multiplicativeMatrix = calculateMultiplicativeMatrix();

FieldElement ConvertToNumber(std::string binary);

std::string ConvertToBinary(const FieldElement& element);
