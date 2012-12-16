/**\page dev_warn WA�NE! ...
  Kilka BARDZO istotnych uwag. To w zasadzie swego rodzaju lista rzeczy do 
  zrobienia w API...
  - Logowanie do pliku nie jest synchronizowane. Mog� pojawi� si� dziwne
    logi gdy dwa w�tki loguj� jednocze�nie.
  - Obs�uga akcji (a w szczeg�lno�ci akcji kontakt�w) NIE jest synchronizowana.
  - Rdze� w BARDZO wielu miejscach nie sprawdza jeszcze poprawno�ci parametr�w
    komunikat�w (czy s� != 0, czy identyfikatory np. kontaktu czy akcji istniej�
    itp.)
  - W tej chwili wszystko jest przesy�ane jako dwa parametry typu int. Trzeba
    bardzo uwa�a� na przesy�ane typy! W przysz�o�ci, bezpo�rednie przesy�anie
    komunikat�w zostanie zast�pione przez "prototypowe" f-cje, by unikn�� problem�w
    z niew�a�ciwymi typami. Podobnie wi�kszo�� flag zamiast w definicjach, zostanie
    opisana w enumeracjach. Same komunikaty b�d� mia�y w�asne struktury w kt�rych
    b�d� przesy�ane.

*/

/**\page dev_dev Okno developera.
  Okno \b \@Dev pozwala na przegl�danie log�w programu na �ywo, a tak�e
  bezpo�redni dost�p do kilku miejsc w rdzeniu.

  Aby otworzy� okno trzeba:
   - Posiada� wersj� Beta, lub Dev
   - W rejestrze systemu w "CURRENT_USER\\Software\\Stamina\\Konnekt\\" nale�y
   doda� warto�� DWORD o nazwie "superUser" i warto�ci 1.
     - ew. uruchomi� program z parametrem -debug
   - W menu g��wnym pojawi si� nowa pozycja - "\@Dev"

  W tej chwili mo�na 
  - przegl�da� logi na bie��co ([Loguj]), 
  - wstawia� znaczniki do konnekt.log ([Zaznacz]), 
  - sprawdzi� czy kt�ra� z wtyczek nie blokuje g��wnego w�tku ([Test]),
  - wypisa� podstawowe informacje o wtyczkach ([Info]).
*/

