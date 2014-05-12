#include <LiquidCrystal.h>
#include "Arduino.h"
#include <StandardCplusplus.h>
#include "genetic_algorithm.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned int currentGeneration = 0;
bool done = false;

void setup() {
    lcd.begin(16, 2);

    randomSeed(analogRead(0));

    init_population(pop_alpha, pop_beta);
    population = &pop_alpha;
    buffer = &pop_beta;
}

void loop() {
    if (done == false) {
        if (currentGeneration < MAX_GENERATIONS) {
            calc_fitness(*population);
            sort_by_fitness(*population);
            if ((*population)[0].fitness == 0) {
                done = true;
                lcd.setCursor(0, 1);
                lcd.print("FOUND");
            } else {
                mate(*population, *buffer);
                swap(population, buffer);
                currentGeneration++;
            }
            updateDisplay();
        } else {
            done = true;
            lcd.setCursor(0, 1);
            lcd.print("FAILED");
        }
    }
}

void updateDisplay(void) {
    lcd.setCursor(0, 0);
    lcd.print((*population)[0].gene.c_str());

    lcd.setCursor(12, 0);
    int currentFitness = (*population)[0].fitness;
    char curretnFitnessLabel[4];
    sprintf(curretnFitnessLabel, "%04d", currentFitness);
    lcd.print(curretnFitnessLabel);

    lcd.setCursor(10, 1);
    char currentGenerationLabel[6];
    sprintf(currentGenerationLabel, "%06d", currentGeneration);
    lcd.print(currentGenerationLabel);
}