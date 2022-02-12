# Library írás — ID BiMap 
## Bevezetés és tudnivalók

    ⚠️⚠️⚠️ EZ A FELADATKIÍRÁS EGYELŐRE CSAK DRAFT ÉS VÉGLEGESÍTÉSRE SZORUL! ⚠️⚠️⚠️

A félév során egy library rész implementációját kell elkészítsétek. Ez a feladat adja majd a tárgy értékelésének felét adó gyakorlati jegy legszignifikánsabb részét. Arra kérünk mindenkit, hogy álljon neki a feladatnak amikor csak tud. Nem szükséges vele "egy szuszra" sokat foglalkozni, inkább rendszeresen érdemes foglalkozni vele egy kicsit. Library-t készíteni nem egy éjszakás kaland és nem egy triviális feladat. Ez a feladat nem olyan, hogy a határidő előtti délutánon össze lehetne dobni!

✅ Bármilyen nyelvi és standard-könyvtári elem, amely egy hosszú stabilitású platformon — Debian 11, Ubuntu 20.04, stb. — vagy egy kellően elterjedt fordítóprogram verzióval — pl. GNU GCC 9, 10, illetve LLVM Clang 10, 11 — elérhető, szabadon felhasználható. Technikai okokból szeretnénk, ha nem kapnánk olyan beadandókat, amik Microsoft MSVC vagy a Windows-os STL implementáció "sajátosságaira" hagyatkoznak! (A feladat elég egyszerű ahhoz, hogy kevés erőforrás mellett egy virtuális gépben is elkészíthető!) Mivel a C++20 még nagyon új és számos funkciója csak elméletben, a "jogszabályban" létezik; a C++23 pedig csak a félév folyamán kerül hivatalosan véglegesítésre, ezért a C++17 standard szerinti megoldásokat várunk.

🚫 A feladat oktatási, tanulási jellege miatt arra kérünk, hogy a megoldást te magad gondold végig, és ne függj harmadik féltől származó – bármennyire híres vagy nemzetközileg elismert – library-től.

⛔️ TILOS a fordítóprogramok által kedvességből alapértelmezésben, vagy platform-specifikusan csak feltételesen engedélyezett, nem C++ szabványos nyelvi konstrukciókra — úgy, mint dinamikus méretű tömbök (variable length array (VLA)), méret-nélküli farok-tömbök (flexible array member (FAM)), a C11 standard "safe" kiterjesztése (Annex K) — vagy egyéb non-portable dolgokra — pl. inline assembly — hagyatkozás!

Az alábbi feladat több részből áll, amelyek sorrendben egymásra épülnek! Arra kérünk mindenkit, hogy a részek között sorban haladjon, és egy rész elérésének végén mentse el az állapotot. A beadáskor szeretnénk minden részt külön-külön megtekinteni, hogy a változásokat is lássuk. Javasolt egy tömörített archívumot (ZIP vagy TAR.GZ) beküldeni!

A feladathoz tartozik egy teszt kód, amelyet inline mellékelünk. Ez a tesztkód nem kőbe vésett — amennyiben a feladat megoldása közben úgy gondolod valami nem jó a designban és a te megoldásodhoz a tesztet is át kell fogalmazni, tedd meg nyugodtan, csak ezt jelöld. (Az nem elfogadható, hogy az üres programra redukáljuk a designt!) Úgy általában kommentezd a kódod bátran, megnézéskor nem csak a megoldást szeretnénk látni, hanem a benned felmerülő kérdéseket, és az arra kigondolt válaszokat! Ez a feladat most nem a tankönyvben szereplő hieroglifa-szekvenciának való tökéletes megfelelésről, hanem a kalandról, a kérdésekről, és a gondolkodás módjáról szól.

Mivel a tesztkód – az észszerűség keretein belül – szabadon módosítható, ezért megoldásként ne csak a feladat megvalósítását, hanem az általatok megváltoztatott teszt kódot együtt adjátok be!

