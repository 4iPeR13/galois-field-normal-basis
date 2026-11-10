#include "FieldElement.h"
#include <immintrin.h>

FieldElement::FieldElement()
{
	for (int i = 0; i <= FIELD_SIZE / 64; i++) {
		coefficients[i] = 0;
	}
}

FieldElement::FieldElement(const FieldElement& other)
{
	for (int i = 0; i <= FIELD_SIZE/64; i++) {
		coefficients[i] = other.coefficients[i];
	}
}

FieldElement::FieldElement(std::string binary)
{
	FieldElement other = ConvertToNumber(binary);
	*this = FieldElement(other);
}

FieldElement FieldElement::operator+(const FieldElement& other)
{
	FieldElement result;
	for (int i = 0; i <= FIELD_SIZE / 64; i++) {
		result.coefficients[i] = coefficients[i] ^ other.coefficients[i];
	}

	return result;
}

FieldElement FieldElement::operator*(const FieldElement& other)
{
	FieldElement result;
	FieldElement left(*this);
	FieldElement right(other);

	
	for (int i = 0; i < FIELD_SIZE; i++) {
		FieldElement temp;
		for (int j = 0; j < FIELD_SIZE; j++) {
			int bit = (left & multiplicativeMatrix[j]).weight() & 1;
			if (bit == 1) {
				temp.set_bit(j);
			}
		}
		temp = temp & right;
		if ((temp.weight() & 1) == 1) {
			result.set_bit(i);
		}
		left = left.LeftCycleShift();
		right = right.LeftCycleShift();
	}

	return result;
}

FieldElement FieldElement::RightCycleShift()
{
	FieldElement result;
	int right = FIELD_SIZE % 64;
	

	uint64_t temp = coefficients[ARRAY_SIZE - 1] >> (right - 1);
	temp = temp & 1;
	result.coefficients[0] = (coefficients[0] << 1) + temp;
	for (int i = 1; i < ARRAY_SIZE; i++) {
		result.coefficients[i] = (coefficients[i] << 1) + (coefficients[i - 1] >> 63);
	}
	result.coefficients[ARRAY_SIZE - 1] = result.coefficients[ARRAY_SIZE - 1] - (static_cast<uint64_t>(1) << right);
	return result;
}

FieldElement FieldElement::LeftCycleShift()
{
	FieldElement result;
	int right = FIELD_SIZE % 64;


	for (int i = 0; i < ARRAY_SIZE - 1; i++) {
		result.coefficients[i] = (coefficients[i] >> 1) + (coefficients[i + 1] << 63);
	}
	int temp = coefficients[0] & 1;

	result.coefficients[ARRAY_SIZE - 1] = (coefficients[ARRAY_SIZE - 1] >> 1) + (static_cast<uint64_t>(temp) << right - 1);

	return result;
}

FieldElement FieldElement::square()
{
	return this->RightCycleShift();
}

FieldElement FieldElement::trace()
{
	if ((weight() & 1) == 1) {
		return FieldElement(one());
	}
	return FieldElement();
	
}

bool FieldElement::operator==(const FieldElement& other)
{
	for (int i = 0; i <= FIELD_SIZE / 64; i++) {
		if (coefficients[i] == other.coefficients[i]) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}



FieldElement FieldElement::operator&(const FieldElement& other)
{
	FieldElement result;
	for (int i = 0; i <= FIELD_SIZE / 64; i++) {
		result.coefficients[i] = coefficients[i] & other.coefficients[i];
	}

	return result;
}


void FieldElement::set_bit(int index)
{
	coefficients[index / 64] ^= static_cast<unsigned long long>(1) << (index % 64);
}

const int FieldElement::get_bit(int index)
{
	return (coefficients[index / 64] >> (index % 64)) & 1;
}

int FieldElement::weight()
{
	int weight = 0;
	for (int i = 0; i <= FIELD_SIZE/64; i++) {
		weight += _mm_popcnt_u64(coefficients[i]);
	}
	return weight;
}

std::ostream& operator<<(std::ostream& os, const FieldElement& element)
{
	std::string res = ConvertToBinary(element);
	os << res;
	return os;
}

FieldElement* calculateMultiplicativeMatrix()
{
	static FieldElement multmatrix[FIELD_SIZE];
	int p = FIELD_SIZE * 2 + 1;
	int powers[FIELD_SIZE] = { 0 };
	powers[0] = 1;
	bool temp[FIELD_SIZE][FIELD_SIZE] = { false };
	for (int i = 1; i < FIELD_SIZE; i++) {
		powers[i] = 2 * powers[i - 1] % p;
	}
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = i; j < FIELD_SIZE; j++) {
			
			if ((powers[i] + powers[j]) % p == 1 ||
				(powers[i] + powers[j]) % p == p - 1 ||
				(powers[i] - powers[j]) % p == 1 ||
				(powers[i] - powers[j]) % p == -1) {
				temp[i][j] = true;
				temp[j][i] = true;
			}
		}
	}
	for (int i = 0; i < FIELD_SIZE; i++) {
		FieldElement column;
		for (int j = 0; j < FIELD_SIZE; j++) {
			if (temp[i][j]) {
				column.set_bit(j);
			}
		}
		multmatrix[i] = column;
	}
	return multmatrix;
}


FieldElement FieldElement::power(FieldElement& other)
{
	FieldElement result = one();
	for (int i = 0; i < FIELD_SIZE; i++) {
		uint64_t bit = other.get_bit(i);
		if (bit == 1) {
			result = *this * result;
		}
		if (i != FIELD_SIZE - 1) {
			result = result.square();
		}
	}
	return result;
}

FieldElement FieldElement::inv()
{
	FieldElement temp(*this);
	FieldElement result = one();
	for (int i = 0; i < FIELD_SIZE - 1; i++) {
		temp = temp.square();
		result = result * temp;
	}
	return result;

}

FieldElement& one()
{
	FieldElement* result = new FieldElement();
	for (int i = 0; i <  FIELD_SIZE / 64; i++) {
		result->coefficients[i] = UINT64_MAX;
	}
	result->coefficients[FIELD_SIZE / 64] = UINT64_MAX >> (64 - FIELD_SIZE % 64);
	return *result;
}
FieldElement& zero()
{
	FieldElement result;
	return result;
}



FieldElement ConvertToNumber(std::string binary)
{
	FieldElement result;
	for (int i = 0; i < FIELD_SIZE; i++) {
		if (binary.at(i) == '1') {
			result.coefficients[i / 64] += static_cast<unsigned long long>(1) << (i % 64);
		}
	}
	return result;
}

std::string ConvertToBinary(const FieldElement& element) {
	std::string binary = "";
	for (int i = 0; i < FIELD_SIZE; i++) {
		int bit = (element.coefficients[i / 64] >> (i % 64)) & 1;
		if (bit == 1) {
			binary += '1';
		}
		else {
			binary += '0';
		}
	}
	return binary;
}
