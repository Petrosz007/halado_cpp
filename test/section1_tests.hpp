#pragma once

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sut.hpp>
#include <utility>

#define NO_FAIL

// // Removes everything from the compilation
// #ifndef NO_FAIL
// void F1_operations() {
//   string_id_bimap SM;

//   assert(SM.size() == 0 && SM.empty());

//   // Exceptions.
//   try {
//     auto S1 = SM["gsd"];
//     assert(false && "Unreachable.");
//   } catch (const std::domain_error&) {
//   }

//   try {
//     auto S2 = SM[8];
//     assert(false && "Unreachable.");
//   } catch (const std::out_of_range&) {
//   }

//   // Insert and query.
//   auto IR1 = SM.insert("gsd");
//   assert(IR1.second);
//   assert(IR1.first->first == 0 && IR1.first->second == "gsd");

//   auto S1 = SM[0];
//   assert(S1 == "gsd");
//   auto S2 = SM["gsd"];
//   assert(S2 == 0);

//   assert(SM.size() == 1 && !SM.empty());

// #ifndef NFAIL
//   // EXPECT FAIL: Do not allow update through operator[].
//   SM[0] = "not-gsd";
// #endif

//   // Support equality-based lookup, not identity-based.
//   std::string W{"Whisperity"};

//   auto IR2 = SM.insert(W);
//   assert(IR2.second && IR2.first->first == 1 && IR2.first->second == W &&
//          IR2.first->second == "Whisperity");
//   // Idempotence.
//   assert(SM[SM[W]] == W);
//   assert(SM[SM[1]] == 1);
//   try {
//     SM[2];
//     assert(false && "Unreachable.");
//   } catch (const std::out_of_range&) {
//   }

//   assert(SM.size() == 2);

//   // Non-unique insertion.
//   std::string G{"gsd"};
//   auto IR3 = SM.insert(G);
//   assert(IR3.second == false && IR3.first->first == 0 &&
//          IR3.first->second == "gsd");
//   assert(SM.size() == 2);

//   // Copy.
//   const string_id_bimap CSM = SM;
//   {
//     string_id_bimap SM2 = SM;
//     SM2.clear();
//     assert(SM2.size() == 0 && SM2.empty());
//   }

//   // Proper copy!
//   assert(CSM.size() == 2 && !CSM.empty() && CSM["gsd"] == 0 &&
//          CSM["Whisperity"] == 1);
//   try {
//     CSM[CSM["Xazax"]];
//     assert(false && "Unreachable.");
//   } catch (const std::domain_error&) {
//   }

//   // Find.
//   assert(CSM.find("Whisperity") != CSM.end());
//   assert(CSM.find("Xazax") == CSM.end());
//   assert(CSM.find("gsd") != CSM.end() && CSM.find("gsd")->second == "gsd");

//   // Foreach iterator.
//   unsigned short Idx = 0;
//   for (const auto& E : CSM) {
//     if (Idx == 0 && E.first == Idx) {
//       ++Idx;
//       assert(E.second == G);
//     } else if (Idx == 1 && E.first == Idx) {
//       ++Idx;
//       assert(E.second == W);
//     } else
//       assert(false && "Expected only 2 elements in the copy!");
//   }

//   // Erase.
//   SM.erase("gsd");
//   // ?: This should be SM[0] here I guess
//   assert(SM.size() == 1 && CSM[0] == "Whisperity");
//   SM.erase(0);
//   assert(SM.empty());
//   assert(CSM.size() == 2);

//   // Initialisation list.
//   string_id_bimap SMInit = {"gsd", "Whisperity", "Bjarne", "Herb"};
//   std::ostringstream OSS;
//   for (const auto& E : SMInit) OSS << E.second << ", ";
//   assert(OSS.str() == "gsd, Whisperity, Bjarne, Herb, ");
// }

// TEST_CASE("[S1] Original Section 1 tests") { F1_operations(); }

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
      auto IR1 = SM.insert("gsd");

      THEN("the return value is correct") {
        REQUIRE(IR1.second);
        REQUIRE(IR1.first->first == 0);
        REQUIRE(IR1.first->second == "gsd");
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
      auto IR2 = SM.insert(W);

      THEN("the return value is correct") {
        REQUIRE(IR2.second);
        REQUIRE(IR2.first->first == 1);
        REQUIRE(IR2.first->second == W);
        REQUIRE(IR2.first->second == "Whisperity");
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
        auto IR3 = SM.insert(G);

        THEN("the element is not inserted") {
          REQUIRE_FALSE(IR3.second);
          REQUIRE(IR3.first->first == 0);
          REQUIRE(IR3.first->second == "gsd");
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

// TEST_CASE("[S1] Find") {
//   GIVEN("string_id_bimap with 2 elements") {
//     string_id_bimap CSM;
//     SM.insert("gsd");
//     SM.insert("Whisperity");

//     WHEN("finding present elements") {
//       auto F1 = CSM.find("Whisperity");
//       auto F2 = CSM.find("gsd");

//       THEN("they are correctly found") {
//         REQUIRE(F1 != CSM.end());
//         REQUIRE(F1->first == 0);
//         REQUIRE(F1->second == "Whisperity");

//         REQUIRE(F2 != CSM.end());
//         REQUIRE(F2->first == 1);
//         REQUIRE(F2->second == "gsd");
//       }
//     }

//     WHEN("finding a not present element") {
//       auto F3 = CSM.find("Xazax");

//       THEN("it is not found") { REQUIRE(F3 == CSM.end()); }
//     }
//   }
// }

TEST_CASE("[S1] Iterator") {
  GIVEN("string_id_bimap with 2 elements") {
    std::string W{"Whisperity"};
    std::string G{"gsd"};

    string_id_bimap CSM;
    CSM.insert("gsd");
    CSM.insert("Whisperity");

    WHEN("iterating over the map") {
      unsigned short Idx = 0;
      for (const auto& E : CSM) {
        if (Idx == 0 && E.first == Idx) {
          ++Idx;
          REQUIRE(E.second == G);
        } else if (Idx == 1 && E.first == Idx) {
          ++Idx;
          REQUIRE(E.second == W);
        } else {
          INFO("Expected only 2 elements in the copy!");
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

// TEST_CASE("[S1] Initialization list") {
//   WHEN("initializing an id map with a list") {
//     string_id_bimap SMInit = {"gsd", "Whisperity", "Bjarne", "Herb"};

//     THEN("it is initialized correctly") {
//       std::ostringstream OSS;
//       for (const auto& E : SMInit) OSS << E.second << ", ";
//       REQUIRE(OSS.str() == "gsd, Whisperity, Bjarne, Herb, ");
//     }
//   }
// }

#undef NOFAIL
