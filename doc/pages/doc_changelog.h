/**@page changelog Changelog

 @section chg_051207 2005-12-07   v0.6.23
   - IMessageDirect ju� nie uznaje id wtyczki r�wnego 0, jako id wtyczki wywo�uj�cej!!!
   - klasa cCtrl przemianowana na Controler
   - klasa Net::Broadcast do konstruowania flag broadcast�w
   - flagi IMT_* przeniesione do enIMessageType
   - cz�� struktur przeniesiona do przestrzeni Konnekt
   - Du�o zmian w obs�udze DTB, oraz w klasie iTable
   - klasa do obs�ugi wtyczek - iPlugin
   - warto�ci sieci s� teraz typu tNet i mieszcz� si� w przestrzeni Konnekt::Net. Wszystkie wtyczki powinny albo zosta� dodane do tej listy, albo deklarowa� swoj� warto�� net jako:
   @code
   const static tNet foo = 1000;
   // lub
   #define NET_FOO (tNet)1000
   @endcode
   - warto�ci IMT s� teraz w enumeracji enIMessageType


 @section chg_050708 2005-07-08   v0.6.22.137
   - Identyfikator tablicy tTableId jest teraz tTableId
   - Wszystkie g��wne tablice podpi�te s� pod nowe API obs�ugi tablic

 @section chg_050210 2005-02-10   v0.6.22
   - #IM_CNT_CHANGED rozsy�any jest r�wnie� po ka�dym zapisaniu informacji o kontakcie.
   - w #IMC_FINDCONTACT (i teoretycznie we wszystkich miejscach gdzie podajemy kontakt jako par� sie�,UID) mo�emy poda� sie� jako NET_NONE i UID jako identyfikator kontaktu w tablicy (oczywi�cie liczba zamieniona na tekst)
   - #ACTT_LINK - p�aski przycisk z kursorem "�apki"
   - #ACTSEDIT_READONLY - flaga dla p�l edycyjnych tylko do odczytu
   - #IMC_SETCONNECT przyjmuje jako pierwszy parametr r�wnie� minimalny czas odst�pu pomi�dzy pr�bami. Przy okazji modyfikacja zasad dzia�ania - zobacz opis.
 @section chg_041102 2004-11-02   v0.6.20.116
   - Stopniowanie poziom�w trudno�ci interfejsu Konnekt::ShowBits. Co prawda ui.dll jeszcze tego nie obs�uguje, ale nale�y uwzgl�dni� je ju� teraz!
   - Nowe typy kontrolek - #ACTT_SLIDER, #ACTT_RADIO, #ACTT_SPINNER, #ACTT_TIPBUTTON
   - Funkcje do obs�ugi dodatkowych warto�ci przekazywanych do akcji w @a txt - GetCfgParam(), SetCfgParam() i CfgParamExists()...
   - Nowe parametry do przekazywania w polu @a txt akcji - #AP_PARAMS, #AP_TIP, #AP_TIPTITLE (i inne zwi�zane z podpowiedziami...)
   - Gar�� zmian w obs�udze URLi do obrazk�w - #IMI_ICONREGISTER
   - #MF_DONTADDTOHISTORY, #MF_HIDE
   - Funkcja do tworzenia standardowych akcji informacyjnych - UIActionCfgAddInfoBox()
   - Funkcje do tworzenia standardowych akcji z informacj� o wtyczce - UIActionCfgAddPluginInfoBox(), UIActionCfgAddPluginInfoBox2()
   - Wysoko�ci akcji ACTT_INFO, ACTT_HTMLINFO i ACTT_TEXT mo�na poda� jako ujemn� liczb� linijek jakie maj� pomie�ci�. np. -2
   - Podaj�c wysoko�� akcji ACTT_INFO jako 0 zostanie dostosowana do wysoko�ci tekstu
   - Podaj�c szeroko�� akcji ACTT_BUTTON, ACTT_TIPBUTTON, ACTT_CHECK i ACTT_RADIO zostan� dostosowane do szeroko�ci tekstu
     
 @section chg_040519 2004-05-19   v0.6.12.113
   - #IMC_GET_MAINTHREAD
 @section chg_040510 2004-05-10   v0.6.12.112
   - lekko zmienione zachowanie cCtrl::is_TUS()
   - wtyczka SMS udostepnia dodatkowe funkcje...

 @section chg_040301 2004-03-01   v0.6.19.110
   - przypominam, �e warto�ci NET wtyczek musz� by� WI�KSZE ni� 255
   - nowe pola opisuj�ce kontakty @ref cnt_
   - #IMI_ACTION_GETOWNER
   - @ref howto_subclass - nowy @b HOWTO
   - zdj�te ograniczenia w przesy�aniu komunikat�w pomi�dzy wtyczkami (przynajmniej tymczasowo...)
   - #UIAIM_VALUE_CONVERT , #ACTN_CONVERT_TO , #ACTN_CONVERT_FROM , #ACTR_CONVERT
   - UIActionCfgSetValue() , UIActionCfgGetValue() , CntNfoSetValue() , CntNfoGetValue() domy�lnie konwertuj� przekazywane warto�ci!
   - Poprawione flagi ACTSFONT_*
   - #ACTT_HTMLINFO
   - sIMessage_UIMakePopup , #IMI_ACTION_MAKEPOPUPMENU
   - #IM_BEFOREEND
   - cCtrl::QuickShutdown()

 @section chg_031204 2003-12-04   v0.6.18.105
   - dzi�ki #MF_MENUBYUI ustawionemu jako flaga wiadomo�ci interfejs zajmie si�
     obs�ug� akcji w menu kontaktu/zdarze�, kt�ra albo wywo�a przypisan� do 
	 wiadomo�ci akcj�, albo otworzy wiadomo�� przez #IM_MSG_OPEN
   - #ACTN_CREATEGROUP
   - #IM_CNT_COMPOSING , #IM_CNT_COMPOSING_STOP , #IMI_CNT_ISCOMPOSING, #IMI_DLGTOKEN, IM_PLUG_ARGS
   - wtyczki obs�uguj�ce sie� powinny obs�u�y� #IM_PLUG_NETSHORTNAME i IM_PLUG_UIDNAME
   - flaga statusu #ST_COMPOSING
   - kolumna #CNT_STATUS_ICON dla kontakt�w w kt�rej mo�na ustawi� identyfikator
     ikony na li�cie IML_16 kt�ra zast�pi ikonk� statusu...
   - #ACTSMENU_BOLD
   - sDIALOG_token

 \section chg_031126 2003-11-26   v0.6.18.104
   - zamiast IMLOG polecam u�ywa� teraz cCtrl::IMDEBUG(), lub samo IMDEBUG.
     Funkcja sprawdza najpierw "poziom debugowania" danej wtyczki przy pomocy
	 cCtrl::DebugLevel i je�eli odpowiedni rodzaj logowania jest w��czony - wywo�uje
	 zwyk�e IMLOG.
   - wszystkie wymagane pliki nag��wkowe znajduj� si� w katalogu Konnekt. 
     Dodatkowo teraz:
       ui_shared.h to teraz ui.h
       plug_shared.h to teraz plug.h
     W kompilatorze nale�y doda� �cie�k� katalogu w kt�rym znajdzie si� kat. Konnekt.
	 W ten spos�b nag��wki do��czamy tak: #include "konnekt/nazwa_naglowka.h"

 \section chg_031011 2003-10-11   v0.6.18.101
   - Identyfikator tablicy jest teraz typu tTableId.
   - Zamiast sSETCOL i IMC_CNT_SETCOL i IMC_CFG_SETCOL wysy�amy teraz struktur�
     sIMessage_setColumn, albo u�ywamy funkcji SetColumn().
   - Wycofane, ale nadal dzia�aj�ce identyfikatory znajduj� si� w plug_obsolete.h
   - W sMESSAGESELECT mo�na poda� kt�r� z kolei wiadomo�� spe�niaj�c� warunki przyj��
   - #IMI_ICONGET
   - Wtyczki obs�uguj�ce sieci musz� obs�ugiwa� #IM_GET_UID
   - wtyczka kSound zmieni�a identyfikatory swoich komunikat�w ze wzgl�du na fakt, �e poprzednie kolidowa�y z komunikatami interfejsu!
   - sCNTSEARCH jest teraz znacznie wi�ksze
   - Wtyczka kLAN udost�pnia swoje API podobnie jak GaduGadu


 \section chg_030919 2003-09-19   v0.6.18.100
   - Nowy komunikat akcji - #ACTN_SETCNT wysy�any do akcji ze stylem
     #ACTR_SETCNT w momencie, gdy zmieniany jest kontakt przypisany
	 do kontrolki i trzeba np. uaktualni� uchwyty.
   - Wiadomo�ci z flag� MF_HTML zawieraj� w tre�ci znaczniki html
     oraz zakodowane znaki specjalne (np. < to &lt; itd...)
   - Nowe komunikaty: #IMI_ACTION_SETCNT

 \section chg_030711 2003-07-11   v0.6.18.99
   - Proces ma ustawione zmienne �rodowiskowe:
     %KonnektPath% - katalog programu
	 %KonnektProfile% - katalog profilu
	 %KonnektTemp% - katalog z plikami tymczasowymi
	 %KonnektLog% - katalog z log'ami

 \section chg_030222 2003-05-10 . v0.6.17.98
   <h3>Zmiany:</h3>
   - Nowe komunikaty:
     #IMC_FINDPLUG , #IMI_ICONUNREGISTER , #IMC_LOGDIR
   - Wtyczki, kt�re chc� doda� swoj� ikonk� na g��wny toolbar
     powinny teraz doda� j� do grupy, kt�rej identyfikator
	 otrzymaj� w #IMI_GETPLUGINSGROUP.
   - Podczas sprawdzania kolejki po dodaniu nowej wiadomo�ci
     prosz� @b sprawdza� czy przypadkiem ID zwr�cone z #IMC_NEWMESSAGE
	 nie jest r�wne 0 i je�li jest zerem, @b nie wywo�ywa� #IMC_MESSAGEQUEUE!!!
   - Je�eli u�ywasz w�tk�w od teraz tw�rz je poprzez cCtrl::BeginThread.
     Dzia�a identycznie jak _beginthreadex i umo�liwia debugowanie w�tku.
   - W mi�dzy czasie pojawi�o si� kilka ma�o istotnych zmian o kt�rych niestety zapomnia�em tu napisa� :)

 \section chg_030108 2003-01-08 . v0.6.17.91
   <h3>Zmiany:</h3>
   W API zasz�y generalnie spore zmiany maj�ce na celu zwi�kszenie stabilno�ci i
   zmniejszenia prawdopodobie�stwa wyst�pienia b��d�w w podstawowych cz�ciach
   API. Wi�kszo�� zmian dotyczy tablic danych i kolejki wiadomo�ci.
   - Nowe komunikaty:
     #IMC_PLUGID_POS, #IMC_PLUGID_HANDLE, #IMC_PLUG_ID, #IMC_NET_TYPE
   - Mo�liwo�� ustawienia wtyczki na li�cie: #IM_PLUG_PRIORITY i PLUGP_enum
   - GETCNTC i GETSTR s� teraz "bezpieczne" i zwracaj� kopie warto�ci.
     Je�eli nie robisz natychmiastowej kopii u siebie, zacznij to robi�, 
     lub wykorzystaj #GETCNTCA / #GETSTRA, lub zablokuj na czas u�ywania
     wska�nik�w tablice...
   - Z cCtrl odpad�y stare DTset i DTget a DTgetEx i DTsetEx nazywaj�
     si� teraz DTget i DTset.
   - Do SETINT i SETCNTI doszed� parametr \a mask do automatycznego
     maskowania.
   - Mo�na blokowa� dost�p do wierszy w tablicach dla innych wtyczek
     przez cCtrl::lock i cCtrl::unlock.
   - Do alokowania bezpiecznych bufor�w pojawi�y si� funkcje:
     cCtrl::malloc, cCtrl::free i cCtrl::strdup. Wszystkie trzy przyjmuj�
     te same parametry co ich odpowiedniki w CRT.
   - #IMC_CNT_CHANGED powinien by� wys�any po zmianie ustawie� �wie�o
     dodanego kontaktu (z flag� 0) i w przypadku, gdy zmieniamy
     net lub UID kontaktu z odpowiednimi warto�ciami w _changed i ustawionym starym net i UID.
   - Zmiana statusu kontakt�w odbywa si� teraz przez #IMC_CNT_SETSTATUS, a najlepiej
     przez gotow� do tego celu funkcj� CntSetStatus().
   - Wtyczka przed zmian� swojego statusu musi wywo�a� PlugStatusChange().
   - W zwi�zku z powy�szym, wtyczki b�d� otrzymywa�y komunikaty #IM_STATUSCHANGE i 
     #IM_CNT_STATUSCHANGE
   - #IM_GET_STATUS i #IM_GET_STATUSINFO pozwol� dowiedzie� si� jaki wtyczka ma aktualnie status.
   

 \section chg_021201 2002-11-01 . v0.6.17.x
   <h3>Zmiany:</h3>
   - Zmiana roz�o�enia bit�w w identyfikatorach ikonek.
     Od teraz type i frame maja po 4 bity, a id - 16. 
     Wystarczy przekompilowa�...
   - Zmiana identyfikator�w status�w

 \section chg_021110 2002-11-10 . v0.6.16.x

   <h3>Zmiany:</h3>

   - Nowe powiadomienia:
     - #ACTN_DEFAULT
   - \ref acts_ otrzyma�y nazwy bardziej zwi�zane ze swoimi kontrolkami.
     Niekt�rym zmieni�y si� r�wnie� warto�ci!
   - Nowy spos�b na zapisu/odczyt ustawie�. Do cCtrl dosz�y dwie nowe funkcje - 
     cCtrl::DTgetEx i cCtrl::DTsetEx, obie wykorzystuj�ce struktur� sDTValue.
     Wszystkie makra typu GETSTR , SETINT itp. s� teraz funkcjami wykorzystuj�cymi
     nowy spos�b. Nowo�ci� jest automatyczna konwersja typ�w dokonywana przez
     rdze�, dzi�ki czemu nie musimy si� zbytnio martwi� o zgodno�� typ�w!
   - Obs�uga listy ostatnich wpis�w.
     sIMessage_MRU , sMRU , #IMC_MRU_GET , #IMC_MRU_SET , #IMC_MRU_UPDATE
   - Kilka drobnostek, kt�re wprowadza�em na �ywo, dawno i ich nie pami�tam :) 


 \section chg_021004 2002-10-30 . v0.6.15.x
  \attention Wtyczki kt�re maj� wsp�pracowa� z wersj� 0.6.15 i wy�ej MUSZ� by� skompilowane z tym SDK!

  <h3>Co zmieni� w kodzie?</h3>
  Przede wszystkim kompilowa� wtyczki z do��czonymi �r�d�ami z plug_defs.cpp.

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
    // Tam gdzie nie u�ywasz parametr�w, najbezpieczniej korzysta� z msgBase
    // Poni�sza linijka jest dla Twojej wygody... Wtedy o wiele mniej trzeba zmienia�
    sIMessage_2params * msg = (msgBase->s_size>=sizeof(sIMessage_2params))?static_cast<sIMessage_2params*>(msgBase):0;
    // Je�eli nie chce ci si� na razie nic dalej zmiania� - u�yj poni�szej (niebezpiecznej) linijki:
 // sIMessage_2params * msg = static_cast<sIMessage_2params*>(msgBase);
    // Pami�taj, �eby odczytywa� p1 i p2, TYLKO tam, gdzie b�d� one na pewno ustawione! (s� przewidziane w dokumentacji)
    switch (msgBase->id) {
        case IM_PLUG_SDKVERSION: return KONNEKT_SDK_V;  // Ta linijka MUSI zosta� dodana!
        ...
        case IM_UIACTION:        return ActionProc((sUIActionNotify_base*)msg->p1);
@endcode
  @change_end

  a tak�e ka�dy *ActionProc wg. wzoru:
  @change_from
  @code
ActionProc(sUIActionNotify * an) {
  if ((an->act.id & IMIB_) == IMIB_CFG) return ActionCfgProc(an); 
  switch (an->act.id) {
  @endcode
  @change_to
  @code
ActionProc(sUIActionNotify_base * anBase) {
  // Poni�sza linijka s�u�y TYLKO waszej wygodzie! 
  // Wi�kszo�� (o ile nie wszystkie) powiadomie� przesy�ana jest jako sUIActionNotify_2params,
  // korzystamy wtedy z obiektu an, w przeciwnym razie z anBase, lub castujemy do spodziewanego typu.
  sUIActionNotify_2params * an = (anBase->s_size>=sizeof(sUIActionNotify_2params))?static_cast<sUIActionNotify_2params*>(anBase):0;
  // Je�eli nie chce ci si� na razie nic dalej zmiania� - u�yj poni�szej (niebezpiecznej) linijki:
// sUIActionNotify_2params * an = static_cast<sUIActionNotify_2params*>(anBase);
  // Pami�taj, �eby odczytywa� notify1 i notify2, TYLKO tam, gdzie b�d� one na pewno ustawione! (s� przewidziane w dokumentacji)

  if ((anBase->act.id & IMIB_) == IMIB_CFG) return ActionCfgProc(anBase); 
  switch (anBase->act.id) {
      // We wszystkich miejscach, gdzie nie znamy jeszcze typu struktury u�ywamy anBase czyli:
      //   wsz�dzie gdzie sprawdzamy pola act, czy code
      //   wszystkie ACTIONONLY(an) trzeba zmieni� na ACTIONONLY(anBase)
      // Oczywi�cie nie musisz tego robi�, je�li u�yjesz drugiego, mniej bezpiecznego sposobu
  @endcode
  @change_end

  Ca�o�� skompilowa� z nowymi nag��wkami i gotowe!

  <h3>Zmiany:</h3>

  - cia�a wszystkich f-cji u�ywanych w nag��wkach zosta�y przeniesione do osobnego .cpp
    kt�rego trzeba skompilowa� razem z wtyczk� (lub skompilowa� sobie osobno jako .lib).
    W ten spos�b nie b�dzie problem�w podczas linkowania w wi�kszych, wielo�r�d�owych projektach.
  - troch� zmienione sIMessage, wydzielona klasa podstawowa sIMessage_base i wska�nik
    do tej w�a�nie klasy powinien obs�ugiwa� IMessageProc. Przyk�ad jak przerobi� kod
    w template.cpp. Podobnie jest z sUIActionInfo.
    Dzi�ki temu zabiegowi, mo�emy przesy�a� struktury bez parametr�w (ale TYLKO tam,
    gdzie druga strona przyjmie t� struktur�!), lub z parametrami innych typ�w...
  - mo�liwo�� tworzenia unikalnych kolumn, poprzez nadawanie kolumnom nazw i pobieranie p�niej
    przypisanych im identyfikator�w. Nowe pola w sSETCOL.
  - zmiany w \ref develop .
  - dodana sie� WApilota (#NET_WAPILOT)
  - zmiana zachowania #IMC_CNT_ADD. Teraz, po dodaniu kontaktu, trzeba
    go ustawi� (lub nie) i wys�a� #IMC_CNT_CHANGED. Dopiero wtedy
    pozosta�e wtyczki dowiedz� si� o jego istnieniu przez #IM_CNT_ADD!
  - #IMC_PROFILEDIR zwraca teraz �cie�k� aktualnego profilu, a
    #IMC_PROFILESDIR �cie�k� ze wszystkimi profilami.
  - Nowe pole sDIALOG_choose::width, przy pomocy kt�rego mo�na okre�li� d�ugo�� przycisk�w.
  - Nowe pole sDIALOG_enter::maxLength do okre�lenia maksymalnej d�ugo�ci wpisywanego tekstu.
  - Od tej wersji rdze� zapisuje u�ywane wersje wtyczek. 
  - Mo�liwo�� dodawania w�asnych okien do Konfiguracji, Toolbar�w i Band jako akcje #ACTT_HWND.
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
    - cMessageAck - na razie ma�o przydatna, ale w nast�pnej wersji b�dzie o wiele bardziej u�ywana
  - Nowy typ wiadomo�ci - #MT_URL . Jako cMessage::ext przyjmuje Width , Height i Title
  - cCtrl::DTgetNameID
  - cCtrl::GetTempBuffer
  - nowe identyfikatory
    - #IML_enum
  - #CFG_DESCR_MANUAL zmienione na #CFG_DESCR_USEMANUAL
  - #CFG_UIWINDOWTITLE zmienione na #CFG_UIMAINTITLE
  - Nowy spos�b rejestrowania ikon. #IMI_ICONREGISTER.


*/