Előfordulhat, hogy egyes, a feladatokban várt ismeret még nem hangzott el előadáson, vagy nem rendelkeztek vele. Ez esetben javasolt utánaolvasni néhány dolognak: az előadás slide-ok korábbi féléves változatai a honlapon elérhetők — ezek a félév előrehaladtával aktívan frissülnek! — vagy CppReference (Linkek egy külső oldalra)-en.

A tesztkódban lesznek // EXPECT FAIL megjelölésű részek, amelyeket az NFAIL makró hiánya véd – hasonlóan az NDEBUG-hoz. Az ilyen kódrészletek olyan konstrukciókat jelölnek, amelyeknek a helyes megoldás esetén nem szabad lefordulnia! Amennyiben megoldottátok, hogy ezekre a részekre hibát adjon a programotok, billentsétek be az NFAIL makrót, és úgy dolgozzatok tovább. Ekkor a célzott hibák már nem fognak előjönni.

## Nulladik rész: Általános összefoglaló

Gyakran előfordul, hogy az adatokat valamilyen könnyen létrehozható, automatikus kulcsokhoz szeretnénk rendelni, az egyszerűség kedvéért számokhoz. Gondoljunk itt pl. adatbázisrendszerekre (AUTOINCREMENT (Linkek egy külső oldalra)) vagy hálózati szerializációra. Bár egy programon belül a pointer tárolása jó indirekciós lehetőségeket biztosít (amennyiben az nem invalidálódik!), pointereket nem tudunk processzek között vagy időben különböző pillanatokra értelmesen megosztani.

A feladat egy kétirányú asszociatív adatszerkezet (map) megvalósítása. Jó C++ Standard Template Library osztály lévén természetesen az adatszerkezet template, két template paraméterrel. Legyen az adatszerkezetet megvalósító osztályunk neve id_bimap.

- A kulcs típusunkról feltételezhetjük, hogy incrementálható (++) és összehasonlítható, rendezhető (==, !=, <). Kötelezően egész szám típusú legyen. Adjunk fordítási időben értelmesen megfogalmazott hibaüzenetet, ha ezek a feltételek nem teljesülnek, pl. valaki float-okat kíván kulcsként használni. A gyakorlatban feltételezhetjük, hogy a kulcs típusunk "kellően pici", és olcsón másolható.
- Adjunk alapértelmezett értéket a "kulcs" típusparaméterünknek! Amennyiben a felhasználó nem specifikálja a kulcs típust, az legyen az adott platformon elérhető indexelésre használt típus. Ezt a standard library megfelelő típusának megnevezésével tedd!
- A mappelt érték típus bármi lehet. Mivel kétirányú asszociáció történik, egyelőre feltételezhetünk összehasonlíthatóságot és rendezhetőséget az érték típuson is. Nem kell törődnünk az ismétlődésekkel, egyedi értékek fognak az adatszerkezetbe kerülni. Az alapfeladatban feltehető, hogy az érték típus gond nélkül másolható. (Később ezen szigorítunk majd. 😉)
- Az érték típust a felhasználónak mindig specifikálnia kell, ne adjunk alapértelmezést.
- A kétirányú asszociáció irányainak egyértelműsége miatt tiltsuk le – újfent értelmesen megfogalmazott hibaüzenet mellett – azt is, hogy a kulcs és az érték típus ugyanaz legyen.

Az adatszerkezet legyen default konstruálható (ekkor nem rendelkezik hozzáadott elemekkel), értelmesen másolható, move-olható, és helyesen (kivételek és memory leak nélkül) destruálható.

ℹ️ Ügyelj a C++ STL container library konvenciói, az órán tanult "jó szokások", és úgy általában az objektum-orientált programozási paradigma szokásainak (pl. enkapszuláció) megtartására.

1. Biztosítsunk az osztályban típus-tagokat, amellyel a tesztkód fordítási időben "megkérdezheti", hogy egy adott változót milyen kulcs és érték típussal példányosítottak.
2. Készíts egy kchar_id_bimap típus-alias-sablont, amelyet a paraméter típusával, mint érték típussal, példányosítja az id_bimap-et, és a kulcs típust a char karakter típusa fixálja.
3. Készíts egy string_id_bimap típus-aliast, amellyel egyszerűen megnevezhetjük az id_bimap olyan példányosítását, amelyben az értékek stringek. A kulcs típus maradjon a id_bimap alapértelmezése szerinti.

