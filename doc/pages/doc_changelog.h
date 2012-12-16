/**@page changelog Changelog

 @section chg_051207 2005-12-07   v0.6.23
   - IMessageDirect ju¿ nie uznaje id wtyczki równego 0, jako id wtyczki wywo³uj¹cej!!!
   - klasa cCtrl przemianowana na Controler
   - klasa Net::Broadcast do konstruowania flag broadcastów
   - flagi IMT_* przeniesione do enIMessageType
   - czêœæ struktur przeniesiona do przestrzeni Konnekt
   - Du¿o zmian w obs³udze DTB, oraz w klasie iTable
   - klasa do obs³ugi wtyczek - iPlugin
   - wartoœci sieci s¹ teraz typu tNet i mieszcz¹ siê w przestrzeni Konnekt::Net. Wszystkie wtyczki powinny albo zostaæ dodane do tej listy, albo deklarowaæ swoj¹ wartoœæ net jako:
   @code
   const static tNet foo = 1000;
   // lub
   #define NET_FOO (tNet)1000
   @endcode
   - wartoœci IMT s¹ teraz w enumeracji enIMessageType


 @section chg_050708 2005-07-08   v0.6.22.137
   - Identyfikator tablicy tTableId jest teraz tTableId
   - Wszystkie g³ówne tablice podpiête s¹ pod nowe API obs³ugi tablic

 @section chg_050210 2005-02-10   v0.6.22
   - #IM_CNT_CHANGED rozsy³any jest równie¿ po ka¿dym zapisaniu informacji o kontakcie.
   - w #IMC_FINDCONTACT (i teoretycznie we wszystkich miejscach gdzie podajemy kontakt jako parê sieæ,UID) mo¿emy podaæ sieæ jako NET_NONE i UID jako identyfikator kontaktu w tablicy (oczywiœcie liczba zamieniona na tekst)
   - #ACTT_LINK - p³aski przycisk z kursorem "³apki"
   - #ACTSEDIT_READONLY - flaga dla pól edycyjnych tylko do odczytu
   - #IMC_SETCONNECT przyjmuje jako pierwszy parametr równie¿ minimalny czas odstêpu pomiêdzy próbami. Przy okazji modyfikacja zasad dzia³ania - zobacz opis.
 @section chg_041102 2004-11-02   v0.6.20.116
   - Stopniowanie poziomów trudnoœci interfejsu Konnekt::ShowBits. Co prawda ui.dll jeszcze tego nie obs³uguje, ale nale¿y uwzglêdniæ je ju¿ teraz!
   - Nowe typy kontrolek - #ACTT_SLIDER, #ACTT_RADIO, #ACTT_SPINNER, #ACTT_TIPBUTTON
   - Funkcje do obs³ugi dodatkowych wartoœci przekazywanych do akcji w @a txt - GetCfgParam(), SetCfgParam() i CfgParamExists()...
   - Nowe parametry do przekazywania w polu @a txt akcji - #AP_PARAMS, #AP_TIP, #AP_TIPTITLE (i inne zwi¹zane z podpowiedziami...)
   - Garœæ zmian w obs³udze URLi do obrazków - #IMI_ICONREGISTER
   - #MF_DONTADDTOHISTORY, #MF_HIDE
   - Funkcja do tworzenia standardowych akcji informacyjnych - UIActionCfgAddInfoBox()
   - Funkcje do tworzenia standardowych akcji z informacj¹ o wtyczce - UIActionCfgAddPluginInfoBox(), UIActionCfgAddPluginInfoBox2()
   - Wysokoœci akcji ACTT_INFO, ACTT_HTMLINFO i ACTT_TEXT mo¿na podaæ jako ujemn¹ liczbê linijek jakie maj¹ pomieœciæ. np. -2
   - Podaj¹c wysokoœæ akcji ACTT_INFO jako 0 zostanie dostosowana do wysokoœci tekstu
   - Podaj¹c szerokoœæ akcji ACTT_BUTTON, ACTT_TIPBUTTON, ACTT_CHECK i ACTT_RADIO zostan¹ dostosowane do szerokoœci tekstu
     
 @section chg_040519 2004-05-19   v0.6.12.113
   - #IMC_GET_MAINTHREAD
 @section chg_040510 2004-05-10   v0.6.12.112
   - lekko zmienione zachowanie cCtrl::is_TUS()
   - wtyczka SMS udostepnia dodatkowe funkcje...

 @section chg_040301 2004-03-01   v0.6.19.110
   - przypominam, ¿e wartoœci NET wtyczek musz¹ byæ WIÊKSZE ni¿ 255
   - nowe pola opisuj¹ce kontakty @ref cnt_
   - #IMI_ACTION_GETOWNER
   - @ref howto_subclass - nowy @b HOWTO
   - zdjête ograniczenia w przesy³aniu komunikatów pomiêdzy wtyczkami (przynajmniej tymczasowo...)
   - #UIAIM_VALUE_CONVERT , #ACTN_CONVERT_TO , #ACTN_CONVERT_FROM , #ACTR_CONVERT
   - UIActionCfgSetValue() , UIActionCfgGetValue() , CntNfoSetValue() , CntNfoGetValue() domyœlnie konwertuj¹ przekazywane wartoœci!
   - Poprawione flagi ACTSFONT_*
   - #ACTT_HTMLINFO
   - sIMessage_UIMakePopup , #IMI_ACTION_MAKEPOPUPMENU
   - #IM_BEFOREEND
   - cCtrl::QuickShutdown()

 @section chg_031204 2003-12-04   v0.6.18.105
   - dziêki #MF_MENUBYUI ustawionemu jako flaga wiadomoœci interfejs zajmie siê
     obs³ug¹ akcji w menu kontaktu/zdarzeñ, która albo wywo³a przypisan¹ do 
	 wiadomoœci akcjê, albo otworzy wiadomoœæ przez #IM_MSG_OPEN
   - #ACTN_CREATEGROUP
   - #IM_CNT_COMPOSING , #IM_CNT_COMPOSING_STOP , #IMI_CNT_ISCOMPOSING, #IMI_DLGTOKEN, IM_PLUG_ARGS
   - wtyczki obs³uguj¹ce sieæ powinny obs³u¿yæ #IM_PLUG_NETSHORTNAME i IM_PLUG_UIDNAME
   - flaga statusu #ST_COMPOSING
   - kolumna #CNT_STATUS_ICON dla kontaktów w której mo¿na ustawiæ identyfikator
     ikony na liœcie IML_16 która zast¹pi ikonkê statusu...
   - #ACTSMENU_BOLD
   - sDIALOG_token

 \section chg_031126 2003-11-26   v0.6.18.104
   - zamiast IMLOG polecam u¿ywaæ teraz cCtrl::IMDEBUG(), lub samo IMDEBUG.
     Funkcja sprawdza najpierw "poziom debugowania" danej wtyczki przy pomocy
	 cCtrl::DebugLevel i je¿eli odpowiedni rodzaj logowania jest w³¹czony - wywo³uje
	 zwyk³e IMLOG.
   - wszystkie wymagane pliki nag³ówkowe znajduj¹ siê w katalogu Konnekt. 
     Dodatkowo teraz:
       ui_shared.h to teraz ui.h
       plug_shared.h to teraz plug.h
     W kompilatorze nale¿y dodaæ œcie¿kê katalogu w którym znajdzie siê kat. Konnekt.
	 W ten sposób nag³ówki do³¹czamy tak: #include "konnekt/nazwa_naglowka.h"

 \section chg_031011 2003-10-11   v0.6.18.101
   - Identyfikator tablicy jest teraz typu tTableId.
   - Zamiast sSETCOL i IMC_CNT_SETCOL i IMC_CFG_SETCOL wysy³amy teraz strukturê
     sIMessage_setColumn, albo u¿ywamy funkcji SetColumn().
   - Wycofane, ale nadal dzia³aj¹ce identyfikatory znajduj¹ siê w plug_obsolete.h
   - W sMESSAGESELECT mo¿na podaæ któr¹ z kolei wiadomoœæ spe³niaj¹c¹ warunki przyj¹æ
   - #IMI_ICONGET
   - Wtyczki obs³uguj¹ce sieci musz¹ obs³ugiwaæ #IM_GET_UID
   - wtyczka kSound zmieni³a identyfikatory swoich komunikatów ze wzglêdu na fakt, ¿e poprzednie kolidowa³y z komunikatami interfejsu!
   - sCNTSEARCH jest teraz znacznie wiêksze
   - Wtyczka kLAN udostêpnia swoje API podobnie jak GaduGadu


 \section chg_030919 2003-09-19   v0.6.18.100
   - Nowy komunikat akcji - #ACTN_SETCNT wysy³any do akcji ze stylem
     #ACTR_SETCNT w momencie, gdy zmieniany jest kontakt przypisany
	 do kontrolki i trzeba np. uaktualniæ uchwyty.
   - Wiadomoœci z flag¹ MF_HTML zawieraj¹ w treœci znaczniki html
     oraz zakodowane znaki specjalne (np. < to &lt; itd...)
   - Nowe komunikaty: #IMI_ACTION_SETCNT

 \section chg_030711 2003-07-11   v0.6.18.99
   - Proces ma ustawione zmienne œrodowiskowe:
     %KonnektPath% - katalog programu
	 %KonnektProfile% - katalog profilu
	 %KonnektTemp% - katalog z plikami tymczasowymi
	 %KonnektLog% - katalog z log'ami

 \section chg_030222 2003-05-10 . v0.6.17.98
   <h3>Zmiany:</h3>
   - Nowe komunikaty:
     #IMC_FINDPLUG , #IMI_ICONUNREGISTER , #IMC_LOGDIR
   - Wtyczki, które chc¹ dodaæ swoj¹ ikonkê na g³ówny toolbar
     powinny teraz dodaæ j¹ do grupy, której identyfikator
	 otrzymaj¹ w #IMI_GETPLUGINSGROUP.
   - Podczas sprawdzania kolejki po dodaniu nowej wiadomoœci
     proszê @b sprawdzaæ czy przypadkiem ID zwrócone z #IMC_NEWMESSAGE
	 nie jest równe 0 i jeœli jest zerem, @b nie wywo³ywaæ #IMC_MESSAGEQUEUE!!!
   - Je¿eli u¿ywasz w¹tków od teraz twórz je poprzez cCtrl::BeginThread.
     Dzia³a identycznie jak _beginthreadex i umo¿liwia debugowanie w¹tku.
   - W miêdzy czasie pojawi³o siê kilka ma³o istotnych zmian o których niestety zapomnia³em tu napisaæ :)

 \section chg_030108 2003-01-08 . v0.6.17.91
   <h3>Zmiany:</h3>
   W API zasz³y generalnie spore zmiany maj¹ce na celu zwiêkszenie stabilnoœci i
   zmniejszenia prawdopodobieñstwa wyst¹pienia b³êdów w podstawowych czêœciach
   API. Wiêkszoœæ zmian dotyczy tablic danych i kolejki wiadomoœci.
   - Nowe komunikaty:
     #IMC_PLUGID_POS, #IMC_PLUGID_HANDLE, #IMC_PLUG_ID, #IMC_NET_TYPE
   - Mo¿liwoœæ ustawienia wtyczki na liœcie: #IM_PLUG_PRIORITY i PLUGP_enum
   - GETCNTC i GETSTR s¹ teraz "bezpieczne" i zwracaj¹ kopie wartoœci.
     Je¿eli nie robisz natychmiastowej kopii u siebie, zacznij to robiæ, 
     lub wykorzystaj #GETCNTCA / #GETSTRA, lub zablokuj na czas u¿ywania
     wskaŸników tablice...
   - Z cCtrl odpad³y stare DTset i DTget a DTgetEx i DTsetEx nazywaj¹
     siê teraz DTget i DTset.
   - Do SETINT i SETCNTI doszed³ parametr \a mask do automatycznego
     maskowania.
   - Mo¿na blokowaæ dostêp do wierszy w tablicach dla innych wtyczek
     przez cCtrl::lock i cCtrl::unlock.
   - Do alokowania bezpiecznych buforów pojawi³y siê funkcje:
     cCtrl::malloc, cCtrl::free i cCtrl::strdup. Wszystkie trzy przyjmuj¹
     te same parametry co ich odpowiedniki w CRT.
   - #IMC_CNT_CHANGED powinien byæ wys³any po zmianie ustawieñ œwie¿o
     dodanego kontaktu (z flag¹ 0) i w przypadku, gdy zmieniamy
     net lub UID kontaktu z odpowiednimi wartoœciami w _changed i ustawionym starym net i UID.
   - Zmiana statusu kontaktów odbywa siê teraz przez #IMC_CNT_SETSTATUS, a najlepiej
     przez gotow¹ do tego celu funkcjê CntSetStatus().
   - Wtyczka przed zmian¹ swojego statusu musi wywo³aæ PlugStatusChange().
   - W zwi¹zku z powy¿szym, wtyczki bêd¹ otrzymywa³y komunikaty #IM_STATUSCHANGE i 
     #IM_CNT_STATUSCHANGE
   - #IM_GET_STATUS i #IM_GET_STATUSINFO pozwol¹ dowiedzieæ siê jaki wtyczka ma aktualnie status.
   

 \section chg_021201 2002-11-01 . v0.6.17.x
   <h3>Zmiany:</h3>
   - Zmiana roz³o¿enia bitów w identyfikatorach ikonek.
     Od teraz type i frame maja po 4 bity, a id - 16. 
     Wystarczy przekompilowaæ...
   - Zmiana identyfikatorów statusów

 \section chg_021110 2002-11-10 . v0.6.16.x

   <h3>Zmiany:</h3>

   - Nowe powiadomienia:
     - #ACTN_DEFAULT
   - \ref acts_ otrzyma³y nazwy bardziej zwi¹zane ze swoimi kontrolkami.
     Niektórym zmieni³y siê równie¿ wartoœci!
   - Nowy sposób na zapisu/odczyt ustawieñ. Do cCtrl dosz³y dwie nowe funkcje - 
     cCtrl::DTgetEx i cCtrl::DTsetEx, obie wykorzystuj¹ce strukturê sDTValue.
     Wszystkie makra typu GETSTR , SETINT itp. s¹ teraz funkcjami wykorzystuj¹cymi
     nowy sposób. Nowoœci¹ jest automatyczna konwersja typów dokonywana przez
     rdzeñ, dziêki czemu nie musimy siê zbytnio martwiæ o zgodnoœæ typów!
   - Obs³uga listy ostatnich wpisów.
     sIMessage_MRU , sMRU , #IMC_MRU_GET , #IMC_MRU_SET , #IMC_MRU_UPDATE
   - Kilka drobnostek, które wprowadza³em na ¿ywo, dawno i ich nie pamiêtam :) 


 \section chg_021004 2002-10-30 . v0.6.15.x
  \attention Wtyczki które maj¹ wspó³pracowaæ z wersj¹ 0.6.15 i wy¿ej MUSZ¥ byæ skompilowane z tym SDK!

  <h3>Co zmieniæ w kodzie?</h3>
  Przede wszystkim kompilowaæ wtyczki z do³¹czonymi Ÿród³ami z plug_defs.cpp.

  @change_from
  @code 
int __stdcall IMessageProc(sIMessage * msg) {
    switch (msg->id) {
        ...
        case IM_UIACTION:        return ActionProc((sUIActionNotify*)msg->p1);
@endcode
  @change_to
  @code
int __stdcall IMessageProc(sIMessage_base * msgBase) {
    // Tam gdzie nie u¿ywasz parametrów, najbezpieczniej korzystaæ z msgBase
    // Poni¿sza linijka jest dla Twojej wygody... Wtedy o wiele mniej trzeba zmieniaæ
    sIMessage_2params * msg = (msgBase->s_size>=sizeof(sIMessage_2params))?static_cast<sIMessage_2params*>(msgBase):0;
    // Je¿eli nie chce ci siê na razie nic dalej zmianiaæ - u¿yj poni¿szej (niebezpiecznej) linijki:
 // sIMessage_2params * msg = static_cast<sIMessage_2params*>(msgBase);
    // Pamiêtaj, ¿eby odczytywaæ p1 i p2, TYLKO tam, gdzie bêd¹ one na pewno ustawione! (s¹ przewidziane w dokumentacji)
    switch (msgBase->id) {
        case IM_PLUG_SDKVERSION: return KONNEKT_SDK_V;  // Ta linijka MUSI zostaæ dodana!
        ...
        case IM_UIACTION:        return ActionProc((sUIActionNotify_base*)msg->p1);
@endcode
  @change_end

  a tak¿e ka¿dy *ActionProc wg. wzoru:
  @change_from
  @code
ActionProc(sUIActionNotify * an) {
  if ((an->act.id & IMIB_) == IMIB_CFG) return ActionCfgProc(an); 
  switch (an->act.id) {
  @endcode
  @change_to
  @code
ActionProc(sUIActionNotify_base * anBase) {
  // Poni¿sza linijka s³u¿y TYLKO waszej wygodzie! 
  // Wiêkszoœæ (o ile nie wszystkie) powiadomieñ przesy³ana jest jako sUIActionNotify_2params,
  // korzystamy wtedy z obiektu an, w przeciwnym razie z anBase, lub castujemy do spodziewanego typu.
  sUIActionNotify_2params * an = (anBase->s_size>=sizeof(sUIActionNotify_2params))?static_cast<sUIActionNotify_2params*>(anBase):0;
  // Je¿eli nie chce ci siê na razie nic dalej zmianiaæ - u¿yj poni¿szej (niebezpiecznej) linijki:
// sUIActionNotify_2params * an = static_cast<sUIActionNotify_2params*>(anBase);
  // Pamiêtaj, ¿eby odczytywaæ notify1 i notify2, TYLKO tam, gdzie bêd¹ one na pewno ustawione! (s¹ przewidziane w dokumentacji)

  if ((anBase->act.id & IMIB_) == IMIB_CFG) return ActionCfgProc(anBase); 
  switch (anBase->act.id) {
      // We wszystkich miejscach, gdzie nie znamy jeszcze typu struktury u¿ywamy anBase czyli:
      //   wszêdzie gdzie sprawdzamy pola act, czy code
      //   wszystkie ACTIONONLY(an) trzeba zmieniæ na ACTIONONLY(anBase)
      // Oczywiœcie nie musisz tego robiæ, jeœli u¿yjesz drugiego, mniej bezpiecznego sposobu
  @endcode
  @change_end

  Ca³oœæ skompilowaæ z nowymi nag³ówkami i gotowe!

  <h3>Zmiany:</h3>

  - cia³a wszystkich f-cji u¿ywanych w nag³ówkach zosta³y przeniesione do osobnego .cpp
    którego trzeba skompilowaæ razem z wtyczk¹ (lub skompilowaæ sobie osobno jako .lib).
    W ten sposób nie bêdzie problemów podczas linkowania w wiêkszych, wieloŸród³owych projektach.
  - trochê zmienione sIMessage, wydzielona klasa podstawowa sIMessage_base i wskaŸnik
    do tej w³aœnie klasy powinien obs³ugiwaæ IMessageProc. Przyk³ad jak przerobiæ kod
    w template.cpp. Podobnie jest z sUIActionInfo.
    Dziêki temu zabiegowi, mo¿emy przesy³aæ struktury bez parametrów (ale TYLKO tam,
    gdzie druga strona przyjmie t¹ strukturê!), lub z parametrami innych typów...
  - mo¿liwoœæ tworzenia unikalnych kolumn, poprzez nadawanie kolumnom nazw i pobieranie póŸniej
    przypisanych im identyfikatorów. Nowe pola w sSETCOL.
  - zmiany w \ref develop .
  - dodana sieæ WApilota (#NET_WAPILOT)
  - zmiana zachowania #IMC_CNT_ADD. Teraz, po dodaniu kontaktu, trzeba
    go ustawiæ (lub nie) i wys³aæ #IMC_CNT_CHANGED. Dopiero wtedy
    pozosta³e wtyczki dowiedz¹ siê o jego istnieniu przez #IM_CNT_ADD!
  - #IMC_PROFILEDIR zwraca teraz œcie¿kê aktualnego profilu, a
    #IMC_PROFILESDIR œcie¿kê ze wszystkimi profilami.
  - Nowe pole sDIALOG_choose::width, przy pomocy którego mo¿na okreœliæ d³ugoœæ przycisków.
  - Nowe pole sDIALOG_enter::maxLength do okreœlenia maksymalnej d³ugoœci wpisywanego tekstu.
  - Od tej wersji rdzeñ zapisuje u¿ywane wersje wtyczek. 
  - Mo¿liwoœæ dodawania w³asnych okien do Konfiguracji, Toolbarów i Band jako akcje #ACTT_HWND.
  - dodane komunikaty:
    - #IMI_GROUP_ACTIONSCOUNT
    - #IMI_ACTION_EXISTS
    - #IMI_ACTION_ISGROUP
    - #IMI_ACTION_GETID
    - #IMI_ACTION_REMOVE
    - #IMI_ACTION_GETTYPE
    - #IMI_GROUP_GETHANDLE
    - #IMI_ICONREGISTER
    - #IMI_ICONREGISTERLIST
    - #IMI_ICONEXISTS
    - #IMC_MESSAGEACK
    - #IMC_ISWINXP
    - #IM_MSG_ACK
    - #IM_PLUG_UPDATE
    - #IM_PLUG_SDKVERSION
  - dodane kolumny konfiguracji:
    - #CFG_UIGRPTABS_MULTILINE
    - #CFG_UIGRPTABS_VERTICAL
    - #CFG_UILIST_FLATSB
    - #CFG_DESCR_CLEARMANUAL
    - #CFG_UIMSGTITLE
    - #CFG_UITRAYTITLE
    - #CFG_UITRAYTITLEEVENT
    - #CFG_UIDOCKSPACE
    - #CFG_UIDOCKEDALPHA
    - #CNT_STREET
    - #CNT_POSTALCODE
  - dodane statusy akcji
    - #ACTSC_BOLD
  - dodany status
    - #ST_BLOCKING
  - nowa powiadomienia akcji
    - #ACTN_CREATEWINDOW
    - #ACTN_DESTROYWINDOW
  - nowe struktury
    - sUIIconRegister
    - sUIIconRegisterList
    - sUIActionNotify_createWindow
    - sUIActionNotify_destroyWindow
    - cMessageAck - na razie ma³o przydatna, ale w nastêpnej wersji bêdzie o wiele bardziej u¿ywana
  - Nowy typ wiadomoœci - #MT_URL . Jako cMessage::ext przyjmuje Width , Height i Title
  - cCtrl::DTgetNameID
  - cCtrl::GetTempBuffer
  - nowe identyfikatory
    - #IML_enum
  - #CFG_DESCR_MANUAL zmienione na #CFG_DESCR_USEMANUAL
  - #CFG_UIWINDOWTITLE zmienione na #CFG_UIMAINTITLE
  - Nowy sposób rejestrowania ikon. #IMI_ICONREGISTER.


*/