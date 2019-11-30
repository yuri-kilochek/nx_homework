#include <nx_homework/hasher.hpp>

#include <vector>
#include <string>
#include <string_view>

#include <catch2/catch.hpp>

namespace {
    void append(nx_homework::hasher& hasher, std::string_view string) {
        hasher.append(string.data(), string.size());
    }

    auto finish(nx_homework::hasher& hasher)
    -> std::string
    {
        std::vector<char> digest;
        hasher.finish(digest);

        std::string hex_digest;
        for (unsigned char byte : digest) {
            constexpr
            auto digits = "0123456789ABCDEF";

            hex_digest.push_back(digits[(byte >> 4u) & 0xF]);
            hex_digest.push_back(digits[(byte      ) & 0xF]);
        }
        return hex_digest;
    }
}

TEST_CASE("compute hash", "[basic][hasher]") {
    nx_homework::hasher hasher;

    SECTION("of zero-length sequence") {
        REQUIRE(finish(hasher) == "E3B0C44298FC1C149AFBF4C8996FB924"
                                  "27AE41E4649B934CA495991B7852B855");
    }

    SECTION("of string 'foo'") {
        append(hasher, u8"foo");
        REQUIRE(finish(hasher) == "2C26B46B68FFC68FF99B453C1D304134"
                                  "13422D706483BFA0F98A5E886266E7AE");
    }

    SECTION("of string 'A quick brown fox jumps over a lazy dog.'") {
        append(hasher, u8"A quick brown fox jumps over a lazy dog.");
        REQUIRE(finish(hasher) == "85CC239BE2866B3DF18183632C3FC55C"
                                  "4C230FEBE15F0980DAC9D75E4BA9C4BE");
    }

    SECTION("of byte sequence '\x00\x01\x02\x03\x04'") {
        append(hasher, std::string_view("\x00\x01\x02\x03\x04", 5));
        REQUIRE(finish(hasher) == "08BB5E5D6EAAC1049EDE0893D30ED022"
                                  "B1A4D9B5B48DB414871F51C9CB35283D");
    }

    SECTION("of string 'A quick brow'"
            "followed by byte sequence '\xAB\xCD\xEF'"
            "followed by string ' a lazy dog.'")
    {
        append(hasher, u8"A quick brow");
        append(hasher, "\xAB\xCD\xEF");
        append(hasher, u8" a lazy dog.");
        REQUIRE(finish(hasher) == "C8315F28CC48097C641B8D70B903CCC6"
                                  "C33FAC25546CD7BB9A1C2E9FA53F75DC");
    }
}