```cpp
#include <string>
#include <type_traits>

void F0_types()
{
#ifndef NFAIL
  // EXPECT FAIL: Template parameter "value" must always be specified.
  id_bimap<> A;

  // EXPECT FAIL: Template parameter "value" must always be specified.
  kchar_id_bimap<> B;

  // EXPECT FAIL: Key must be integer!
  id_bimap<std::string, float> F;

  // EXPECT FAIL: Key and value must be separate types.
  id_bimap<int, int> I;
  id_bimap<std::string, std::string> S;
  id_bimap<const std::string, std::string> S2;
#endif

  string_id_bimap SM;
  static_assert(std::is_same_v<decltype(SM)::mapped_type, std::string>);

  struct T {};
  id_bimap<T> TM;
  static_assert(sizeof(decltype(TM)::key_type) == sizeof(&TM));
  static_assert(std::is_same_v<decltype(TM)::mapped_type, T>);

  id_bimap<T, short> STM;
  static_assert(std::is_same_v<decltype(STM)::key_type, short>);
  static_assert(std::is_same_v<decltype(STM)::mapped_type, T>);

  kchar_id_bimap<T> CTM;
  static_assert(std::is_same_v<decltype(CTM)::key_type, char>);
  static_assert(std::is_same_v<decltype(CTM)::mapped_type, T>);
}
```

## Első rész: Alapfeladat

Az adatszerkezetünk alapvető szemantikája a következő műveletekből áll össze. Koncepciójában nagyban hasonlítani szeretnénk a std::map (Linkek egy külső oldalra) viselkedésére, de a kulcsok kezelése miatt lesz némi eltérés. Összesen 7 jellegzetes alapműveletet készíts el, hogy egy felhasználók számára már hasznos adatszerkezeted legyen.

1. Legyen lehetőség új érték hozzáadására az adatszerkezetbe. Az új értékhez automatikusan rendelődjön hozzá az a kulcs, amely a meglévő legnagyobb kulcsnál egyel nagyobb. Tehát az elsőnek beszúrt elem a 0, a másodiknak beszúrt az 1, stb. kulcsot kapja.
Visszatérési értéke ennek a függvénynek a std::map::insert (Linkek egy külső oldalra)-éhez hasonló pár legyen, amely első komponense egy iterátor a beszúrt elemre, második komponense pedig egy logikai érték, amely azt mondja meg, történt-e már beszúrás.
Ne történjen beszúrás, ha az érték már megtalálható az adatszerkezetben.
2. Legyen lehetőség lekérdezni az indexelő operátor segítségével egy kulcshoz a társított értéket, és az értékhez a kulcsát. Amennyiben a lekérdezés nem sikerül, dobj kivételt. A két dobott kivétel legyen különböző típusú: érték alapú indexeléskor std::domain_error (Linkek egy külső oldalra), kulcs index alapú indexeléskor std::out_of_range (Linkek egy külső oldalra).
Ne biztosíts lehetőséget arra, hogy az indexelő operátor visszatérési értékén "keresztül" bármit módosíthasson a felhasználó.
3. Létezzenek a megszokott alapvető lekérdezések, pl. az aktuális méret, üresség; és az üressé tétel metódusa.
4. Legyen lehetőség egy elemet törölni, akár index, akár érték alapján. Törléskor a törölt elem megfelelően semmisüljön meg, és a törölt elemnél nagyobb indexű – időrendben később beszúrt – elemek lépjenek "balra", azaz ne keletkezzen "lyuk" az adatszerkezetben. Más szóval: mindig a LaTeX: \left[ 0\ \ldots\ n \right][0&ltdot;n] intervallumba eső indexek legyenek érvényesek, törlés után is.
Ha a törlő művelet érvénytelen paramétert kapott, pl. nem lefoglalt indexet, vagy olyan értéket, amely nem található, csendben (hibás viselkedés vagy kivétel nélkül) ne csináljon semmit.
5. Implementáld a container-eken megszokott find() műveletet, amely egy értéket vár. Ellenben az indexelő operátorral, a find() ne dobjon kivételt, ha nem találja meg a keresett értéket. Minden egyéb szempontból viselkedjen úgy, mint a std::map::find (Linkek egy külső oldalra), azaz megfelelő iterátort adjon vissza.
6. Biztosítsunk iterátort az adatszerkezetünkön, amellyel a hozzáadott kulcs-érték párok bejárhatók. Ne legyen az iterátoron keresztül módosítható a tárolt érték!
7. Készítsünk egy konstruktort, amely inicializációs listából hozza létre az adatszerkezetünk. Az inicializációs listában az érték-típusnak megfelelő objektumok találhatók, amelyek a megadott sorrendben hozzáadásra kerülnek.

