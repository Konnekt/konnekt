/**\page dev_warn WA¯NE! ...
  Kilka BARDZO istotnych uwag. To w zasadzie swego rodzaju lista rzeczy do 
  zrobienia w API...
  - Logowanie do pliku nie jest synchronizowane. Mog¹ pojawiæ siê dziwne
    logi gdy dwa w¹tki loguj¹ jednoczeœnie.
  - Obs³uga akcji (a w szczególnoœci akcji kontaktów) NIE jest synchronizowana.
  - Rdzeñ w BARDZO wielu miejscach nie sprawdza jeszcze poprawnoœci parametrów
    komunikatów (czy s¹ != 0, czy identyfikatory np. kontaktu czy akcji istniej¹
    itp.)
  - W tej chwili wszystko jest przesy³ane jako dwa parametry typu int. Trzeba
    bardzo uwa¿aæ na przesy³ane typy! W przysz³oœci, bezpoœrednie przesy³anie
    komunikatów zostanie zast¹pione przez "prototypowe" f-cje, by unikn¹æ problemów
    z niew³aœciwymi typami. Podobnie wiêkszoœæ flag zamiast w definicjach, zostanie
    opisana w enumeracjach. Same komunikaty bêd¹ mia³y w³asne struktury w których
    bêd¹ przesy³ane.

*/

/**\page dev_dev Okno developera.
  Okno \b \@Dev pozwala na przegl¹danie logów programu na ¿ywo, a tak¿e
  bezpoœredni dostêp do kilku miejsc w rdzeniu.

  Aby otworzyæ okno trzeba:
   - Posiadaæ wersjê Beta, lub Dev
   - W rejestrze systemu w "CURRENT_USER\\Software\\Stamina\\Konnekt\\" nale¿y
   dodaæ wartoœæ DWORD o nazwie "superUser" i wartoœci 1.
     - ew. uruchomiæ program z parametrem -debug
   - W menu g³ównym pojawi siê nowa pozycja - "\@Dev"

  W tej chwili mo¿na 
  - przegl¹daæ logi na bie¿¹co ([Loguj]), 
  - wstawiaæ znaczniki do konnekt.log ([Zaznacz]), 
  - sprawdziæ czy któraœ z wtyczek nie blokuje g³ównego w¹tku ([Test]),
  - wypisaæ podstawowe informacje o wtyczkach ([Info]).
*/

