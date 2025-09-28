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
    std::string getName() const override { 
        return "Dice[1," + std::to_string(max) + "]"; 
    }

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
    std::string getName() const override { 
        return "Penalty(" + dice->getName() + ")"; 
    }

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
    std::string getName() const override { 
        return "Bonus(" + dice->getName() + ")"; 
    }

private:
    std::unique_ptr<DiceBase> dice;
};

// РЕАЛИЗАЦИЯ 1: DoubleDice с множественным наследованием (с виртуальным наследованием)
class PenaltyDiceVirtual : virtual public DiceBase {
public:
    PenaltyDiceVirtual(std::unique_ptr<DiceBase> dice) 
        : dice(std::move(dice)) {}
    
    unsigned roll() override {
        unsigned roll1 = dice->roll();
        unsigned roll2 = dice->roll();
        return std::min(roll1, roll2);
    }

    unsigned getMinValue() const override { return dice->getMinValue(); }
    unsigned getMaxValue() const override { return dice->getMaxValue(); }
    std::string getName() const override { 
        return "PenaltyVirtual(" + dice->getName() + ")"; 
    }

private:
    std::unique_ptr<DiceBase> dice;
};

class BonusDiceVirtual : virtual public DiceBase {
public:
    BonusDiceVirtual(std::unique_ptr<DiceBase> dice) 
        : dice(std::move(dice)) {}
    
    unsigned roll() override {
        unsigned roll1 = dice->roll();
        unsigned roll2 = dice->roll();
        return std::max(roll1, roll2);
    }


    unsigned getMinValue() const override { return dice->getMinValue(); }
    unsigned getMaxValue() const override { return dice->getMaxValue(); }
    std::string getName() const override { 
        return "BonusVirtual(" + dice->getName() + ")"; 
    }

private:
    std::unique_ptr<DiceBase> dice;
};

class DoubleDiceMultipleInheritance : public PenaltyDiceVirtual, public BonusDiceVirtual {
public:
    DoubleDiceMultipleInheritance(Dice& dice) 
        : PenaltyDiceVirtual(std::make_unique<Dice>(dice.getMax())),
          BonusDiceVirtual(std::make_unique<Dice>(dice.getMax())) {}
    
    unsigned roll() override {
        unsigned penalty_roll = PenaltyDiceVirtual::roll();
        unsigned bonus_roll = BonusDiceVirtual::roll();
        return (penalty_roll + bonus_roll) / 2;
    }

    unsigned getMinValue() const override { 
        return PenaltyDiceVirtual::getMinValue(); 
    }
    
    unsigned getMaxValue() const override { 
        return BonusDiceVirtual::getMaxValue(); 
    }
    
    std::string getName() const override {
        return "DoubleDice(MultipleInheritance)";
    }
};

// РЕАЛИЗАЦИЯ 2: DoubleDice с композицией
class DoubleDiceComposition : public DiceBase {
public:
    DoubleDiceComposition(Dice& dice) 
        : penalty_dice(std::make_unique<PenaltyDice>(std::make_unique<Dice>(dice.getMax()))),
          bonus_dice(std::make_unique<BonusDice>(std::make_unique<Dice>(dice.getMax()))) {}
    
    unsigned roll() override {
        unsigned penalty_roll = penalty_dice->roll();
        unsigned bonus_roll = bonus_dice->roll();
        return (penalty_roll + bonus_roll) / 2;
    }

    unsigned getMinValue() const override { 
        return penalty_dice->getMinValue(); 
    }
    
    unsigned getMaxValue() const override { 
        return bonus_dice->getMaxValue(); 
    }
    
    std::string getName() const override {
        return "DoubleDice(Composition)";
    }

private:
    std::unique_ptr<PenaltyDice> penalty_dice;
    std::unique_ptr<BonusDice> bonus_dice;
};

// РЕАЛИЗАЦИЯ 3: DoubleDice альтернативная (самая эффективная)
class DoubleDiceAlternative : public DiceBase {
public:
    DoubleDiceAlternative(std::unique_ptr<DiceBase> dice) 
        : dice(std::move(dice)) {}
    