```cpp
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

void F1_operations()
{
  string_id_bimap SM;

  assert(SM.size() == 0 && SM.empty());

  // Exceptions.
  try
  {
    auto S1 = SM["gsd"];
    assert(false && "Unreachable.");
  } catch (const std::domain_error&) {}

  try
  {
    auto S2 = SM[8];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {}

  // Insert and query.
  auto IR1 = SM.insert("gsd");
  assert(IR1.second);
  assert(IR1.first->first == 0 && IR1.first->second == "gsd");

  auto S1 = SM[0];
  assert(S1 == "gsd");
  auto S2 = SM["gsd"];
  assert(S2 == 0);

  assert(SM.size() == 1 && !SM.empty());

#ifndef NFAIL
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
  try
  {
    SM[2];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {}

  assert(SM.size() == 2);

  // Non-unique insertion.
  std::string G{"gsd"};
  auto IR3 = SM.insert(G);
  assert(IR3.second == false && IR3.first->first == 0 &&
         IR3.first->second == "gsd");
  assert(SM.size() == 2);

  // Copy.
  const string_id_bimap CSM = SM;

  SM.clear();
  assert(SM.size() == 0 && SM.empty());

  // Proper copy!
  assert(CSM.size() == 2 && !CSM.empty() && CSM["gsd"] == 0 &&
         CSM["Whisperity"] == 1);
  try
  {
    CSM[CSM["Xazax"]];
    assert(false && "Unreachable.");
  } catch (const std::domain_error&) {}

  // Find.
  assert(CSM.find("Whisperity") != CSM.end());
  assert(CSM.find("Xazax") == CSM.end());
  assert(CSM.find("gsd") != CSM.end() && CSM.find("gsd")->second == "gsd");

  // Foreach iterator.
  unsigned short Idx = 0;
  for (const auto& E : CSM)
  {
    if (Idx == 0 && E.first == Idx)
    {
      ++Idx;
      assert(E.second == G);
    }
    else if (Idx == 1 && E.first == Idx)
    {
      ++Idx;
      assert(E.second == W);
    } else
      assert(false && "Expected only 2 elements in the copy!");
  }

  // Erase.
  SM.erase("gsd");
  assert(SM.size() == 1 && CSM[0] == "Whisperity");
  SM.erase(0);
  assert(SM.empty());
  assert(CSM.size() == 2);

  // Initialisation list.
  string_id_bimap SMInit = {"gsd", "Whisperity", "Bjarne", "Herb"};
  std::ostringstream OSS;
  for (const auto& E : SMInit)
    OSS << E.second << ", ";
  assert(OSS.str() == "gsd, Whisperity, Bjarne, Herb, ");
}
```

## Második rész: Predikátumok és move-olás

Most már továbbfejleszthetjük a korábban elkészített adatszerkezetünket. Bár az alapmegoldás számos esetben elég jó, sajnos felmerülhetnek vele szemben teljesítménybeli kritikák. Egyszerűen fogalmazva: nem tökéletes, két szempontból. Egyrészt, könnyen előfordul, hogy az előző feladatot tökéletesen kielégítő de mégis naív megoldásban a kétirányú megfeleltetéshez kétszer került letárolásra az érték. Az érték-típus a felhasználótól jövő paraméter, általános esetben nem ismerjük a költségét a rajta végzett műveleteknek. Bár az LaTeX: \Theta\left(2n\right)Θ(2n) aszimptotikusan nem végzetes, ha úgy gondolunk rá, hogy tetszőleges 4 GiB memóriába csak 2 GiB adatot tudunk tenni, már nem hangzik túl kellemesnek.

