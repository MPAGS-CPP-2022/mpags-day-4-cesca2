//! Unit Tests for MPAGSCipher PlayfairCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Test successive encryption and decryption", "[playfair]")
{
    PlayfairCipher pf{"testkey"};
    REQUIRE(pf.applyCipher(pf.applyCipher("HELLOWORLD", CipherMode::Encrypt), CipherMode::Decrypt) == "HELXLOWORLDZ");
}