/**\page develop Jak napisa� w�asn� wtyczk� ...
  Ten artyku� jest miejscem od kt�rego najlepiej zacz�� przegl�da�
  SDK. W tej chwili jest to bardziej referencja ni� dokumentacja
  ale by� mo�e jak znajdzie si� wi�cej czasu dopisz� co nieco wi�cej.

  Ewentualne pytanie prosz� zadawa� na naszym forum. Czemu inni nie mieliby
  skorzysta�, nieprawda�?

  <p>API Konnekta mo�e wydawa� si� skomplikowane, ale stara�em si� aby na d�u�sz� met�
  by�o jak najwygodniejsze w u�yciu. Podczas pisania, nie ukrywam, mocno wzorowa�em
  si� na systemie przesy�ania komunikat�w w systemie Windows, wi�c osoby obeznane
  z WINAPI znajd� sporo podobie�stw. Podstawow� zalet� tego rozwi�zania jest prostota
  z jak� implementuje si� obs�ug� kolejnych komunikat�w i du�a rozszerzalno��... 
  </p>
  Zanim napiszesz pierwsz� wtyczk� najlepiej przej�� krok po kroku przez t� list�:
   - @ref dev_basics
   - @ref dev_guidelines
   - @ref dev_warn
   - @ref dev_template
   - @ref dev_examples
  
  Bardzo pomocne mo�e si� okaza� r�wnie� @ref dev_dev .

  <p>
  Z ch�ci� zamieszcz� gotowe (lub przynajmniej funkcjonalne :) ) wtyczki na naszej stronie
  domowej. W tej chwili nie ma autoryzowania wtyczek, ale nie wykluczone, �e kiedy� si� 
  pojawi... Tak czy inaczej, nalegam, aby ka�dy kto doprowadzi swoj� wtyczk� do stadium
  co najmniej beta skontaktowa� si� ze mn�, w celu do��czenia identyfikator�w do g��wnego
  SDK.</p>

  \section dev_basics Podstawy
  <p>
  Do dzia�ania wtyczka potrzebuje nag��wk�w SDK (*.h), jednej wyexportowanej funkcji z jednym
  switch'em, jednego �r�d�a do skompilowania i do��czenia (plug_defs.cpp), 
  jednego pliku .lib zawieraj�cego odwo�ania do funkcji exportowanych z konnekt.exe (jeszcze nie)
  i jednej, bardzo wa�nej liczby. Liczba o kt�rej mowa to identyfikator sieci
  oznaczany dalej jako \i NET. Jako sie� rozumiemy grup� wtyczek jak� nasz dll mo�e utworzy�
  z innymi dll'ami o tej samej warto�ci \i NET. Np. jedna wtyczka wysy�a emaile, 
  a druga potrafi je odbiera�, jako �e ich akcje si� nie pokrywaj�, mog� korzysta� z tej samej
  warto�ci NET. Za to ju� zupe�nie inna wtyczka, mo�e wys�a� komunikat "wy�lij emaila" 
  z tak� warto�ci� NET, �e jedna z naszych wtyczek ten komunikat poprawnie wy�le. Komunikaty
  mo�na wysy�a� w zasadzie w ciemno, warto jednak jeszcze sprawdzi� czy zosta�y one odebrane i 
  wykonane. Jak najlepiej ustali� warto�� NET przeczytasz dalej, w \ref dev_guidelines.

  Podczas tworzenia wtyczek nale�y korzysta� z wersji programu dla developer�w, kt�ra nie wysy�a
  raport�w o b��dach. S� dwie wersje - 
  konnekt_dev.exe - zwyk�a
  konnekt_dev_log.exe - loguje do konnekt.log wszystkie IMessage.

  Tak na prawd� do dzia�ania wtyczki wystarczy sama definicja funkcji
  g��wnej - IMessageProcess i kilka deklaracji z plik�w nag��wkowych.
  Jednak do pe�nego wykorzystania API, potrzebna b�dzie r�wnie� reszta
  akcesori�w. 
  W plug_defs.cpp znajduj� si� definicje funkcji, kt�re uproszcz� korzystanie
  z API. Wi�kszo�� funkcji korzysta ze zmiennej globalnej Ctrl (typu Controler) kt�ra
  powinna by� ustawiona natychmiast po otrzymaniu komunikatu IM_INIT i 
  wskazuje na struktur� reprezentuj�c� dan� wtyczk� w API (i umo�liwiaj�c�
  komunikacj� z rdzeniem). Je�eli u�yjesz kt�rego� z przyk�ad�w za baz�
  b�dzie to ju� zrobione za Ciebie...

  @sa net_
  </p>
  <p>
  Ca�a komunikacja opiera si� na przesy�aniu komunikat�w (sIMessage_base). Na tre�� komunikatu
  sk�ada si� struktura stworzona na podstawie sIMessage_base. Zazwyczaj b�dzie to sIMessage_2params
  sk�adaj�ca si� z dw�wch 4 bajtowych (long, void* co kto woli) parametr�w. 
  W strukturze bazowej zawarty jest adresat i identyfikator
  okre�laj�cy jaka akcja powinna zosta� wywo�ana. Zaadresowa� komunikat mo�na na kilka sposob�w:
   - do dok�adnie wybranej wtyczki, poprzez podanie jej ID,
   - do sieci wtyczek, kt�ra powinna potrafi� obs�u�y� nasz komunikat, podaj�c NET,
   - do wszystkich wtyczek, podaj�c jako NET warto�� #NET_BROADCAST.
   - i w r�nych wariacjach: #net_

  Dodatkowo mo�emy okroi� teren naszych poszukiwa� wskazuj�c typ wtyczek, kt�re powinny
  otrzyma� komunikat.
  @sa gr_im imt_
  </p>
  <p>
  Opr�cz tego, rdze� dostarcza te� mo�liwo�� do przechowywania ustawie� w plikach profilu
  u�ytkownika. S�u�� do tego tablice *.dtb. Aby m�c z nich korzysta�, trzeba zarejestrowa�
  identyfikatory kolumn pod kt�rymi b�dziemy przechowywa� informacje. Odczyt i zapis
  odbywa si� poprzez np. #SETINT , #GETINT dla konfiguracji, lub bezpo�rednio np.
  w Controler::DTsetStr().
  Raz zarejestrowana kolumna pozostaje w pliku profilu niemal�e na zawsze, 
  niezale�nie od tego czy nasza wtyczka dzia�a czy nie i bez wzgl�du
  na wersj� dzia�aj�cego programu (kompatybilne w d� i cz�ciowo w g�r�)...
  </p>
  <p>
  Interfejs u�ytkownika UI mo�e zosta� rozbudowany poprzez akcje (\ref gr_act). Dzi�ki nim, 
  dowolna wtyczka mo�e doda� now� belk�, przycisk, pozycj� w menu czy w konfiguracji.
  </p>
  <p>
  Dodatkowo API rozszerzane jest o mo�liwo�ci do��czonych do nich wtyczek
  (tak na prawd� UI te� jest najzwyklejsz� wtyczk�, to powinno pokaza� mo�liwo�ci)...
  Preferowane s� nag��wki uniwersalne, je�eli istnieje ju� jeden do wtyczki
  okre�lonego rodzaju, osoba pisz�ca inn� odmian� tej samej wtyczki powinna
  w miar� mo�liwo�ci wykorzysta� gotowe API. W ten spos�b, je�eli mamy
  na przyk�ad dwie r�ne wtyczki A i B do odtwarzania d�wi�k�w obs�uguj�ce
  to samo API, to je�eli jaka� wtyczka C chce odtworzy� d�wi�k -
  wysy�a po API komunikat do sieci obs�uguj�cej d�wi�k, �e okre�lony
  sygna� ma zosta� odtworzony. Niezale�nie teraz czy za�adowana jest wtyczka
  A, czy B - d�wi�k zostanie puszczony. Je�eli nie jest za�adowana �adna, 
  po prostu nic si� nie stanie, ale wtyczka C nie musi (cho� oczywi�cie mo�e)
  si� tym przejmowa�. Na tym polega g��wna zasada API Konnekta.
  </p>

  \section dev_guidelines Podstawowe zasady
  Podczas pisania wtyczek, wszystkie identyfikatory najlepiej trzyma� w osobnym pliku nag��wkowym, 
  tak �eby mo�na go by�o szybko do��czy� do SDK. Je�li kto� chcia�by do��czy� opis swojej
  wtyczki do tej dokumentacji, zapraszam do poczytania o generatorze dokumentacji doxygen na 
  http://www.doxygen.org .<br><br>
  Kilka zasad dotycz�cych SDK (niekt�re mog� by� na razie niezrozumia�e ale warto ju� je zapami�ta�):
   - Raz poprawnie przydzielonego identyfikatora NIE zmieniamy!
   - Sprawdzamy w SDK czy identyfikator kt�ry chcemy wykorzysta� nie jest ju� zaj�ty!!!
   - Warto�� NET powinna by� w przedziale od @b 256 do 0x00007FFF, w praktyce, nie powinna
     przekracza� 8000.
   - Niedu�e wtyczki nie posiadaj�ce w�asnej sieci kontakt�w mog� spokojnie dzieli�
     t� sam� warto�� NET z innymi niewielkimi wtyczkami...
   - Wtyczki obs�uguj�ce protoko�y sieciowe i korzystaj�ce z identyfikator�w ikonek
     musz� mie� warto�� < 128. Osoby pisz�ce takie wtyczki prosz� o kontakt, nie
     ma problemu �eby wstawi� je poni�ej tej warto�ci.
   - Nowe identyfikatory typ�w komunikat�w (\ref MT_) i status�w (\ref ST_) na potrzeby nowych wtyczek
     MUSZ� by� skonsultowane z opiekunem SDK...
   - Nie ma potrzeby tworzy� kolejnego identyfikatora, je�li jest ju� jaki� spe�aniaj�cy dan� rol�.
   - Wszystkie pozosta�e identyfikatory (akcji, ikon, kolumn itp.) powinny by� tworzone jako NET*1000+warto��
   - Identyfikatory powinny by� mniejsze od 0x007FFFFF (nie licz�c maski, na kt�re zosta�y przygotowane pozosta�e bity.)
   - Identyfikatory komunikat�w powinny by� tworzone jako (NET*1000+warto��), ale wi�ksze ni� #IM_USER
   - Wszystkie identyfikatory w gotowych wtyczkach powinny by� zg�oszone do SDK. Najlepiej
     w pliku nag��wkowym...
   - Wszystkie tablice znak�w (char*) zwracane przez dowolne funkcje NIE MOG� by�
     edytowane. Zwracane warto�ci s� najcz�ciej @b tymczasowe i kolejne
	 wywo�anie funkcji API mo�e bufor tymczasowy nadpisa�...
	 Je�eli ich zawarto�� b�dzie potrzebna po wywo�aniu jakiejkolwiek nast�pnej
     f-cji API musi ona zosta� najpierw skopiowana.
   - Zak�adamy �e typy int i void * maj� oba po 4 bajty.
   - UID to np. numerek/nazwa u�ytkownika w sieci (np. UIN w ICQ , czy numerek z Gadu) w postaci tekstowej...
   - U�ytkownik programu to kontakt o pozycji 0. �eby sprawdzi� pozycj� maj�c identyfikator u�ywamy Controler::DTgetPos(DTCNT , id_kontaktu)
   - Wszystkie nazwy s� angielskie, uwa�am �e dziwnie wygl�daj� konstrukcje polsko-angielskie.
   - Wszystkie d�u�sze operacje (zw�aszcza sieciowe) \b MUSZ� by� wykonywane w osobnych w�tkach!
   - Czekam na WSZYSTKIE propozycje zmian. Je�eli potrzebujecie u�y� nowego typu 
     wiadomo�ci, nowego identyfikatora sieci, nowego typu ikonki czy nowego komunikatu - zapraszam do powiadomienia mnie o tym :) (\ref team) 
   - Nale�y respektowa� wszystkie ustawienia u�ytkownika sprawdzaj�c warto�ci w tablicach konfiguracji!.
   - Podczas pisania i testowania wtyczek prosz� u�ywa� wersji DEV, lub DEV_LOG Konnekta. 
     Obie nie b�d� zg�asza� b��d�w wygenerowanych podczas prac developerskich. Obie do pobrania na
     stronie domowej SDK!
   - Testowanie powinno odbywa� si� na osobnym koncie w systemie Beta z dopiskiem "dev" na ko�cu!!!
   - Informacje o wtyczce powinny by� zapisane w Resources DLLa. Jako IM_PLUG_VERSION, je�li nie
     u�ywamy resources musimy przekaza� wersj� w postaci "Major.Minor.Release.Build", przy czym,
     z dw�ch ostatnich p�l mo�na zrezygnowa�.
   - Niekt�re (nieliczne) komunikaty do UI (\ref IMI_) wymagaj� znajomo�ci obs�ugi obiekt�w WinAPI, lub 
     przynajmniej do��czenia nag��wk�w <windows.h> w celu sprawdzenia zdefiniowanych warto�ci. Je�eli
     nie znasz winAPI w og�le, radz� z tych komunikat�w NIE korzysta�...
   - Je�eli tworzysz og�lnodost�pny IMessage, zamiast przekazywa� do niego informacje w p1 i p2
     zdefiniuj w�asn� struktur� w kt�rej b�d� przekazywane informacje.
   - B��dnie wys�any komunikat (np. opakowany w z�� (np. za ma��) struktur�) nie jest po drodze
     sprawdzany pod k�tem poprawno�ci. W najlepszym wypadku spowoduje wywr�cenie si� programu!
     W tej chwili najcz�ciej u�ywan� struktur� jest IMessage_2params (przez f-cje pomocnicze),
     w przysz�o�ci, ka�dy komunikat b�dzie mia� swoj� prototypow� funkcj� i ju� nie trzeba b�dzie
     tak uwa�a� :) ...
	 Trzeba jednak koniecznie stosowa� si� do podawanych wymaganych struktur, a w obs�udze
	 komunikat�w, wypada�oby sprawdzi�, czy strukturka ma odpowiedni rozmiar...
   - Pami�taj, �e wtyczki do kt�rych wysy�asz sIMessage, czy sUIActionNotify mog� modyfikowa�
     poszczeg�lne pola tych struktur! Je�eli wykorzystujesz jeden obiekt do wysy�ania komunikat�w
     do kilku wtyczek, najlepiej ustawi� pola parametr�w i sUIAction::cnt na nowo!
   - Programuj ostro�nie! Przemy�l dok�adnie numeracj� identyfikator�w, upewnij si� 3 razy
     czy nie b�dziesz kolidowa� z kt�rym� z wymienionych tu punkt�w b�d� ju� wykorzystywanym
     identyfikatorem. Twoja pomy�ka, mo�e kosztowa� potencjalnego u�ytkownika sporo nerw�w i
	 by� bardzo trudn� do wykrycia!
   - Wtyczki NIE mog� zmienia� aktywnego katalogu aplikacji. Wi�kszo�� �cie�ek zapisana jest
     relatywnie do katalogu z programem. W razie niepewno�ci, mo�na u�y� #IMC_RESTORECURDIR.
   - W ka�dym nowym SDK @b koniecznie trzeba przeczyta� changelog!
   - Pliki nag��wkowe z API wtyczki nale�y umie�ci� tam, gdzie i inne nag��wki (katalog Konnekt).
	 Nazwa pliku powinna brzmie� tak samo jak nazwa pliku i nie kolidowa� z istniej�cymi ju� nazwami...     

  \section dev_template Projekt bazowy

  W katalogu z przyk�adami mo�na znale�� <b>template.cpp</b>. Jest to "pusty" projekt
  wtyczki z podstawowymi funkcjami. Najlepiej zaczyna� pisanie ka�dej wtyczki od
  skopiowania i @b modyfikacji tego pliku...<br><br>
  \sa xmpl_template
  
  \section dev_examples Przyk�ady
  Do SDK do��czony jest katalog z pryk�adowymi wtyczkami. Wszystkie korzystaj� z 
  identyfikatora sieci #NET_EXAMPLE.<br>
  <br>
  Ich kody �r�d�owe i kr�tkie opisy znajdziesz @ref gr_examples "tutaj".


*/


