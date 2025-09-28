#include <iostream>
#include <random>
#include <vector>
#include <memory>

// Базовый класс для всех игральных костей
class DiceBase {
public:
    virtual ~DiceBase() = default;
    virtual unsigned roll() = 0;
};

// Класс Dice с наследованием от базового
class Dice : public DiceBase {
public:
    Dice(unsigned max, unsigned seed = std::random_device{}())
        : max(max), dstr(1, max), reng(seed) {}
    
    unsigned roll() override {
        return dstr(reng);
    }

    unsigned getMax() const { return max; }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

// Класс ThreeDicePool с использованием композиции Dice
class ThreeDicePool : public DiceBase {
public:
    ThreeDicePool(unsigned max, 
                  unsigned seed_1 = std::random_device{}(),
                  unsigned seed_2 = std::random_device{}(), 
                  unsigned seed_3 = std::random_device{}())
        : dice1(std::make_unique<Dice>(max, seed_1)),
          dice2(std::make_unique<Dice>(max, seed_2)),
          dice3(std::make_unique<Dice>(max, seed_3)) {}

    unsigned roll() override {
        return dice1->roll() + dice2->roll() + dice3->roll();
    }

private:
    std::unique_ptr<Dice> dice1, dice2, dice3;
};

// Универсальная функция для вычисления математического ожидания
double expected_value(DiceBase& dice, unsigned number_of_rolls = 1000) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
        accum += dice.roll();
    }
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

// Функция для вычисления дисперсии
double variance(DiceBase& dice, unsigned number_of_rolls = 1000) {
    double mean = expected_value(dice, number_of_rolls);
    double variance_accum = 0.0;
    
    for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
        double value = dice.roll();
        variance_accum += (value - mean) * (value - mean);
    }
    
    return variance_accum / number_of_rolls;
}

// Функция для построения гистограммы
void print_histogram(DiceBase& dice, unsigned number_of_rolls = 10000) {
    std::cout << "Гистограмма (" << number_of_rolls << " бросков):\n";
    
    // Для Dice максимальное значение известно, для ThreeDicePool вычислим
    unsigned max_possible = 6; // по умолчанию для демонстрации
    
    if (auto* single_dice = dynamic_cast<Dice*>(&dice)) {
        max_possible = single_dice->getMax();
    } else if (dynamic_cast<ThreeDicePool*>(&dice)) {
        max_possible = 18; // 3 * 6
    }
    
    std::vector<unsigned> counts(max_possible + 1, 0);
    
    for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
        unsigned value = dice.roll();
        if (value <= max_possible) {
            counts[value]++;
        }
    }
    
    for (unsigned i = 1; i <= max_possible; i++) {
        std::cout << i << ": ";
        unsigned bar_length = counts[i] * 50 / number_of_rolls;
        for (unsigned j = 0; j < bar_length; j++) {
            std::cout << "█";
        }
        std::cout << " " << (counts[i] * 100.0 / number_of_rolls) << "%\n";
    }
}
int main() {
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА: СЛУЧАЙНЫЕ ВЕЛИЧИНЫ ===\n\n";
    
    // Создаем экземпляры с фиксированными seed для воспроизводимости
    Dice dice6(6, 42);
    ThreeDicePool threeDice(6, 42, 123, 456);
    
    const unsigned NUM_ROLLS = 100000;
    
    std::cout << "1. ИССЛЕДОВАНИЕ ОДНОГО КУБИКА (Dice):\n";
    std::cout << "=====================================\n";
    
    double dice_expected = expected_value(dice6, NUM_ROLLS);
    double dice_variance = variance(dice6, NUM_ROLLS);
    double theoretical_dice_expected = (1 + 6) / 2.0;
    double theoretical_dice_variance = (6 * 6 - 1) / 12.0;
    
    std::cout << "Теоретическое мат. ожидание: " << theoretical_dice_expected << "\n";
    std::cout << "Экспериментальное мат. ожидание: " << dice_expected << "\n";
    std::cout << "Погрешность: " << std::abs(dice_expected - theoretical_dice_expected) << "\n\n";
    
    std::cout << "Теоретическая дисперсия: " << theoretical_dice_variance << "\n";
    std::cout << "Экспериментальная дисперсия: " << dice_variance << "\n";
    std::cout << "Погрешность: " << std::abs(dice_variance - theoretical_dice_variance) << "\n\n";
    
    print_histogram(dice6, 10000);
    
    std::cout << "\n2. ИССЛЕДОВАНИЕ ТРЕХ КУБИКОВ (ThreeDicePool):\n";
    std::cout << "============================================\n";
    
    double three_dice_expected = expected_value(threeDice, NUM_ROLLS);
    double three_dice_variance = variance(threeDice, NUM_ROLLS);
    double theoretical_three_expected = 3 * (1 + 6) / 2.0;
    double theoretical_three_variance = 3 * (6 * 6 - 1) / 12.0;
    
    std::cout << "Теоретическое мат. ожидание: " << theoretical_three_expected << "\n";
    std::cout << "Экспериментальное мат. ожидание: " << three_dice_expected << "\n";
    std::cout << "Погрешность: " << std::abs(three_dice_expected - theoretical_three_expected) << "\n\n";
    
    std::cout << "Теоретическая дисперсия: " << theoretical_three_variance << "\n";
    std::cout << "Экспериментальная дисперсия: " << three_dice_variance << "\n";
    std::cout << "Погрешность: " << std::abs(three_dice_variance - theoretical_three_variance) << "\n\n";
    
    print_histogram(threeDice, 10000);
    
    std::cout << "\n3. ДЕМОНСТРАЦИЯ ПОЛИМОРФИЗМА:\n";
    std::cout << "============================\n";
    
    std::vector<std::unique_ptr<DiceBase>> dice_collection;
    dice_collection.push_back(std::make_unique<Dice>(6, 111));
    dice_collection.push_back(std::make_unique<ThreeDicePool>(6, 222, 333, 444));
    dice_collection.push_back(std::make_unique<Dice>(10, 555)); // 10-гранный кубик
    
    std::cout << "Разные типы кубиков через единый интерфейс:\n";
    for (size_t i = 0; i < dice_collection.size(); i++) {
        double exp_val = expected_value(*dice_collection[i], 5000);
        std::cout << "Кубик " << (i + 1) << ": мат. ожидание = " << exp_val << "\n";
    }
    
    return 0;
}