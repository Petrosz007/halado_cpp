#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

// IGNORE! Helper class for testing.
struct SMFCounter {
  static std::size_t Ctor, CCtor, MCtor, CAsg, MAsg, Dtor;
  static void reset() { Ctor = CCtor = MCtor = CAsg = MAsg = Dtor = 0; }

  SMFCounter() : ID(0) { ++Ctor; }
  SMFCounter(int V) : ID(V) { ++Ctor; }
  SMFCounter(const SMFCounter& R) : ID(R.ID) { ++CCtor; }
  SMFCounter(SMFCounter&& R) : ID(R.ID) { ++MCtor; }
  SMFCounter& operator=(const SMFCounter& R) {
    ID = R.ID;
    ++CAsg;
    return *this;
  }
  SMFCounter& operator=(SMFCounter&& R) {
    ID = R.ID;
    ++MAsg;
    return *this;
  }
  ~SMFCounter() { ++Dtor; }

  int id() const { return ID; }

  bool operator==(const SMFCounter& R) const { return ID == R.ID; }
  bool operator!=(const SMFCounter& R) const { return ID != R.ID; }
  bool operator<(const SMFCounter& R) const { return ID < R.ID; }
  bool operator<=(const SMFCounter& R) const { return ID <= R.ID; }
  bool operator>=(const SMFCounter& R) const { return ID >= R.ID; }
  bool operator>(const SMFCounter& R) const { return ID > R.ID; }

 private:
  int ID = 0;
};
std::size_t SMFCounter::Ctor, SMFCounter::CCtor, SMFCounter::MCtor,
    SMFCounter::CAsg, SMFCounter::MAsg, SMFCounter::Dtor;

// IGNORE! Helper type: something that's nice enough not to be copyable,
// but useful without a lot of hassle.
using non_copyable = std::unique_ptr<std::string>;

void F2_advanced() {
  SMFCounter::reset();

  id_bimap<SMFCounter> SMFM;
  auto ER1 = SMFM.emplace(8);  // +1 construction.
  assert(SMFM.size() == 1 && ER1.second == true && ER1.first->first == 0 &&
         ER1.first->second.id() == 8);
  assert(SMFCounter::Ctor == 1 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 0);

  assert(SMFM.find(4) == SMFM.end());  // +1 construction, +1 destruction.
  assert(SMFCounter::Ctor == 2 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 1);

  assert(SMFM[0u].id() == 8);  // Index-based lookup, no ctor/dtor.
  assert(SMFCounter::Ctor == 2 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 1);

  SMFM.clear();  // +1 destruction.
  assert(SMFM.size() == 0 && SMFM.empty());
  assert(SMFCounter::Ctor == 2 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 2);

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

  auto ER2 = USM.emplace(std::make_unique<std::string>("Xazax"));
  assert(USM.size() == 1 && ER2.second == true &&
         (*ER2.first->second) == "Xazax");

  for (int I = 0; I < 64; ++I) {
    auto ER3 = USM.emplace(std::make_unique<std::string>(std::to_string(I)));
    assert(ER3.second == true);
  }
  assert(USM.size() == 1 + 64);

  const auto& CUSM = USM;
  auto FIR1 = CUSM.find_if([](auto&& E) -> bool { return *E == "Xazax"; });
  assert(FIR1 == CUSM.begin());
  assert(FIR1 != CUSM.end());

  USM.delete_all([](auto&& E) -> bool {
    try {
      int NumericValue = std::stoi(*E);
      if (NumericValue % 2 == 1)  // Odd number.
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
  assert(USM.size() == 1 + 32);

  int IndexAccumulator = 0;
  std::ostringstream OSS;
  for (const auto& E : USM) {
    IndexAccumulator += E.first;  // 0 + 1 + 2 + ... + 32
    OSS << *E.second << ", ";
  }
  assert(IndexAccumulator == ((32 * (32 + 1)) / 2));
  assert(OSS.str() ==
         "Xazax, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, "
         "32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, ");
}