    unsigned roll() override {
        unsigned roll1 = dice->roll();
        unsigned roll2 = dice->roll();
        unsigned penalty = std::min(roll1, roll2);
        unsigned bonus = std::max(roll1, roll2);
        return (penalty + bonus) / 2;
    }

    unsigned getMinValue() const override { 
        return dice->getMinValue(); 
    }
    
    unsigned getMaxValue() const override { 
        return dice->getMaxValue(); 
    }
    
    std::string getName() const override {
        return "DoubleDice(Alternative)";
    }

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
if (prob < 0.001) continue;

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

// Функция для сравнения всех реализаций DoubleDice
void compare_all_double_dice_implementations() {
std::cout << "\n\n=== СРАВНЕНИЕ ВСЕХ РЕАЛИЗАЦИЙ DOUBLE DICE ===\n";

Dice dice100(100, 42);
const unsigned NUM_ROLLS = 50000;

std::cout << "\n1. МНОЖЕСТВЕННОЕ НАСЛЕДОВАНИЕ:\n";
DoubleDiceMultipleInheritance double_mi(dice100);
std::cout << "Мат. ожидание: " << expected_value(double_mi, NUM_ROLLS) << "\n";

std::cout << "\n2. КОМПОЗИЦИЯ:\n";
DoubleDiceComposition double_comp(dice100);
std::cout << "Мат. ожидание: " << expected_value(double_comp, NUM_ROLLS) << "\n";

std::cout << "\n3. АЛЬТЕРНАТИВНАЯ РЕАЛИЗАЦИЯ:\n";
auto double_alt = std::make_unique<DoubleDiceAlternative>(std::make_unique<Dice>(100, 42));
std::cout << "Мат. ожидание: " << expected_value(*double_alt, NUM_ROLLS) << "\n";

// Сравнение распределений
std::cout << "\n4. СРАВНЕНИЕ РАСПРЕДЕЛЕНИЙ ВЕРОЯТНОСТЕЙ:\n";
std::cout << "Значение |   MI   |  Comp  |  Alt   \n";
std::cout << "---------|--------|--------|--------\n";

for (unsigned value : {25, 50, 75}) {
double prob_mi = value_probability(value, double_mi, 10000);
double prob_comp = value_probability(value, double_comp, 10000);
double prob_alt = value_probability(value, *double_alt, 10000);

std::cout << std::setw(5) << value << "   | " 
<< std::setw(6) << std::fixed << std::setprecision(3) << prob_mi*100 << "% | "
<< std::setw(6) << prob_comp*100 << "% | "
<< std::setw(6) << prob_alt*100 << "%\n";
}

// Сравнение эффективности
std::cout << "\n5. СРАВНЕНИЕ ЭФФЕКТИВНОСТИ:\n";
std::cout << "• Множественное наследование: 4 броска на roll()\n";
std::cout << "• Композиция: 4 броска на roll()\n";
std::cout << "• Альтернативная: 2 броска на roll() - В 2 РАЗА ЭФФЕКТИВНЕЕ!\n";
}

// Функция для демонстрации гистограммы DoubleDice
void demonstrate_double_dice_histogram() {
std::cout << "\n\n=== ГИСТОГРАММА DOUBLE DICE [1, 100] ===\n";

Dice dice100(100, 42);

// Используем альтернативную реализацию для демонстрации
auto double_dice = std::make_unique<DoubleDiceAlternative>(std::make_unique<Dice>(100, 42));
print_probability_histogram(*double_dice, "DoubleDice Distribution", 100000);
}


int main() {
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА: МНОЖЕСТВЕННОЕ НАСЛЕДОВАНИЕ ===\n";
    
    // Сравниваем все реализации DoubleDice
    compare_all_double_dice_implementations();
    
    // Демонстрируем гистограмму
    demonstrate_double_dice_histogram();
    
    std::cout << "\n\n=== ВЫВОДЫ ===\n";
    std::cout << "1. Все три реализации дают схожие математические ожидания (~50.5)\n";
    std::cout << "2. Альтернативная реализация в 2 раза эффективнее\n";
    std::cout << "3. DoubleDice дает распределение, сконцентрированное в середине диапазона\n";
    std::cout << "4. Для практического использования рекомендуется альтернативная реализация\n";
    
    return 0;
}