Ezt mindenképp javítsuk ki, most már ne hagyatkozzunk arra, hogy megengedhetjük magunknak a memória pazarlását! Ezen kívül a következő további fejlesztéseket valósítsd meg az implementációdon:

1. A korábbi feltevés, miszerint az "érték típus gond nélkül másolható" nem csak költségében, hanem funkcionalitásban sem állja meg a helyét. Előfordul, hogy valamilyen nem másolható objektumot — legtöbbször rendszer-erőforrást, pl. thread-eket, hálózati kapcsolatok socket-jeit, megnyitott fájlokat, stb. —  próbálunk "beszámozni". Támogassuk ezt a megoldásunk mögöttes logikájában!
2. Amennyiben a felhasználótól kapott érték típus nem másolható, az egész adatszerkezetünk se legyen másolható. ⚠️ VIGYÁZZ! Az eddigi string esetben – és minden másolható érték-típusra – továbbra is másolható maradjon az adatszerkezetünk!
3. Az érték-típus másolhatatlansága megnehezíti az elkészített insert() dolgát, amely az értékeket bemásolta az adatszerkezetbe. Ne nyúlj az insert()-hez! Helyette készítsd el a "helyben példányosító" emplace() műveletet, amely az insert-tel megegyező típusú és jelentésű visszatérési értékkel rendelkezik.
4. Készítsünk el két predikátumot (egyváltozós, a tárolt értékeinket kapó, és logikai értékkel visszatérő) váró tagfüggvény-sablont is.
    1. A find_if viselkedjen a megszokott módon, visszaadva a bejárás sorrendjében predikátumra először illeszkedő elem iterátorát.
    2. A delete_all függvény törölje ki az összes elemet amelyre illeszkedik a kapott predikátum.
    
        ⚠️ VIGYÁZZ! A törlést követően ne legyenek "lyukak" az adatszerkezetben, hasonlóan az egy elem törlése művelethez. Figyelj arra, mi történik, ha esetleg bejárás közben törölsz elemeket!

```cpp
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

// IGNORE! Helper class for testing.
struct SMFCounter
{
  static std::size_t Ctor, CCtor, MCtor, CAsg, MAsg, Dtor;
  static void reset() { Ctor = CCtor = MCtor = CAsg = MAsg = Dtor = 0; }

  SMFCounter() : ID(0) { ++Ctor; }
  SMFCounter(int V) : ID(V) { ++Ctor; }
  SMFCounter(const SMFCounter& R) : ID(R.ID) { ++CCtor; }
  SMFCounter(SMFCounter&& R) : ID(R.ID) { ++MCtor; }
  SMFCounter &operator=(const SMFCounter& R)
  {
    ID = R.ID;
    ++CAsg;
    return *this;
  }
  SMFCounter &operator=(SMFCounter&& R)
  {
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

void F2_advanced()
{
  SMFCounter::reset();

  id_bimap<SMFCounter> SMFM;
  auto ER1 = SMFM.emplace(8); // +1 construction.
  assert(SMFM.size() == 1 && ER1.second == true && ER1.first->first == 0 &&
         ER1.first->second.id() == 8);
  assert(SMFCounter::Ctor == 1 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 0);

  assert(SMFM.find(4) == SMFM.end()); // +1 construction, +1 destruction.
  assert(SMFCounter::Ctor == 2 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 1);

  SMFM.clear(); // +1 destruction.
  assert(SMFM.size() == 0 && SMFM.empty());
  assert(SMFCounter::Ctor == 2 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 2);

  assert(SMFM[0u].id() == 8); // Index-based lookup, no ctor/dtor.

  id_bimap<non_copyable> USM;

#ifndef NFAIL
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

  for (int I = 0; I < 64; ++I)
  {
    auto ER3 = USM.emplace(std::make_unique<std::string>(std::to_string(I)));
    assert(ER3.second == true);
  }
  assert(USM.size() == 1 + 64);

  const auto& CUSM = USM;
  auto FIR1 = CUSM.find_if([](const auto&& E) -> bool { return *E == "Xazax"; });
  assert(FIR1 == CUSM.begin());
  assert(FIR1 != CUSM.end());

  USM.delete_all([](const auto&& E) -> bool
  {
    try
    {
      int NumericValue = std::stoi(*E);
      if (NumericValue % 2 == 0) // Even number.
        return true;
      return false;
    }
    catch (const std::invalid_argument&) { return false; }
    catch (const std::out_of_range&) { return false; }

    return false;
  });

  // 1->64 has 32 even numbers. The "Xazax" shall be ignored by delete.
  assert(USM.size() == 1 + 32);

  int IndexAccumulator = 0;
  std::ostringstream OSS;
  for (const auto& E : USM)
  {
    IndexAccumulator += E.first; // 0 + 1 + 2 + ... + 32
    OSS << *E.second << ", ";
  }
  assert(IndexAccumulator == ((32 * (32 - 1)) / 2));
  assert(OSS.str() ==
         "Xazax, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, "
         "34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, ");
}
```

