#pragma once

#include <string>
#include <sut.hpp>
#include <type_traits>

#define NO_FAIL

#ifndef NO_FAIL
TEST_CASE("[S0] Expect fails") {
  SECTION("Expect fails") {
    SECTION("Template parameter 'value' must always be specified") {
      id_bimap<> A;
    }

    SECTION("Template parameter 'value' must always be specified.") {
      kchar_id_bimap<> B;
    }

    SECTION("Key must be integer!") { id_bimap<std::string, float> F; }

    SECTION("Key and value must be separate types.") {
      id_bimap<int, int> I;
      id_bimap<std::string, std::string> S;
      id_bimap<const std::string, std::string> S2;
    }
  }
}
#endif

TEST_CASE("[S0] Correct types") {
  struct T {};

  GIVEN("string bimap") {
    string_id_bimap SM;

    THEN("the mapped type is string") {
      REQUIRE(std::is_same_v<decltype(SM)::mapped_type, std::string>);
    }
  }

  GIVEN("bimap for struct T") {
    id_bimap<T> TM;

    THEN("key type is std::size_t and mapped type is T") {
      // ?: What the heck is this sizeof check?
      // REQUIRE(sizeof(decltype(TM)::key_type) == sizeof(&TM));
      // REWRITE: it expects the default key to be std::size_t
      REQUIRE(std::is_same_v<decltype(TM)::key_type, std::size_t>);
      REQUIRE(std::is_same_v<decltype(TM)::mapped_type, T>);
    }
  }

  GIVEN("bimap for key=short value=T") {
    id_bimap<T, short> STM;

    THEN("key type is short and mapped type is T") {
      REQUIRE(std::is_same_v<decltype(STM)::key_type, short>);
      REQUIRE(std::is_same_v<decltype(STM)::mapped_type, T>);
    }
  }

  GIVEN("kchat bimap for struct T") {
    kchar_id_bimap<T> CTM;

    THEN("key is char and mapped type is T") {
      REQUIRE(std::is_same_v<decltype(CTM)::key_type, char>);
      REQUIRE(std::is_same_v<decltype(CTM)::mapped_type, T>);
    }
  }
}

#undef NOFAIL