/**\page develop Jak napisaæ w³asn¹ wtyczkê ...
  Ten artyku³ jest miejscem od którego najlepiej zacz¹æ przegl¹daæ
  SDK. W tej chwili jest to bardziej referencja ni¿ dokumentacja
  ale byæ mo¿e jak znajdzie siê wiêcej czasu dopiszê co nieco wiêcej.

  Ewentualne pytanie proszê zadawaæ na naszym forum. Czemu inni nie mieliby
  skorzystaæ, nieprawda¿?

  <p>API Konnekta mo¿e wydawaæ siê skomplikowane, ale stara³em siê aby na d³u¿sz¹ metê
  by³o jak najwygodniejsze w u¿yciu. Podczas pisania, nie ukrywam, mocno wzorowa³em
  siê na systemie przesy³ania komunikatów w systemie Windows, wiêc osoby obeznane
  z WINAPI znajd¹ sporo podobieñstw. Podstawow¹ zalet¹ tego rozwi¹zania jest prostota
  z jak¹ implementuje siê obs³ugê kolejnych komunikatów i du¿a rozszerzalnoœæ... 
  </p>
  Zanim napiszesz pierwsz¹ wtyczkê najlepiej przejœæ krok po kroku przez t¹ listê:
   - @ref dev_basics
   - @ref dev_guidelines
   - @ref dev_warn
   - @ref dev_template
   - @ref dev_examples
  
  Bardzo pomocne mo¿e siê okazaæ równie¿ @ref dev_dev .

  <p>
  Z chêci¹ zamieszczê gotowe (lub przynajmniej funkcjonalne :) ) wtyczki na naszej stronie
  domowej. W tej chwili nie ma autoryzowania wtyczek, ale nie wykluczone, ¿e kiedyœ siê 
  pojawi... Tak czy inaczej, nalegam, aby ka¿dy kto doprowadzi swoj¹ wtyczkê do stadium
  co najmniej beta skontaktowa³ siê ze mn¹, w celu do³¹czenia identyfikatorów do g³ównego
  SDK.</p>

  \section dev_basics Podstawy
  <p>
  Do dzia³ania wtyczka potrzebuje nag³ówków SDK (*.h), jednej wyexportowanej funkcji z jednym
  switch'em, jednego Ÿród³a do skompilowania i do³¹czenia (plug_defs.cpp), 
  jednego pliku .lib zawieraj¹cego odwo³ania do funkcji exportowanych z konnekt.exe (jeszcze nie)
  i jednej, bardzo wa¿nej liczby. Liczba o której mowa to identyfikator sieci
  oznaczany dalej jako \i NET. Jako sieæ rozumiemy grupê wtyczek jak¹ nasz dll mo¿e utworzyæ
  z innymi dll'ami o tej samej wartoœci \i NET. Np. jedna wtyczka wysy³a emaile, 
  a druga potrafi je odbieraæ, jako ¿e ich akcje siê nie pokrywaj¹, mog¹ korzystaæ z tej samej
  wartoœci NET. Za to ju¿ zupe³nie inna wtyczka, mo¿e wys³aæ komunikat "wyœlij emaila" 
  z tak¹ wartoœci¹ NET, ¿e jedna z naszych wtyczek ten komunikat poprawnie wyœle. Komunikaty
  mo¿na wysy³aæ w zasadzie w ciemno, warto jednak jeszcze sprawdziæ czy zosta³y one odebrane i 
  wykonane. Jak najlepiej ustaliæ wartoœæ NET przeczytasz dalej, w \ref dev_guidelines.

  Podczas tworzenia wtyczek nale¿y korzystaæ z wersji programu dla developerów, która nie wysy³a
  raportów o b³êdach. S¹ dwie wersje - 
  konnekt_dev.exe - zwyk³a
  konnekt_dev_log.exe - loguje do konnekt.log wszystkie IMessage.

  Tak na prawdê do dzia³ania wtyczki wystarczy sama definicja funkcji
  g³ównej - IMessageProcess i kilka deklaracji z plików nag³ówkowych.
  Jednak do pe³nego wykorzystania API, potrzebna bêdzie równie¿ reszta
  akcesoriów. 
  W plug_defs.cpp znajduj¹ siê definicje funkcji, które uproszcz¹ korzystanie
  z API. Wiêkszoœæ funkcji korzysta ze zmiennej globalnej Ctrl (typu Controler) która
  powinna byæ ustawiona natychmiast po otrzymaniu komunikatu IM_INIT i 
  wskazuje na strukturê reprezentuj¹c¹ dan¹ wtyczkê w API (i umo¿liwiaj¹c¹
  komunikacjê z rdzeniem). Je¿eli u¿yjesz któregoœ z przyk³adów za bazê
  bêdzie to ju¿ zrobione za Ciebie...

  @sa net_
  </p>
  <p>
  Ca³a komunikacja opiera siê na przesy³aniu komunikatów (sIMessage_base). Na treœæ komunikatu
  sk³ada siê struktura stworzona na podstawie sIMessage_base. Zazwyczaj bêdzie to sIMessage_2params
  sk³adaj¹ca siê z dwówch 4 bajtowych (long, void* co kto woli) parametrów. 
  W strukturze bazowej zawarty jest adresat i identyfikator
  okreœlaj¹cy jaka akcja powinna zostaæ wywo³ana. Zaadresowaæ komunikat mo¿na na kilka sposobów:
   - do dok³adnie wybranej wtyczki, poprzez podanie jej ID,
   - do sieci wtyczek, która powinna potrafiæ obs³u¿yæ nasz komunikat, podaj¹c NET,
   - do wszystkich wtyczek, podaj¹c jako NET wartoœæ #NET_BROADCAST.
   - i w ró¿nych wariacjach: #net_

  Dodatkowo mo¿emy okroiæ teren naszych poszukiwañ wskazuj¹c typ wtyczek, które powinny
  otrzymaæ komunikat.
  @sa gr_im imt_
  </p>
  <p>
  Oprócz tego, rdzeñ dostarcza te¿ mo¿liwoœæ do przechowywania ustawieñ w plikach profilu
  u¿ytkownika. S³u¿¹ do tego tablice *.dtb. Aby móc z nich korzystaæ, trzeba zarejestrowaæ
  identyfikatory kolumn pod którymi bêdziemy przechowywaæ informacje. Odczyt i zapis
  odbywa siê poprzez np. #SETINT , #GETINT dla konfiguracji, lub bezpoœrednio np.
  w Controler::DTsetStr().
  Raz zarejestrowana kolumna pozostaje w pliku profilu niemal¿e na zawsze, 
  niezale¿nie od tego czy nasza wtyczka dzia³a czy nie i bez wzglêdu
  na wersjê dzia³aj¹cego programu (kompatybilne w dó³ i czêœciowo w górê)...
  </p>
  <p>
  Interfejs u¿ytkownika UI mo¿e zostaæ rozbudowany poprzez akcje (\ref gr_act). Dziêki nim, 
  dowolna wtyczka mo¿e dodaæ now¹ belkê, przycisk, pozycjê w menu czy w konfiguracji.
  </p>
  <p>
  Dodatkowo API rozszerzane jest o mo¿liwoœci do³¹czonych do nich wtyczek
  (tak na prawdê UI te¿ jest najzwyklejsz¹ wtyczk¹, to powinno pokazaæ mo¿liwoœci)...
  Preferowane s¹ nag³ówki uniwersalne, je¿eli istnieje ju¿ jeden do wtyczki
  okreœlonego rodzaju, osoba pisz¹ca inn¹ odmianê tej samej wtyczki powinna
  w miarê mo¿liwoœci wykorzystaæ gotowe API. W ten sposób, je¿eli mamy
  na przyk³ad dwie ró¿ne wtyczki A i B do odtwarzania dŸwiêków obs³uguj¹ce
  to samo API, to je¿eli jakaœ wtyczka C chce odtworzyæ dŸwiêk -
  wysy³a po API komunikat do sieci obs³uguj¹cej dŸwiêk, ¿e okreœlony
  sygna³ ma zostaæ odtworzony. Niezale¿nie teraz czy za³adowana jest wtyczka
  A, czy B - dŸwiêk zostanie puszczony. Je¿eli nie jest za³adowana ¿adna, 
  po prostu nic siê nie stanie, ale wtyczka C nie musi (choæ oczywiœcie mo¿e)
  siê tym przejmowaæ. Na tym polega g³ówna zasada API Konnekta.
  </p>

  \section dev_guidelines Podstawowe zasady
  Podczas pisania wtyczek, wszystkie identyfikatory najlepiej trzymaæ w osobnym pliku nag³ówkowym, 
  tak ¿eby mo¿na go by³o szybko do³¹czyæ do SDK. Jeœli ktoœ chcia³by do³¹czyæ opis swojej
  wtyczki do tej dokumentacji, zapraszam do poczytania o generatorze dokumentacji doxygen na 
  http://www.doxygen.org .<br><br>
  Kilka zasad dotycz¹cych SDK (niektóre mog¹ byæ na razie niezrozumia³e ale warto ju¿ je zapamiêtaæ):
   - Raz poprawnie przydzielonego identyfikatora NIE zmieniamy!
   - Sprawdzamy w SDK czy identyfikator który chcemy wykorzystaæ nie jest ju¿ zajêty!!!
   - Wartoœæ NET powinna byæ w przedziale od @b 256 do 0x00007FFF, w praktyce, nie powinna
     przekraczaæ 8000.
   - Niedu¿e wtyczki nie posiadaj¹ce w³asnej sieci kontaktów mog¹ spokojnie dzieliæ
     t¹ sam¹ wartoœæ NET z innymi niewielkimi wtyczkami...
   - Wtyczki obs³uguj¹ce protoko³y sieciowe i korzystaj¹ce z identyfikatorów ikonek
     musz¹ mieæ wartoœæ < 128. Osoby pisz¹ce takie wtyczki proszê o kontakt, nie
     ma problemu ¿eby wstawiæ je poni¿ej tej wartoœci.
   - Nowe identyfikatory typów komunikatów (\ref MT_) i statusów (\ref ST_) na potrzeby nowych wtyczek
     MUSZ¥ byæ skonsultowane z opiekunem SDK...
   - Nie ma potrzeby tworzyæ kolejnego identyfikatora, jeœli jest ju¿ jakiœ spe³aniaj¹cy dan¹ rolê.
   - Wszystkie pozosta³e identyfikatory (akcji, ikon, kolumn itp.) powinny byæ tworzone jako NET*1000+wartoœæ
   - Identyfikatory powinny byæ mniejsze od 0x007FFFFF (nie licz¹c maski, na które zosta³y przygotowane pozosta³e bity.)
   - Identyfikatory komunikatów powinny byæ tworzone jako (NET*1000+wartoœæ), ale wiêksze ni¿ #IM_USER
   - Wszystkie identyfikatory w gotowych wtyczkach powinny byæ zg³oszone do SDK. Najlepiej
     w pliku nag³ówkowym...
   - Wszystkie tablice znaków (char*) zwracane przez dowolne funkcje NIE MOG¥ byæ
     edytowane. Zwracane wartoœci s¹ najczêœciej @b tymczasowe i kolejne
	 wywo³anie funkcji API mo¿e bufor tymczasowy nadpisaæ...
	 Je¿eli ich zawartoœæ bêdzie potrzebna po wywo³aniu jakiejkolwiek nastêpnej
     f-cji API musi ona zostaæ najpierw skopiowana.
   - Zak³adamy ¿e typy int i void * maj¹ oba po 4 bajty.
   - UID to np. numerek/nazwa u¿ytkownika w sieci (np. UIN w ICQ , czy numerek z Gadu) w postaci tekstowej...
   - U¿ytkownik programu to kontakt o pozycji 0. ¯eby sprawdziæ pozycjê maj¹c identyfikator u¿ywamy Controler::DTgetPos(DTCNT , id_kontaktu)
   - Wszystkie nazwy s¹ angielskie, uwa¿am ¿e dziwnie wygl¹daj¹ konstrukcje polsko-angielskie.
   - Wszystkie d³u¿sze operacje (zw³aszcza sieciowe) \b MUSZ¥ byæ wykonywane w osobnych w¹tkach!
   - Czekam na WSZYSTKIE propozycje zmian. Je¿eli potrzebujecie u¿yæ nowego typu 
     wiadomoœci, nowego identyfikatora sieci, nowego typu ikonki czy nowego komunikatu - zapraszam do powiadomienia mnie o tym :) (\ref team) 
   - Nale¿y respektowaæ wszystkie ustawienia u¿ytkownika sprawdzaj¹c wartoœci w tablicach konfiguracji!.
   - Podczas pisania i testowania wtyczek proszê u¿ywaæ wersji DEV, lub DEV_LOG Konnekta. 
     Obie nie bêd¹ zg³aszaæ b³êdów wygenerowanych podczas prac developerskich. Obie do pobrania na
     stronie domowej SDK!
   - Testowanie powinno odbywaæ siê na osobnym koncie w systemie Beta z dopiskiem "dev" na koñcu!!!
   - Informacje o wtyczce powinny byæ zapisane w Resources DLLa. Jako IM_PLUG_VERSION, jeœli nie
     u¿ywamy resources musimy przekazaæ wersjê w postaci "Major.Minor.Release.Build", przy czym,
     z dwóch ostatnich pól mo¿na zrezygnowaæ.
   - Niektóre (nieliczne) komunikaty do UI (\ref IMI_) wymagaj¹ znajomoœci obs³ugi obiektów WinAPI, lub 
     przynajmniej do³¹czenia nag³ówków <windows.h> w celu sprawdzenia zdefiniowanych wartoœci. Je¿eli
     nie znasz winAPI w ogóle, radzê z tych komunikatów NIE korzystaæ...
   - Je¿eli tworzysz ogólnodostêpny IMessage, zamiast przekazywaæ do niego informacje w p1 i p2
     zdefiniuj w³asn¹ strukturê w której bêd¹ przekazywane informacje.
   - B³êdnie wys³any komunikat (np. opakowany w z³¹ (np. za ma³¹) strukturê) nie jest po drodze
     sprawdzany pod k¹tem poprawnoœci. W najlepszym wypadku spowoduje wywrócenie siê programu!
     W tej chwili najczêœciej u¿ywan¹ struktur¹ jest IMessage_2params (przez f-cje pomocnicze),
     w przysz³oœci, ka¿dy komunikat bêdzie mia³ swoj¹ prototypow¹ funkcjê i ju¿ nie trzeba bêdzie
     tak uwa¿aæ :) ...
	 Trzeba jednak koniecznie stosowaæ siê do podawanych wymaganych struktur, a w obs³udze
	 komunikatów, wypada³oby sprawdziæ, czy strukturka ma odpowiedni rozmiar...
   - Pamiêtaj, ¿e wtyczki do których wysy³asz sIMessage, czy sUIActionNotify mog¹ modyfikowaæ
     poszczególne pola tych struktur! Je¿eli wykorzystujesz jeden obiekt do wysy³ania komunikatów
     do kilku wtyczek, najlepiej ustawiæ pola parametrów i sUIAction::cnt na nowo!
   - Programuj ostro¿nie! Przemyœl dok³adnie numeracjê identyfikatorów, upewnij siê 3 razy
     czy nie bêdziesz kolidowa³ z którymœ z wymienionych tu punktów b¹dŸ ju¿ wykorzystywanym
     identyfikatorem. Twoja pomy³ka, mo¿e kosztowaæ potencjalnego u¿ytkownika sporo nerwów i
	 byæ bardzo trudn¹ do wykrycia!
   - Wtyczki NIE mog¹ zmieniaæ aktywnego katalogu aplikacji. Wiêkszoœæ œcie¿ek zapisana jest
     relatywnie do katalogu z programem. W razie niepewnoœci, mo¿na u¿yæ #IMC_RESTORECURDIR.
   - W ka¿dym nowym SDK @b koniecznie trzeba przeczytaæ changelog!
   - Pliki nag³ówkowe z API wtyczki nale¿y umieœciæ tam, gdzie i inne nag³ówki (katalog Konnekt).
	 Nazwa pliku powinna brzmieæ tak samo jak nazwa pliku i nie kolidowaæ z istniej¹cymi ju¿ nazwami...     

  \section dev_template Projekt bazowy

  W katalogu z przyk³adami mo¿na znaleŸæ <b>template.cpp</b>. Jest to "pusty" projekt
  wtyczki z podstawowymi funkcjami. Najlepiej zaczynaæ pisanie ka¿dej wtyczki od
  skopiowania i @b modyfikacji tego pliku...<br><br>
  \sa xmpl_template
  
  \section dev_examples Przyk³ady
  Do SDK do³¹czony jest katalog z pryk³adowymi wtyczkami. Wszystkie korzystaj¹ z 
  identyfikatora sieci #NET_EXAMPLE.<br>
  <br>
  Ich kody Ÿród³owe i krótkie opisy znajdziesz @ref gr_examples "tutaj".


*/