## Harmadik rész: Logikai törlés a "lyukak" ellen

Néhány jelentős elemét a C++ nyelvnek és szokásainak azonban még mindig nem aknáztuk ki, az előző továbbfejlesztéssel együtt sem. Tanultunk az előadáson az "exception safety" fogalmáról. Gondoljuk meg, mi történik, ha konstruálás vagy beszúrás közben hibát kapunk. Előfordulhat-e, hogy valami csak félig kezelődik le? Oldjuk meg – saját implementációnk ismeretében, – hogy ez a lehető legkevesebb problémát okozza, ne essen szét az adatszerkezetünk, ha az elemtípus műveletei elhasalnának.

Vezessük be továbbá a "lyuk", azaz a logikai törlés fogalmát. Korábban probléma adódhatott abból, ha eltároltunk egy beszúrás során keletkezett indexet, de egy törléssel – és az ezt követő balra tömörítéssel – ezt az indexet invalidáltuk is. Például az "A", "B", "C" beszúrás után a kapott indexek 0, 1, és 2, de ha töröljük "A"-t, akkor a "B" indexének hitt 1 helyén már a "C" szerepel. Logikai törlés esetén csak megjelöljük egy indexet, hogy az szabad, de a többi elemet nem módosítjuk!

A logikai törlés bevezetése után a meglévő műveleteinket alakítsd át, hogy megfelelően kezeljék ezt a fogalmat.

- Bár egy elem helye logikailag töröltté válik a törlés hatására, az elemet magát megfelelően destruáljuk el, mivel előfordulhat, hogy komplex destruktora van, pl. ha rendszer-erőforrás, amit megfelelően fel kell szabadítanunk!
- Amennyiben indexeléskor az index létezik az adatszerkezetben, de a hozzá kapcsolt érték logikailag törölt, dobjunk kivételt oly módon, mintha a kért index nem is létezne.
- A kereső műveletek, az iterátor léptetése, a find_if és a delete_all ugorja át a logikailag törölt elemeket.
- A méret lekérdezésekor csak a "valódi" (értékkel rendelkező) elemek számát kapjuk meg.

A logikai törléssel való megfelelő együttélés, és apróbb optimalizációs célokból bevezetünk néhány új műveletet:

1. Vezessük be a capacity() függvényt, amely az összes, az adatszerkezetben tárolt elem-hely számát — tehát a foglalt és a logikailag töröltek együtt! — adja. Ez a két függvény műveletigénye legyen konstans, LaTeX: \Theta\left(1\right)Θ(1)!
2. Készítsük el a next_index() metódust, amely megadja azt az indexet, ahová egy képzeletbeli beszúrás az elemet tenné. Ez az index a legkisebb olyan index, amely logikailag törölt "lyukat" tartalmaz, vagy ha nincs ilyen, akkor a korábbi viselkedéssel megegyezően az új, 1-gyel nagyobb index.
3. Vezessünk be még egy is_contiguous() lekérdezést is, amely megmondja, hogy van-e logikailag törölt lyuk az adatszerkezetben.

