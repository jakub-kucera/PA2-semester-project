<pre>
Tower defense
=============
Autor: Jakub Kučera (kucerj56)

====Téma z Progtestu====
Naprogramujte jednoduchou grafickou tower defense hru

Váš engine:

ze souboru nahraje definici věží (cena, velikost, útok, dosah, ...)
ze souboru nahraje možné mapy a typy nepřátel (životy, rychlost, odolnost na určitý typ věže ,...)
implementuje jednoduchou interakci věž vs. útočníci (útok, proběhnutí, ...), počitadlo skóre, detekci vítězství (po nezabití x útočníků)
implementuje alespoň 2 strategie průchodu hracím polem
umožňuje ukládat a načítat rozehrané hry
Engine může fungovat jako real-time hra, či tahová hra.

Jak může vypadat mapa?

" " označuje možnou lokaci věže a prázdné místa, A, B označuje dva druhy věží, \# označuje zeď, @ a % jsou různé druhy útočníků.


\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
\#                        \#       @@  \#          \#
\#         \#              \#    %B  @  \#\#\#        \#
\#         \#              \#    %\#  @    \#    B   \#
<=%%%     \#              A    %\#       \#@@@@@@@<=
\#         \#              A    %\#       @@   B   \#
\#                        A    %\#                \#
\#                 %%%       %%%\#                \#
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
Cesta kudy se budu útočníci ubírat bude vždy nejkratší možná vzhledem ke zdím a věžím. "Nejkratší" se může lišit pro létající (ignorují zdi) a pozemní jednotky. Pro chytré jednotky, které se vyhýbají věžím. Pro jednotky s krátkým dohledem (k pohybu využívají jen informaci o okolí 5ti polí a cíl).

Kde lze využít polymorfismus? (doporučené)

Parametry věží: znak, barva, dosah, zranění, ...
Efekty útoku věží: zranění, zpomalení, ...
Políčka mapy: volno, věž, útočník ...
Strategie hledání cesty: předdefinovaná cesta, nejkratší (BFS, případně náhodná odchylka), A* prohledávání, vyhýbání se věžím
Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ...

====Upřesnění zadání====
Program po spuštění ukáže možnost, zdali by uživatel chtěl načíst rozehranou hru ze souboru a pokud ano, tak uživatel zadá název souboru, který pokud je platný, tak se hra z něj načte. Pokud uživatel chce začít novou hru, tak se z příslušných souborů načtou definice map, věží a nepřátel.
Formát uložené hry + část ukázky:
Identifikator_typu_záznamu část_záznamu Identifikator_typu_záznamu část_záznamu Identifikator_typu_záznamu …
-MONEY-1700-MONEY-
-PATH-24-PATH-3-PATH-
-TOWER-6-TOWER-3-TOWER-1-TOWER-0-TOWER-
Formát nepřítele + ukázka:
Znak pro zobrazení na mapě; barva; jméno; typ; velikost zdraví
Z;green;Zombie;Undead;50
Formát věže + ukázka:
Znak pro zobrazení na mapě; barva; název; typ; informace zda utočí na všechno v okolí či pouze jednoho; cena; poškození útokem; vzdálenost útoku; rychlost útoku
B;white;Basic Tower;basic;all;100;200;5;1
Formát mapy + ukázka:
Mapa vykreslená pomocí znaků. \# pro volné místo pro věž, = pro start a konec, a mezeru pro volné místo pro pohyb nepřátel.
-Název-Celkové zdraví-Počáteční peníze
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
\#      \#  \#      \#       \#
\#      \#  \#  \#   \#       \#
=      \#\#\#\#  \#   \#    \#  =
\#            \#        \#  \#
\#            \#        \#  \#
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
-Bludistatko-1000-1000
Pokud si uživatel nezvolil načtení uložené hry ze souboru, tak po načtení assetů se vypíše list se všemi mapami, ze kterých si uživatel jednu vybere a k tomu si ještě vybere, jak se má vytvořit cesta po které se budou pohybovat nepřátelé a následně se spustí herní cyklus, který má několik fází:
1)	Načte se vstup. Pokud uživatel zadá na vstup znak 'c', tak následně zadá souřadnice a id věže které se má postavit. Pokud uživatel zadá znak 'q', tak se hra ukončí, po zadání 's' se hra uloží. Při zadání jakékoliv jiného znaku než 'c','s' a 'q' se hra “posune”.
2)	Vytvořené věže zaútočí na nepřátele a některé věže “přenesou” svůj efekt na nepřátele, kteří nadále mohou dostávat poškození.
3)	Všichni nepřátele se posunou a na startu se mohou objevit noví. Ti kteří dorazí do cíle “zmizí” a odečte se uživatelovo zdraví, které pokud se sníží na nulu tak uživatel prohraje.
4)	Nakonec se všechny změny, které byli provedeny zobrazí uživatelovi na konzoli, a případně mu oznámí prohru.

====Polymorfismus====
Polymorfismus využívám u abstraktní třídy CPathFinder, od které dědí třídy CDFS a CBFS. V třídě CPathFinder jsou dvě virtuální metody: createDistances a createDistancesRec (Obě metody jsou využity v obou dvouch podtřídách) (V createDistances volám createDistancesRec). Při implementaci si vytvořím pointer na CPathFinder a přes upcasting mu přiřadím hodnotu z jedné podřazené třídy, takže mi je později jedno o jakou podřazenou (instanci třídy) třídu se jedná.

Polymorfismus také využívám u třídy CArea, od které dědí třídy CTower, CEnemy a CWall. V některých podtřídách přepisuji metody damageByEffect, attack a attackChargeIncrement. Díky čemuž dosáhnu že i přes to že do 2d pole pointerů na CArea, přes upcasting přiřadím pointer podřazené třídy, tak se instance jiných podřazených tříd budou chovat jinak. Například v CTower přepisuji metodu attack a v CEnemy přepisuji metodu damageByEffect.

Polymorfismus ještě používám v abstraktní třídě CEffect s virtuálními metodami jako getEffect, damageByEffect a attack od které dědí třídy CEffectBasic, CEffectBleed, CEffectFire, CEffectPoison. Přepisováním metod v jednotlivých třídách zajistím, že každá podtřída se chová jinak. Opět využívám upcasting.
</pre>
