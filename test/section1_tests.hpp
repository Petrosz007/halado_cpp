#pragma once

#include <cassert>
#include <original_tests/f1.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sut.hpp>
#include <utility>

TEST_CASE("[S1] Original Section 1 tests") { F1_operations(); }

#ifndef NO_FAIL
TEST_CASE("[S1] Expect fails") {
  SECTION("EXPECT FAIL: Do not allow update through operator[].") {
    GIVEN("string_id_bimap with one element") {
      string_id_bimap SM;
      SM.insert("gsd");

      THEN("EXPECT FAIL: Do not allow update through operator[].") {
        SM[0] = "not-gsd";
      }
    }
  }
}
#endif

TEST_CASE("[S1] Exceptions") {
  GIVEN("string_id_bimap") {
    string_id_bimap SM;

    WHEN("accessing an invalid value") {
      THEN("throw exception, unreachable") {
        REQUIRE_THROWS_AS(SM["gsd"], std::domain_error);
      }
    }

    WHEN("accessing an invalid ID") {
      THEN("throw exception, unreachable") {
        REQUIRE_THROWS_AS(SM[8], std::out_of_range);
      }
    }
  }
}

TEST_CASE("[S1] Insert and Query") {
  GIVEN("empty string_id_bimap") {
    string_id_bimap SM;
    std::string G{"gsd"};

    THEN("it should be empty") {
      REQUIRE(SM.size() == 0);
      REQUIRE(SM.empty());
    }

    WHEN("inserting and querying an element") {
      auto [iterator, did_insert] = SM.insert("gsd");

      THEN("the return value is correct") {
        REQUIRE(did_insert);
        REQUIRE(iterator->first == 0);
        REQUIRE(iterator->second == "gsd");
      }

      auto S1 = SM[0];
      auto S2 = SM["gsd"];

      THEN("the element is inserted correctly") {
        REQUIRE(S1 == "gsd");
        REQUIRE(S2 == 0);
        REQUIRE(SM.size() == 1);
        REQUIRE(!SM.empty());
      }
    }
  }

  GIVEN("string_id_bimap with one element") {
    string_id_bimap SM;
    std::string G{"gsd"};
    SM.insert("gsd");

    WHEN("inserting and querying another element") {
      std::string W{"Whisperity"};
      auto [iterator, did_insert] = SM.insert(W);

      THEN("the return value is correct") {
        REQUIRE(did_insert);
        REQUIRE(iterator->first == 1);
        REQUIRE(iterator->second == W);
        REQUIRE(iterator->second == "Whisperity");
      }

      AND_THEN("querying is idempotent") {
        REQUIRE(SM[SM[W]] == W);
        REQUIRE(SM[SM[1]] == 1);
      }

      AND_THEN("there is still no third element") {
        REQUIRE(SM.size() == 2);
        REQUIRE_THROWS_AS(SM[2], std::out_of_range);
      }

      AND_WHEN("trying a non-unique insertion") {
        auto [iterator2, did_insert2] = SM.insert(G);

        THEN("the element is not inserted") {
          REQUIRE_FALSE(did_insert2);
          REQUIRE(iterator2->first == 0);
          REQUIRE(iterator2->second == "gsd");
          REQUIRE(SM.size() == 2);
        }
      }
    }
  }
}

TEST_CASE("[S1] Copy") {
  GIVEN("string_id_bimap with 2 elements") {
    string_id_bimap SM;
    SM.insert("gsd");
    SM.insert("Whisperity");

    WHEN("copying and clearing the map") {
      string_id_bimap SM2 = SM;
      SM2.clear();

      THEN("the new map should be cleared, the old should remain filled") {
        REQUIRE(SM2.size() == 0);
        REQUIRE(SM2.empty());
      }
    }

    WHEN("copying the map") {
      const string_id_bimap CSM = SM;

      THEN("it should copy the elements correctly") {
        REQUIRE(CSM.size() == 2);
        REQUIRE(!CSM.empty());
        REQUIRE(CSM["gsd"] == 0);
        REQUIRE(CSM["Whisperity"] == 1);
        REQUIRE_THROWS_AS(CSM[CSM["Xazax"]], std::domain_error);
      }
    }
  }
}

TEST_CASE("[S1] Find") {
  GIVEN("string_id_bimap with 2 elements") {
    string_id_bimap CSM;
    CSM.insert("gsd");
    CSM.insert("Whisperity");

    WHEN("finding present elements") {
      auto F1 = CSM.find("Whisperity");
      auto F2 = CSM.find("gsd");

      THEN("they are correctly found") {
        REQUIRE(F1 != CSM.end());
        REQUIRE(F1->first == 1);
        REQUIRE(F1->second == "Whisperity");

        REQUIRE(F2 != CSM.end());
        REQUIRE(F2->first == 0);
        REQUIRE(F2->second == "gsd");
      }
    }

    WHEN("finding a not present element") {
      auto F3 = CSM.find("Xazax");

      THEN("it is not found") { REQUIRE(F3 == CSM.end()); }
    }
  }
}

TEST_CASE("[S1] Iterator") {
  GIVEN("string_id_bimap with 2 elements") {
    std::string W{"Whisperity"};
    std::string G{"gsd"};

    string_id_bimap CSM;
    CSM.insert("gsd");
    CSM.insert("Whisperity");

    WHEN("iterating over the map") {
      unsigned short Idx = 0;
      for (const auto& [id, value] : CSM) {
        if (Idx == 0 && id == Idx) {
          ++Idx;
          REQUIRE(value == G);
        } else if (Idx == 1 && id == Idx) {
          ++Idx;
          REQUIRE(value == W);
        } else {
          INFO("Expected only 2 elements in the copy!")
          REQUIRE(false);
        }
      }
    }
  }
}

TEST_CASE("[S1] Erase") {
  GIVEN("string_id_bimap and copy with 2 elements") {
    std::string W{"Whisperity"};
    std::string G{"gsd"};

    string_id_bimap SM;
    SM.insert("gsd");
    SM.insert("Whisperity");

    const string_id_bimap CSM = SM;

    WHEN("erasing an element by value") {
      SM.erase("gsd");

      THEN("it is erased correctly") {
        REQUIRE(SM.size() == 1);
        REQUIRE(CSM.size() == 2);
        REQUIRE(SM[0] == W);
        REQUIRE(CSM[0] == G);
      }

      AND_WHEN("erasing an element by ID") {
        SM.erase(0);

        THEN("it is erased correctly") {
          REQUIRE(SM.empty());
          REQUIRE(CSM.size() == 2);
        }
      }
    }
  }
}

TEST_CASE("[S1] Initialization list") {
  WHEN("initializing an id map with a list") {
    string_id_bimap SMInit{"gsd", "Whisperity", "Bjarne", "Herb"};

    THEN("it is initialized correctly") {
      std::ostringstream OSS;
      for (const auto& [_, value] : SMInit) OSS << value << ", ";
      REQUIRE(OSS.str() == "gsd, Whisperity, Bjarne, Herb, ");
    }
  }
}