/**@defgroup gr_examples Przyk�ady
  �eby lepiej zrozumie� zasady dzia�ania systemu, zamieszczam kody �r�d�owe w dokumentacji
  �eby szybko i wygodnie m�c sprawdzi� znaczenia u�ytych struktur czy define'�w...
  @{
  */

    /**
       @defgroup xmpl_template Na dobry pocz�tek
       @{
        Najlepsze �r�d�o do rozpocz�cia pisania ka�dej wtyczki w C++.
        <br><br>template.cpp:<br>
        \include template.cpp
       @}
    */

    /**
       @defgroup xmpl_simple Komunikaty
       @{
        Przesy�anie prostych komunikat�w.
        <br><br>simple.cpp:<br>
        \include simple.cpp
       @}
    */

    /**
       @defgroup xmpl_actcfg UI i CFG
       @{
        Prosta obs�uga UI i konfiguracji.
        <br><br>actcfg.cpp:<br>
        \include actcfg.cpp
       @}
    */

    /**
       @defgroup xmpl_msgs Wiadomo�ci
       @{
        Prosta obs�uga wiadomo�ci tekstowych.
        <br><br>msgs.cpp:<br>
        \include msgs.cpp
       @}
    */


///@}


// --------------------------------------------------------------------------------------------
/**\page port Pisanie wtyczek - w jakim j�zyku?
  API Konnekta z za�o�enia wykorzystuje j�zyk C++ (w tej chwili w bardzo
  uproszczonej wersji), ale tworzone jest z my�l� o mo�liwo�ci przygotowania
  kodu dzia�aj�cego pod innym kompilatorem i w innym j�zyku. 
  Dop�ki nie ma du�ego zainteresowania, nie mamy r�wnie� plan�w przygotowania
  nag��wk�w do wsp�pracy z innymi kompilatorami. Gdy zapotrzebowanie si�
  pojawi, z pewno�ci� zajmiemy si� i ta kwesti�.
  Docelowo zamiast makr i definicji wykorzystywane b�d� przestrzenie
  nazw, sta�e i zdecydowanie wi�cej obiekt�w.

  Aktualnie Konnekt i nasze wtyczki kompilowane s� w Visual C++ 7 (ten z pakietem
  .NET), w opcji Multi-threaded DLL (opcje projektu/C++/Code generation), wi�c
  je�li u�ywasz tego kompilatora - gor�co t� opcj� polecamy...
  Za jaki� czas najpewniej ca�o�� zostanie przeniesiona na Visual C++ 7.1.

*/
