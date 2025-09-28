#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

// Базовый класс для всех игральных костей
class DiceBase {
public:
    virtual ~DiceBase() = default;
    virtual unsigned roll() = 0;
    virtual unsigned getMinValue() const = 0;
    virtual unsigned getMaxValue() const = 0;
    virtual std::string getName() const { return "DiceBase"; }
};

// Класс Dice
class Dice : public DiceBase {
public:
    Dice(unsigned max, unsigned seed = std::random_device{}())
        : max(max), dstr(1, max), reng(seed) {}
    
    unsigned roll() override {
        return dstr(reng);
    }

    unsigned getMinValue() const override { return 1; }
    unsigned getMaxValue() const override { return max; }
    unsigned getMax() const { return max; }
    std::string getName() const override { return "Dice[1," + std::to_string(max) + "]"; }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

// Класс ThreeDicePool
class ThreeDicePool : public DiceBase {
public:
    ThreeDicePool(unsigned max, 
                  unsigned seed_1 = std::random_device{}(),
                  unsigned seed_2 = std::random_device{}(), 
                  unsigned seed_3 = std::random_device{}())
        : max(max), 
          dice1(std::make_unique<Dice>(max, seed_1)),
          dice2(std::make_unique<Dice>(max, seed_2)),
          dice3(std::make_unique<Dice>(max, seed_3)) {}

    unsigned roll() override {
        return dice1->roll() + dice2->roll() + dice3->roll();
    }

    unsigned getMinValue() const override { return 3; }
    unsigned getMaxValue() const override { return 3 * max; }
    std::string getName() const override { return "ThreeDicePool"; }

private:
    unsigned max;
    std::unique_ptr<Dice> dice1, dice2, dice3;
};

// Класс PenaltyDice
class PenaltyDice : public DiceBase {
public:
    PenaltyDice(std::unique_ptr<DiceBase> dice) 
        : dice(std::move(dice)) {}
    
    unsigned roll() override {
        unsigned roll1 = dice->roll();
        unsigned roll2 = dice->roll();
        return std::min(roll1, roll2);
    }

    unsigned getMinValue() const override { return dice->getMinValue(); }
    unsigned getMaxValue() const override { return dice->getMaxValue(); }
    std::string getName() const override { return "Penalty(" + dice->getName() + ")"; }

private:
    std::unique_ptr<DiceBase> dice;
};

// Класс BonusDice
class BonusDice : public DiceBase {
public:
    BonusDice(std::unique_ptr<DiceBase> dice) 
        : dice(std::move(dice)) {}
    
    unsigned roll() override {
        unsigned roll1 = dice->roll();
        unsigned roll2 = dice->roll();
        return std::max(roll1, roll2);
    }

    unsigned getMinValue() const override { return dice->getMinValue(); }
    unsigned getMaxValue() const override { return dice->getMaxValue(); }
    std::string getName() const override { return "Bonus(" + dice->getName() + ")"; }

private:
    std::unique_ptr<DiceBase> dice;
};

// Универсальная функция для вычисления математического ожидания
double expected_value(DiceBase& dice, unsigned number_of_rolls = 1000) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
        accum += dice.roll();
    }
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

// Функция для вычисления вероятности конкретного значения
double value_probability(unsigned value, DiceBase& dice, unsigned number_of_rolls = 10000) {
    unsigned count = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
        if (dice.roll() == value) {
            count++;
        }
    }
    return static_cast<double>(count) / static_cast<double>(number_of_rolls);
}


// Функция для построения гистограммы вероятностей
void print_probability_histogram(DiceBase& dice, const std::string& title, 
    unsigned number_of_rolls = 50000) {
std::cout << "\n" << title << " (" << number_of_rolls << " бросков):\n";
std::cout << std::string(50, '=') << "\n";

unsigned min_val = dice.getMinValue();
unsigned max_val = dice.getMaxValue();

// Собираем статистику
std::vector<unsigned> counts(max_val + 1, 0);
for (unsigned cnt = 0; cnt != number_of_rolls; cnt++) {
unsigned value = dice.roll();
if (value >= min_val && value <= max_val) {
counts[value]++;
}
}

// Выводим гистограмму
double max_prob = 0.0;
for (unsigned i = min_val; i <= max_val; i++) {
double prob = static_cast<double>(counts[i]) / number_of_rolls;
if (prob > max_prob) max_prob = prob;
}

for (unsigned i = min_val; i <= max_val; i++) {
double prob = static_cast<double>(counts[i]) / number_of_rolls;
if (prob < 0.001) continue; // Пропускаем очень редкие значения

unsigned bar_length = static_cast<unsigned>(prob * 40 / max_prob);

std::cout << std::setw(3) << i << ": " << std::setw(6) << std::fixed 
<< std::setprecision(3) << (prob * 100) << "% ";

for (unsigned j = 0; j < bar_length; j++) {
std::cout << "█";
}
std::cout << "\n";
}

double expected = expected_value(dice, number_of_rolls);
std::cout << "Мат. ожидание: " << std::fixed << std::setprecision(3) << expected << "\n";
}