/**@defgroup gr_examples Przyk³ady
  ¯eby lepiej zrozumieæ zasady dzia³ania systemu, zamieszczam kody Ÿród³owe w dokumentacji
  ¿eby szybko i wygodnie móc sprawdziæ znaczenia u¿ytych struktur czy define'ów...
  @{
  */

    /**
       @defgroup xmpl_template Na dobry pocz¹tek
       @{
        Najlepsze Ÿród³o do rozpoczêcia pisania ka¿dej wtyczki w C++.
        <br><br>template.cpp:<br>
        \include template.cpp
       @}
    */

    /**
       @defgroup xmpl_simple Komunikaty
       @{
        Przesy³anie prostych komunikatów.
        <br><br>simple.cpp:<br>
        \include simple.cpp
       @}
    */

    /**
       @defgroup xmpl_actcfg UI i CFG
       @{
        Prosta obs³uga UI i konfiguracji.
        <br><br>actcfg.cpp:<br>
        \include actcfg.cpp
       @}
    */

    /**
       @defgroup xmpl_msgs Wiadomoœci
       @{
        Prosta obs³uga wiadomoœci tekstowych.
        <br><br>msgs.cpp:<br>
        \include msgs.cpp
       @}
    */


///@}


// --------------------------------------------------------------------------------------------
/**\page port Pisanie wtyczek - w jakim jêzyku?
  API Konnekta z za³o¿enia wykorzystuje jêzyk C++ (w tej chwili w bardzo
  uproszczonej wersji), ale tworzone jest z myœl¹ o mo¿liwoœci przygotowania
  kodu dzia³aj¹cego pod innym kompilatorem i w innym jêzyku. 
  Dopóki nie ma du¿ego zainteresowania, nie mamy równie¿ planów przygotowania
  nag³ówków do wspó³pracy z innymi kompilatorami. Gdy zapotrzebowanie siê
  pojawi, z pewnoœci¹ zajmiemy siê i ta kwesti¹.
  Docelowo zamiast makr i definicji wykorzystywane bêd¹ przestrzenie
  nazw, sta³e i zdecydowanie wiêcej obiektów.

  Aktualnie Konnekt i nasze wtyczki kompilowane s¹ w Visual C++ 7 (ten z pakietem
  .NET), w opcji Multi-threaded DLL (opcje projektu/C++/Code generation), wiêc
  jeœli u¿ywasz tego kompilatora - gor¹co t¹ opcjê polecamy...
  Za jakiœ czas najpewniej ca³oœæ zostanie przeniesiona na Visual C++ 7.1.

*/
