#pragma once

#include <original_tests/f2.hpp>
#include <sut.hpp>

TEST_CASE("[S2] Original Section 2 tests") { F2_advanced(); }

TEST_CASE("[S2] All Tests") {
  SMFCounter::reset();

  id_bimap<SMFCounter> SMFM;
  auto [ER1_it, ER1_inserted] = SMFM.emplace(8);  // +1 construction.
  REQUIRE(SMFM.size() == 1);
  REQUIRE(ER1_inserted);
  REQUIRE(ER1_it->first == 0);
  REQUIRE(ER1_it->second.id() == 8);

  REQUIRE(SMFCounter::Ctor == 1);
  REQUIRE(SMFCounter::CCtor == 0);
  REQUIRE(SMFCounter::MCtor == 0);
  REQUIRE(SMFCounter::CAsg == 0);
  REQUIRE(SMFCounter::MAsg == 0);
  REQUIRE(SMFCounter::Dtor == 0);

  REQUIRE(SMFM.find(4) == SMFM.end());  // +1 construction, +1 destruction.

  REQUIRE(SMFCounter::Ctor == 2);
  REQUIRE(SMFCounter::CCtor == 0);
  REQUIRE(SMFCounter::MCtor == 0);
  REQUIRE(SMFCounter::CAsg == 0);
  REQUIRE(SMFCounter::MAsg == 0);
  REQUIRE(SMFCounter::Dtor == 1);

  REQUIRE(SMFM[0u].id() == 8);  // Index-based lookup, no ctor/dtor.

  REQUIRE(SMFCounter::Ctor == 2);
  REQUIRE(SMFCounter::CCtor == 0);
  REQUIRE(SMFCounter::MCtor == 0);
  REQUIRE(SMFCounter::CAsg == 0);
  REQUIRE(SMFCounter::MAsg == 0);
  REQUIRE(SMFCounter::Dtor == 1);

  SMFM.clear();  // +1 destruction.
  REQUIRE(SMFM.size() == 0);
  REQUIRE(SMFM.empty());

  REQUIRE(SMFCounter::CCtor == 0);
  REQUIRE(SMFCounter::MCtor == 0);
  REQUIRE(SMFCounter::CAsg == 0);
  REQUIRE(SMFCounter::MAsg == 0);
  REQUIRE(SMFCounter::Dtor == 2);

  id_bimap<non_copyable> USM;

#ifndef NO_FAIL
  // EXPECT FAIL: The mapped type is not copyable!
  id_bimap<non_copyable> USMC = USM;
#endif

  {
    id_bimap<non_copyable> X;
    // EXPECT PASS: The mapped type is moveable.
    id_bimap<non_copyable> USMM = std::move(X);
  }

  auto [ER2_it, ER2_inserted] =
      USM.emplace(std::make_unique<std::string>("Xazax"));
  REQUIRE(USM.size() == 1);
  REQUIRE(ER2_inserted);
  REQUIRE((*ER2_it->second) == "Xazax");

  for (int I = 0; I < 64; ++I) {
    auto [_, ER3_inserted] =
        USM.emplace(std::make_unique<std::string>(std::to_string(I)));
    REQUIRE(ER3_inserted == true);
  }
  REQUIRE(USM.size() == 1 + 64);

  const auto& CUSM = USM;
  auto FIR1 = CUSM.find_if([](auto&& E) -> bool { return *E == "Xazax"; });
  REQUIRE(FIR1 == CUSM.begin());
  REQUIRE(FIR1 != CUSM.end());

  USM.delete_all([](auto&& E) -> bool {
    try {
      if (int NumericValue = std::stoi(*E);
          NumericValue % 2 == 1)  // Odd number.
        return true;
      return false;
    } catch (const std::invalid_argument&) {
      return false;
    } catch (const std::out_of_range&) {
      return false;
    }

    return false;
  });

  // 1->64 has 32 even numbers. The "Xazax" shall be ignored by delete.
  REQUIRE(USM.size() == 1 + 32);

  int IndexAccumulator = 0;
  std::ostringstream OSS;
  for (const auto& E : USM) {
    IndexAccumulator += E.first;  // 0 + 1 + 2 + ... + 32
    OSS << *E.second << ", ";
  }
  REQUIRE(IndexAccumulator == ((32 * (32 + 1)) / 2));
  REQUIRE(OSS.str() ==
          "Xazax, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, "
          "32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, ");
}
