Féléves feladat:
A feladat egy játék, amely a ragadozó életmódot szimulálná nagyon leegyszerűsítve.

Az irányítható karakter a ragadozó (egy róka), és különböző állatokat kell levadásznia. Ezek az állatok véletlenszerűen generált pontokban jelennek meg a játék elején, és annak függvényében, hogy mennyire közel van adott pillanatban a ragadozó, menekülnek előle.
Véletlenszerű pontok generálása: a pálya terének pontjai kétdimenziós tömbként lesznek tekintve, ahol két egymás melletti pont között egy egységnyi távolság van. Ezek egyetlen számmal lesznek indexelve (i*m + j formában, ahol i az aktuális sor, j az aktuális oszlop és m az oszlopok számát jelenti), és ez az index lesz generálva a rand() függvény felhasználásával minden állat esetén. Nincs ismétlődésvizsgálat, emiatt pedig lesznek állatok, amelyek ugyanabból a pontból indulnak, ez azonban az állatok közötti ütközésvizsgálat miatt rövid időn belül megoldódik.

Az állatok elkapási ideje és egy maximális pontszám függvényében a játékos pontot szerez azáltal, hogy elkap egy állatot. Kétféle elkapható állat van, nyúl és tyúk. A nyúl nagyobb sebességgel szalad el, de több pontot is kap a játékos, ha levadássza, míg a tyúk valamivel lassúbb, és kevesebb pontot ad.

A játékos egy fényforrást hordoz magával, ez azonban alap állapotában halvány, így a játékos nem láthatja be a teljes pályát (így pedig nem is láthat minden állatot a pályán). A pálya közepén egy fénygömb áll, amely erősítheti a játékosnál levő fényforrást ideiglenesen, hogy távolabbra elláthasson.

A játék akkor ér véget, amikor a játékos minden állatot levadászott.
