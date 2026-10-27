#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::string generate_username(std::mt19937& generator, int max_t, bool strict_limit,
                              bool include_number) {
    std::uniform_int_distribution<int> length_distribution(5, 10);
    int length = length_distribution(generator);
    if (strict_limit && max_t == 1) {
        // Alternating letters need an even length to end in tu or ty.
        std::uniform_int_distribution<int> pair_distribution(3, 5);
        length = 2 * pair_distribution(generator);
    }
    std::string name;
    int consecutive_t = 0;

    for (int i = 0; i < length; ++i) {
        std::vector<char> candidates;
        if (i == length - 1) {
            candidates = {'u', 'y'};
        } else if (i == length - 2) {
            // Always end the letter portion with "tu" or "ty".
            candidates = {'t'};
        } else {
            const int limit = strict_limit && i == length - 3 ? max_t - 1 : max_t;
            if (consecutive_t < limit) {
                candidates.push_back('t');
            }
            if (!name.empty() && name.back() == 't') {
                candidates.push_back('u');
            }
        }

        std::uniform_int_distribution<int> character_distribution(
            0, static_cast<int>(candidates.size()) - 1);
        const char next = candidates[character_distribution(generator)];
        consecutive_t = next == 't' ? consecutive_t + 1 : 0;
        name += next;
    }

    if (!include_number) {
        return name;
    }
    std::uniform_int_distribution<int> number_distribution(0, 999);
    std::ostringstream result;
    result << name << std::setfill('0') << std::setw(3)
           << number_distribution(generator);
    return result.str();
}

}  // namespace

int main(int argc, char* argv[]) {
    int max_t = 3;
    bool strict_limit = false;
    bool include_number = true;
    for (int i = 1; i < argc; ++i) {
        const std::string value = argv[i];
        if (value == "--help") {
            std::cout << "Usage: " << argv[0]
                      << " [max-consecutive-t] [--no-number]\n"
                      << "Specify a positive integer to limit consecutive t characters.\n"
                      << "--no-number  Omit the trailing three digits.\n";
            return 0;
        }
        if (value == "--no-number") {
            include_number = false;
            continue;
        }
        if (strict_limit) {
            std::cerr << "Specify max-consecutive-t only once.\n";
            return 1;
        }
        max_t = 0;
        for (char digit : value) {
            if (digit < '0' || digit > '9' ||
                max_t > (std::numeric_limits<int>::max() - (digit - '0')) / 10) {
                std::cerr << "max-consecutive-t must be a positive integer.\n";
                return 1;
            }
            max_t = max_t * 10 + (digit - '0');
        }
        if (max_t == 0) {
            std::cerr << "max-consecutive-t must be a positive integer.\n";
            return 1;
        }
        strict_limit = true;
    }
    std::random_device seed;
    std::mt19937 generator(seed());
    std::cout << "UserNameIs...\n"
              << generate_username(generator, max_t, strict_limit, include_number) << '\n';
}
