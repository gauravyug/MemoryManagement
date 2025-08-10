#include <vector>
#include <cstdint>
#include <iostream>

class BuddyBitmap {
public:
    std::vector<uint64_t> bits;

    BuddyBitmap(size_t bitCount) {
        bits.resize((bitCount + 63) / 64, UINT64_MAX); // initially all bits set
    }
    /*
    word size = 64 bits
    start     = 70
    len       = 20
           end         = start + len = 90
    firstWord   = start / 64 = 1       // starts in word 1
    lastWord    = (end - 1) / 64 = 1   // ends in same word
    startOffset = start % 64 = 6
    endOffset   = end % 64 = 26
    */
    void resetBits(size_t start, size_t len) {
        if (len == 0) return;

        size_t end = start + len;               // one past the last bit to clear
        size_t firstWord = start / 64;
        size_t lastWord  = (end - 1) / 64;

        size_t startOffset = start % 64;
        size_t endOffset   = end % 64;
        /*
        Apply mask for startOffset=6 to endOffset=26
        Mask in word 1:
        bits 6 → 25 cleared
        mask = ((1ULL << (26 - 6)) - 1) << 6
             = ((1ULL << 20) - 1) << 6
        */
        if (firstWord == lastWord) {
            // Range fits within a single 64-bit word
            uint64_t mask = ((uint64_t(1) << (endOffset - startOffset)) - 1) << startOffset;
            //We clear bits by ANDing with the invervse, That turns bits 6–25 into 0, leaving the rest unchanged.
            bits[firstWord] &= ~mask;
        } else {
            // First partial word
            /*
              ~0ULL = all bits set to 1 (64 ones).
              Shift left by startOffset to get:
                Zeros in positions 0..startOffset-1
              Ones in startOffset..63
              This mask selects everything from startOffset to the end of the word to clear.
              Invert (~mask) to keep the lower bits unchanged and clear the rest.
            */
            if (startOffset != 0) {
                uint64_t mask = ~0ULL << startOffset;
                bits[firstWord] &= ~mask;
                firstWord++;
            }

            // Full words in between
            for (size_t w = firstWord; w < lastWord; w++) {
                bits[w] = 0ULL;
            }

            // Last partial word
            if (endOffset != 0) {
                uint64_t mask = (uint64_t(1) << endOffset) - 1;
                bits[lastWord] &= ~mask;
            } else {
                bits[lastWord] = 0ULL;
            }
        }
    }

    void printBits(size_t bitCount) {
        for (int i = bitCount - 1; i >= 0; --i) {
            size_t word = i / 64;
            size_t bit  = i % 64;
            std::cout << ((bits[word] >> bit) & 1ULL);
        }
        std::cout << "\n";
    }
};