```cpp
#include <cassert>
#include <stdexcept>
#include <string>
#include <type_traits>

void F3_logicalDelete()
{
  {
    id_bimap<char, short> SCM;
    static_assert(std::is_same_v<decltype(SCM.next_index()), short>,
                  "Return index!");
  }

  string_id_bimap SM;

  assert(SM.size() == 0 && SM.capacity() == 0 &&
         SM.next_index() == 0); // No elements yet, so 1st element is index 0.

  SM.insert("gsd");
  SM.insert("Whisperity");
  SM.insert("John");
  SM.insert("Hyrum");

  assert(SM.size() == 4 && SM.capacity() == 4);
  assert(SM.next_index() == 4); // [0, 1, 2, 3] are allocated indices.
  assert(SM.is_contiguous());

  SM.erase("gsd");

  assert(SM.size() == 3 && SM.capacity() == 4 && SM.next_index() == 0);
  assert(!SM.is_contiguous());

  try
  {
    SM["gsd"];
    assert(false && "Unreachable.");
  } catch (const std::domain_error&) {}

  SM.delete_all([](const auto&& E) -> bool
  {
    assert(*E != "gsd" && "Deleted element remained in iteration?");
    return false; // Don't delete anything!
  });

  SM.erase("Bryce"); // Deleting non-existent element.
  assert(!SM.is_contiguous() && SM.size() == 3 && SM.capacity() == 4 &&
         SM.next_index() == 0);

  SM.erase("John");
  assert(SM.next_index() == 0); // [-, 1, -, 3] are allocated indices.
  assert(SM.size() == 2 && SM.capacity() == 4);
  assert(!SM.is_contiguous());

  try
  {
    SM[2];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {}

  auto IR1 = SM.insert("Hyrum");
  assert(IR1.second == false && IR1.first->first == 3);

  auto IR2 = SM.insert("Bjarne");
  assert(IR2.second == true &&
         IR2.first->first == 0); // Insert at first "good" hole.
  assert(SM.size() == 3 && SM.capacity() == 4);
  assert(!SM.is_contiguous());

  assert(SM.next_index() == 2);
  auto IR3 = SM.insert("Herb");
  assert(IR3.second == true &&
         IR3.first->first == 2); // Insert at first "good" hole.
  assert(SM.is_contiguous());
  assert(SM.capacity() == 4);

  SM.insert("Alexandrescu");
  assert(SM.size() == 5 && SM.next_index() == 5 && SM.capacity() == 5);
  assert(SM.is_contiguous());

  id_bimap<SMFCounter> SMFM;
  SMFM.emplace(1);
  SMFM.emplace(2);

  SMFCounter::reset();
  SMFM.erase(0);
  assert(SMFM.size() == 1 && SMFM.capacity() == 2);
  assert(SMFCounter::Ctor == 0 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 1 &&
         "erase() should destroy removed elements!");
}
```

A std::vector::reserve (Linkek egy külső oldalra)-hez hasonlóan készítsünk egy reserve() függvényt mi is. Segítségével a felhasználó felkészítheti az adatszerkezetünket egy előre ismert számú elemsorozat hozzáadására — ezt a mögöttes implementáció ismeretében értelmesen tegyük lehetővé. Ez a függvény soha nem törölhet meglévő elemet, amennyiben olyan bemenetet kap, amely tényleges elem törlésével járna, csendben (hibás viselkedés vagy kivétel dobása nélkül!) ne csináljon semmit. Egyesítsük a std::vector::shrink_to_fit (Linkek egy külső oldalra) viselkedésének szellemiségét a reserve() függvényünkben, az alábbi logika szerint.