// Функция для анализа всех трех типов бросков (ИСПРАВЛЕННАЯ)
void analyze_dice_behavior(std::unique_ptr<DiceBase> base_dice, const std::string& dice_name) {
std::cout << "\n\nАНАЛИЗ: " << dice_name << "\n";
std::cout << std::string(25, '=') << "\n";

// Сохраняем параметры базового кубика для создания новых экземпляров
unsigned max_value = base_dice->getMaxValue();

// Обычный бросок
print_probability_histogram(*base_dice, "Обычный бросок");

// Бросок со штрафом - создаем новый экземпляр
auto penalty_dice = std::make_unique<PenaltyDice>(std::make_unique<Dice>(max_value, 111));
print_probability_histogram(*penalty_dice, "Бросок со штрафом (Penalty)");

// Бросок с преимуществом - создаем новый экземпляр
auto bonus_dice = std::make_unique<BonusDice>(std::make_unique<Dice>(max_value, 222));
print_probability_histogram(*bonus_dice, "Бросок с преимуществом (Bonus)");
}

// Альтернативная функция анализа для ThreeDicePool
void analyze_three_dice_behavior() {
std::cout << "\n\nАНАЛИЗ: ThreeDicePool [3, 18]\n";
std::cout << std::string(30, '=') << "\n";

// Создаем отдельные экземпляры для каждого типа броска
auto normal_three = std::make_unique<ThreeDicePool>(6, 100, 200, 300);
auto penalty_three = std::make_unique<PenaltyDice>(
std::make_unique<ThreeDicePool>(6, 400, 500, 600));
auto bonus_three = std::make_unique<BonusDice>(
std::make_unique<ThreeDicePool>(6, 700, 800, 900));

print_probability_histogram(*normal_three, "Обычный бросок (ThreeDicePool)");
print_probability_histogram(*penalty_three, "Бросок со штрафом (ThreeDicePool)");
print_probability_histogram(*bonus_three, "Бросок с преимуществом (ThreeDicePool)");
}


int main() {
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА: ШТРАФЫ И ПРЕИМУЩЕСТВА ===\n";
    
    const unsigned NUM_ROLLS = 50000;
    
    // Анализ для Dice со значениями [1, 100]
    std::cout << "\n1. АНАЛИЗ ДЛЯ КУБИКА СО ЗНАЧЕНИЯМИ [1, 100]\n";
    std::cout << std::string(40, '=') << "\n";
    
    auto dice100 = std::make_unique<Dice>(100, 42);
    analyze_dice_behavior(std::move(dice100), "Dice [1, 100]");
    
    // Анализ для ThreeDicePool из трех кубиков [1, 6]
    analyze_three_dice_behavior();
    
    // Дополнительный анализ: сравнение математических ожиданий
    std::cout << "\n\n2. СРАВНЕНИЕ МАТЕМАТИЧЕСКИХ ОЖИДАНИЙ\n";
    std::cout << std::string(35, '=') << "\n";
    
    auto dice6 = std::make_unique<Dice>(6, 111);
    auto penalty_dice6 = std::make_unique<PenaltyDice>(std::make_unique<Dice>(6, 222));
    auto bonus_dice6 = std::make_unique<BonusDice>(std::make_unique<Dice>(6, 333));
    
    std::cout << "Для Dice [1, 6]:\n";
    std::cout << "Обычный бросок: " << expected_value(*dice6, NUM_ROLLS) << "\n";
    std::cout << "Со штрафом:     " << expected_value(*penalty_dice6, NUM_ROLLS) << "\n";
    std::cout << "С преимуществом:" << expected_value(*bonus_dice6, NUM_ROLLS) << "\n";
    
    // Теоретические расчеты
    std::cout << "\nТеоретические значения для Dice [1, 6]:\n";
    std::cout << "Обычный: " << (1 + 6) / 2.0 << "\n";
    std::cout << "Штраф:   ~2.53\n";
    std::cout << "Бонус:   ~4.47\n";
    
    return 0;
}