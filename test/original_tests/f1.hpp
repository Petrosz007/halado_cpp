#pragma once

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sut.hpp>
#include <utility>

void F1_operations() {
  string_id_bimap SM;

  assert(SM.size() == 0 && SM.empty());

  // Exceptions.
  try {
    SM["gsd"];
    assert(false && "Unreachable.");
  } catch (const std::domain_error&) {
  }

  try {
    auto S2 = SM[8];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {
  }

  // Insert and query.
  auto IR1 = SM.insert("gsd");
  assert(IR1.second);
  assert(IR1.first->first == 0 && IR1.first->second == "gsd");

  auto S1 = SM[0];
  assert(S1 == "gsd");
  auto S2 = SM["gsd"];
  assert(S2 == 0);

  assert(SM.size() == 1 && !SM.empty());

#ifndef NO_FAIL
  // EXPECT FAIL: Do not allow update through operator[].
  SM[0] = "not-gsd";
#endif

  // Support equality-based lookup, not identity-based.
  std::string W{"Whisperity"};

  auto IR2 = SM.insert(W);
  assert(IR2.second && IR2.first->first == 1 && IR2.first->second == W &&
         IR2.first->second == "Whisperity");
  // Idempotence.
  assert(SM[SM[W]] == W);
  assert(SM[SM[1]] == 1);
  try {
    SM[2];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {
  }

  assert(SM.size() == 2);

  // Non-unique insertion.
  std::string G{"gsd"};
  auto IR3 = SM.insert(G);
  assert(IR3.second == false && IR3.first->first == 0 &&
         IR3.first->second == "gsd");
  assert(SM.size() == 2);

  // Copy.
  const string_id_bimap CSM = SM;
  {
    string_id_bimap SM2 = SM;
    SM2.clear();
    assert(SM2.size() == 0 && SM2.empty());
  }

  // Proper copy!
  assert(CSM.size() == 2 && !CSM.empty() && CSM["gsd"] == 0 &&
         CSM["Whisperity"] == 1);
  try {
    CSM[CSM["Xazax"]];
    assert(false && "Unreachable.");
  } catch (const std::domain_error&) {
  }

  // Find.
  assert(CSM.find("Whisperity") != CSM.end());
  assert(CSM.find("Xazax") == CSM.end());
  assert(CSM.find("gsd") != CSM.end() && CSM.find("gsd")->second == "gsd");

  // Foreach iterator.
  unsigned short Idx = 0;
  for (const auto& E : CSM) {
    if (Idx == 0 && E.first == Idx) {
      ++Idx;
      assert(E.second == G);
    } else if (Idx == 1 && E.first == Idx) {
      ++Idx;
      assert(E.second == W);
    } else
      assert(false && "Expected only 2 elements in the copy!");
  }

  // Erase.
  SM.erase("gsd");
  assert(SM.size() == 1 && SM[0] == "Whisperity");
  SM.erase(0);
  assert(SM.empty());
  assert(CSM.size() == 2);

  // Initialisation list.
  // REWRITE: removed the assignment, my initializer list constructor is
  //          explicit
  string_id_bimap SMInit{"gsd", "Whisperity", "Bjarne", "Herb"};
  std::ostringstream OSS;
  for (const auto& E : SMInit) OSS << E.second << ", ";
  assert(OSS.str() == "gsd, Whisperity, Bjarne, Herb, ");
}