1. Ha a függvény paramétere nagyobb, mint az aktuális kapacitás, akkor növeljük meg a kapacitást. Új elemeket ne hozzunk létre, csak készüljünk fel a beérkező értékekre, logikailag töröltnek jelölt "lyukak" létrehozásával.
2. Ha a függvény paramétere kisebb, mint a legnagyobb valójában felhasznált index, — tehát az új kapacitás beállításához törölni kéne — akkor ne csináljunk semmit. Nem engedjük meg, hogy a függvény létező adatot töröljön.
Például 20-as kapacitás esetén, ha az első 10 helyen vannak elemek, ne csináljunk semmit, ha a felhasználó 5-ös kapacitást kíván beállíŧani.
3. Ha a függvény paramétere kisebb, mint az aktuális kapacitás, de az új kapacitás beállításával nem törölnénk elemeket, akkor szabadítsuk fel a nem használt (pl. logikailag töröltnek jelölt) farokrészt.
Például, ha a kapacitásunk mondjuk 20 volt, majd beszúrtunk 10 elemet az első 10 helyre, és a felhasználó 12-es kapacitást kíván beállítani, tegyük ezt meg, és a "hátsó" 8 helyet ténylegesen töröljük a memóriából.

```cpp
#include <cassert>
#include <stdexcept>

void F3b_reserve()
{
  string_id_bimap SM;
  SM.reserve(4);

  assert(SM.size() == 0 && SM.capacity() == 4);

  SM.insert("gsd");        // 0
  SM.insert("Whisperity"); // 1
  SM.insert("Herb");       // 2
  SM.insert("Xazax");      // 3
  SM.insert("Bryce");      // 4

  assert(SM.size() == 5 && SM.capacity() == 5);

  SM.erase("Herb");
  assert(SM.size() == 4 && SM.capacity() == 5);
  try
  {
    SM[2];
    assert(false && "Unreachable.");
  } catch (const std::out_of_range&) {}

  auto S1 = SM[0];
  auto S2 = SM[3];

  // Valid: [0, 1, -, 3, 4, -]

  SM.reserve(3); // Nothing happens, as there would be elements AFTER the
                 // potentially shrinked size!
  assert(SM.size() == 4 && SM.capacity() == 5);
  // Valid: [0, 1, -, 3, 4, -]

  SM.reserve(8);
  assert(SM.size() == 4 && SM.capacity() == 8);
  // Valid: [0, 1, -, 3, 4, -, -, -]

  SM.reserve(5);
  // Valid: [0, 1, -, 3, 4]
  assert(SM.size() == 4 && SM.capacity() == 5);

  SMFCounter::reset();
  id_bimap<SMFCounter> SMFM;
  SMFM.reserve(1024);
  assert(SMFM.size() == 0 && SMFM.capacity() == 1024);
  assert(SMFCounter::Ctor == 0 && SMFCounter::CCtor == 0 &&
         SMFCounter::MCtor == 0 && SMFCounter::CAsg == 0 &&
         SMFCounter::MAsg == 0 && SMFCounter::Dtor == 0 &&
         "reserve() should not directly construct any elements!");
}
```

## Negyedik rész: Multi-threading

Valósítsuk meg, hogy az adatszerkezet többszálú környezetben is helyt álljon. Az egyszerű, teljes kölcsönös kizárás helyett gondoljunk bele a "readers-writers lock" – std::shared_mutex (Linkek egy külső oldalra), std::shared_lock (Linkek egy külső oldalra), std::unique_lock (Linkek egy külső oldalra) – használatába: miért ne végezhetne keresést egyszerre több szál, ha éppen senki nem ír az adatszerkezetbe?

Ehhez a példához — annak jellege miatt — nem tartozik automatikus teszt.

## Ötödik rész: Levezetés

Végezetül járjunk nyitott szemmel, válasszunk ki néhány egyéb C++ nyelvi elemet vagy megszokott library funkcionalitást, amit beviszünk az implementációnkba, Lehet pl. constexpr vagy noexcept operátor használatával a típusparaméterek függvényében megadni a specifikációt. Gondolkozzunk az implementáció hatékonyságáról, vzeessünk be néhány esetleges optimalizációt. Dokumentáljuk a feltárt trade-off-okat.
