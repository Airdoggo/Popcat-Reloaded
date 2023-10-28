#include "tables.hh"

#include <iostream>
namespace board
{
    Tables::Tables()
    {
        generate_rook_table();
        generate_bishop_table();
    }

    Tables::~Tables()
    {
        for (int i = 0; i < 64; i++)
        {
            delete[] rook_attack_table[i];
            delete[] bishop_attack_table[i];
        }
    }

    static Bitboard generate_blockers_from_index(unsigned index, Bitboard mask)
    {
        Bitboard blockers = 0x0;
        unsigned long long nb_bits = __popcnt64(mask);
        unsigned long bit_pos;

        for (int i = 0; i < nb_bits; i++)
        {
            _BitScanForward64(&bit_pos, mask);

            if (index & (1 << i))
            {
                blockers |= (1ULL << bit_pos);
            }

            mask ^= 1ULL << bit_pos;
        }

        return blockers;
    }

    static Bitboard generate_rook_attack(unsigned rook_index, Bitboard blockers)
    {
        Bitboard result = 0x0, position = 0x0;
        unsigned rank = rook_index / 8;
        unsigned file = rook_index % 8;

        for (unsigned i = 1; i <= 7 - file && !(position & blockers); i++)
        {
            position = 1ULL << (rook_index + i);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= file && !(position & blockers); i++)
        {
            position = 1ULL << (rook_index - i);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= 7 - rank && !(position & blockers); i++)
        {
            position = 1ULL << (rook_index + i * 8);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= rank && !(position & blockers); i++)
        {
            position = 1ULL << (rook_index - i * 8);
            result |= position;
        }

        return result;
    }

    void Tables::generate_rook_table()
    {
        for (int rook_index = 0; rook_index < 64; rook_index++)
        {
            unsigned nb_permutations = 1 << rook_magic_bits[rook_index];
            rook_attack_table[rook_index] = new Bitboard[nb_permutations];

            for (unsigned blockers_index = 0; blockers_index < nb_permutations; blockers_index++)
            {
                Bitboard blockers = generate_blockers_from_index(blockers_index, rook_masks[rook_index]);
                Bitboard key = (blockers * rook_magic_number[rook_index]) >> (64 - rook_magic_bits[rook_index]);

                rook_attack_table[rook_index][key] = generate_rook_attack(rook_index, blockers);
            }
        }
    }

    static Bitboard generate_bishop_attack(unsigned bishop_index, Bitboard blockers)
    {
        Bitboard result = 0x0, position = 0x0;
        unsigned rank = bishop_index / 8;
        unsigned file = bishop_index % 8;

        unsigned sw_length = __min(file, rank);
        unsigned se_length = __min(7 - file, rank);
        unsigned nw_length = __min(file, 7 - rank);
        unsigned ne_length = __min(7 - file, 7 - rank);

        for (unsigned i = 1; i <= sw_length && !(position & blockers); i++)
        {
            position = 1ULL << (bishop_index - i * 9);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= se_length && !(position & blockers); i++)
        {
            position = 1ULL << (bishop_index - i * 7);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= nw_length && !(position & blockers); i++)
        {
            position = 1ULL << (bishop_index + i * 7);
            result |= position;
        }

        position = 0x0;
        for (unsigned i = 1; i <= ne_length && !(position & blockers); i++)
        {
            position = 1ULL << (bishop_index + i * 9);
            result |= position;
        }

        return 1;
    }

    void Tables::generate_bishop_table()
    {
        for (int bishop_index = 0; bishop_index < 64; bishop_index++)
        {
            unsigned nb_permutations = 1 << bishop_magic_bits[bishop_index];
            bishop_attack_table[bishop_index] = new Bitboard[nb_permutations];

            for (unsigned blockers_index = 0; blockers_index < nb_permutations; blockers_index++)
            {
                Bitboard blockers = generate_blockers_from_index(blockers_index, bishop_masks[bishop_index]);
                Bitboard key = (blockers * bishop_magic_number[bishop_index]) >> (64 - bishop_magic_bits[bishop_index]);

                bishop_attack_table[bishop_index][key] = generate_bishop_attack(bishop_index, blockers);
            }
        }
    }
} // namespace board