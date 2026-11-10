#include <iostream>
#include "FieldElement.h"
#include <stdlib.h>
#include <chrono>

FieldElement generateFieldElement() {
    std::string binary = "01";
    std::string result = "";
    for (int i = 0; i < FIELD_SIZE; i++) {
        result += binary[rand() % 2];
    }
    return FieldElement(result);
}

void measureTime(int numberOfMeasurements) {
    long long nanoseconds = 0;
    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();
        FieldElement second = generateFieldElement();

        const auto start{ std::chrono::high_resolution_clock::now() };
        first + second;
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for addition: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;

    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();
        FieldElement second = generateFieldElement();

        const auto start{ std::chrono::high_resolution_clock::now() };
        first* second;
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for multiplication: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;

    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();


        const auto start{ std::chrono::high_resolution_clock::now() };
        first.square();
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for square: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;

    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();
        FieldElement second = generateFieldElement();

        const auto start{ std::chrono::high_resolution_clock::now() };
        first.power(second);
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for power: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;

    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();
        FieldElement second = generateFieldElement();

        const auto start{ std::chrono::high_resolution_clock::now() };
        first.inv();
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for inverse: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;



    for (int i = 0; i < numberOfMeasurements; i++) {
        FieldElement first = generateFieldElement();
        FieldElement second = generateFieldElement();

        const auto start{ std::chrono::high_resolution_clock::now() };
        first.trace();
        const auto end{ std::chrono::high_resolution_clock::now() };
        const std::chrono::duration<int64_t, std::nano> elapsed_seconds{ end - start };
        nanoseconds += elapsed_seconds.count();
    }
    std::cout << "Elapsed nanoseconds for trace: " << nanoseconds / numberOfMeasurements << '\n';
    nanoseconds = 0;
}



int main()
{
    FieldElement f("00011101000110000010001111100111101011010000101010011011011111101100011100100111001011000110110101101111000100100011000011010011110011011010111100100111110000111010000110001001001111000010001");
    FieldElement g("01010000011011010100100110000001011100010001100100100111111111000000000011110100101110110001101000000001111001110011100011100010011110100000111000011010111011100001110110001001100001111011011");

    FieldElement h("01110101001101100001001100000111101011011100000110110010000000011110110110010110101001110110011100000100111010000011000101001011000110011000101000110110110011011101011010011000101110111101011");

    
    std::cout << "A = " << f << "\n\n";
    std::cout << "B = " << g << "\n\n";
    std::cout << "N = " << h << "\n\n";


    FieldElement add = f + g;
    std::cout << "A+B = " << add << "\n\n";

    FieldElement mult = f * g;
    std::cout << "A*B = " << mult << "\n\n";

    FieldElement square = f.square();
    std::cout << "A^2 = " << square << "\n\n";

    FieldElement pow = f.power(h);
    std::cout << "A^N = " << pow << "\n\n";

    FieldElement inv = f.inv();
    std::cout << "A^-1 = " << inv << "\n\n";

    FieldElement trace = f.trace();
    
    std::cout << "Tr(A) = " << trace << "\n\n";

    measureTime(100);
}

