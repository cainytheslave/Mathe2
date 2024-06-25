#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <functional>
#include <iostream>
#include "../lib/CRandom.h"

using namespace Catch::Matchers;

TEST_CASE("CRandom test function", "[CRandom]") {
    SECTION("Test with same values for init") {
        CRandom r(42);
        r.test(3, 7, 10000);

        CRandom r2(42);
        r2.test(3, 7, 10000);
        
        CRandom r3(3);
        r3.test(3, 7, 10000);

        CRandom r4(7);
        r4.test(3, 7, 10000);

        CRandom r5(time(NULL));
        r3.test(3, 7, 10000);

        CRandom r6(time(NULL));
        r4.test(3, 7, 10000);

        CRandom r7(42);
        r7.test_wrong(3,7,10000);
    }
    
    
    SECTION("Test Poker hands", "[CRandom]") {
        CRandom r;
        std::function<std::vector<int>()> makeDeck = []{
            std::vector<int> deck(52, 0);
            for (int i = 0; i < 52; i++) {
                deck[i] = i;
            }
            return deck;
        };
        
        std::vector<int> ns = {1, 10, 100, 1000, 10000, 100000};
        
        int two_pairs_all = 0;
        int three_of_a_kind_all = 0;
        int four_of_a_kind_all = 0;
        int full_house_all = 0;
        int rounds = 0;

        for (int N : ns) {
            int two_pairs = 0;
            int three_of_a_kind = 0;
            int four_of_a_kind = 0;
            int full_house = 0;
            std::cout << "Testing with N = " << N << std::endl;
            for (int n = 0; n < N; n++) {
                rounds++;
                std::vector<int> deck = makeDeck();
                std::vector<int> hand;
                for (int i = 0; i < 7; i++) {
                    int j = r.nextInt(0, deck.size() - 1);
                    hand.push_back(deck[j]);
                    deck.erase(deck.begin() + j);
                }

                std::vector<int> values(13, 0);
                for (int i = 0; i < hand.size(); i++) {
                    values[hand[i] % 13]++;
                }

                std::sort(values.begin(), values.end());
                
                int best = values[12];
                int second_best = values[11];
                
                if (best >= 4)
                    four_of_a_kind++;
                else if (best >= 3 && second_best >= 2)
                    full_house++;
                else if (best >= 3)
                    three_of_a_kind++;
                else if (best >= 2 && second_best >= 2)
                    two_pairs++;
            }
            std::cout << "Two pairs: " << two_pairs << " ("
                << two_pairs / static_cast<float>(N) * 100 << "%)" << std::endl;
            std::cout << "Three of a kind: " << three_of_a_kind << " ("
                << three_of_a_kind / static_cast<float>(N) * 100 << "%)" << std::endl;
            std::cout << "Full house: " << full_house << " ("
                << full_house / static_cast<float>(N) * 100 << "%)" << std::endl;
            std::cout << "Four of a kind: " << four_of_a_kind << " ("
                << four_of_a_kind / static_cast<float>(N) * 100 << "%)" << std::endl;
            std::cout << std::endl;

            two_pairs_all += two_pairs;
            three_of_a_kind_all += three_of_a_kind;
            full_house_all += full_house;
            four_of_a_kind_all += four_of_a_kind;
        }

        std::cout << std::endl << "Played " << rounds << " rounds" << std::endl;
        std::cout << "Two pairs: " << two_pairs_all << " ("
            << two_pairs_all / static_cast<float>(rounds) * 100 << "%)" << std::endl;
        std::cout << "Three of a kind: " << three_of_a_kind_all << " ("
            << three_of_a_kind_all / static_cast<float>(rounds) * 100 << "%)" << std::endl;
        std::cout << "Full house: " << full_house_all << " ("
            << full_house_all / static_cast<float>(rounds) * 100 << "%)" << std::endl;
        std::cout << "Four of a kind: " << four_of_a_kind_all << " ("
            << four_of_a_kind_all / static_cast<float>(rounds) * 100 << "%)" << std::endl;
        std::cout << std::endl;
    }
}
