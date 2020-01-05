//
// fast_float_sign_extract
// (c) 2019 Andrzej Budzanowski <kontakt@andrzej.budzanowski.pl>
//

#pragma once

#include <algorithm>
#include <random>

template <typename T>
    class ring_buff
    {
        public:
            ring_buff(T from, T to)
            {
                std::random_device rd;
                auto gen = std::mt19937_64{rd()};
                auto dit = std::uniform_real_distribution<>{from, to};

                std::generate(std::begin(values), std::end(values), [&]() {
                    return dit(gen);
                });
            }

            T get()
            {
                int tmp = n++;
                return values[tmp & (size - 1)];
            }

            void get_8(T * ptr)
            {
                // we assume that people that use this method, will only use this method and not the other one
                *(ptr+0) = values[n++];
                *(ptr+1) = values[n++];
                *(ptr+2) = values[n++];
                *(ptr+3) = values[n++];
                *(ptr+4) = values[n++];
                *(ptr+5) = values[n++];
                *(ptr+6) = values[n++];
                *(ptr+7) = values[n++];

                n &= size;
            }

        private:
            int n = 0;
            static constexpr int size = 1024;
            T values[size] = { };
    };