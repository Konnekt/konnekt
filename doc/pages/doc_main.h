// -----------------------------------------------------------------------------------------------
/** \mainpage

        \section wstep Wstêp
        \warning Dokumentacja jak i ca³y projekt s¹ nieustannie modyfikowane! 
        Zawarte tu informacje mog¹ i ulegn¹ zmianie! Spora czêœæ dokumentacji jest nieukoñczona, a
        wiele pozycji jest s³abo udokumentowanych! Mo¿e siê zdarzyæ, ¿e niektóre sekcje
        nie s¹ ju¿ aktualne!!! <br>
        \warning API jest w trakcie gruntownych zmian. Wtyczki powinny kompilowaæ
		siê bez problemów, najprawdopodobniej jednak, trzeba bêdzie czêœciej
		zagl¹daæ do nag³ówków ni¿ dokumentacji.


		\warning Pamiêtaj aby @b koniecznie przejrzeæ \ref changelog , dokonaæ ewentualnych
		zmian i skompilowaæ swoj¹ wtyczkê aby by³a zgodna z now¹ wersj¹ API.

        <h3>Co? gdzie? jak?</h3>
         - Zacznij przegl¹danie dokumentacji od \ref develop. Przeczytaj to uwa¿nie i w @b ca³oœci!
		 - <a href=pages.html>"Dodatkowe strony"</a> to dodatkowe dokumenty, które mog¹ okazaæ siê pomocne...
         - Nastêpnie przejrzyj <a href=modules.html>"Modu³y"</a>. To w nich pogrupowane
           s¹ referencje plików nag³ówkowych... Ka¿dy modu³ opatrzony jest krótkim
           komentarzem, który powinien pomóc rozeznaæ siê o co chodzi...
         - \ref gr_examples powinny pomóc w rozszyfrowaniu zawartoœci dokumentacji...
         - Gdy tylko API siê trochê ustabilizuje i czas na to pozwoli pojawi¹ siê artyku³y
           opisuj¹ce poszczególne elementy systemu i sposób ich wykorzystania...

        Je¿eli ju¿ znasz SDK - zobacz co siê zmieni³o:
        \ref changelog

        <h3>Info</h3>
        <p> \b Konnekt jest programem - \e serwerem o otwartej architekturze.
        Pozwala na do³¹czenie do \e rdzenia (g³ówna aplikacja *.exe)
        dowolnej liczby wzajemnie wspó³pracuj¹cych ze sob¹ \e wtyczek (pliki .dll).
        </p><p>
        Celem tej dokumentacji jest przedstawienie wszystkich materia³ów
        potrzebnych do przygotowania wtyczki wspó³pracuj¹cej z systemem.
        Do dokumentacji do³¹czone s¹ (i w niej opisane) wymagane pliki nag³ówkowe.
        </p><p>
        Kody Ÿród³owe bêd¹ce podstaw¹ tej dokumentacji by³y testowane w œrodowisku 
		Microsoft Visual C++ 7. Na wersji 6, 7.1 jak i w kompilatorach Borlanda
		nie powinny wyst¹piæ ¿adne problemy...
        </p><p>
        Przed przyst¹pieniem do pracy z kodem zapoznaj siê z \ref licencja "licencj¹"!
        </p><p>
        Informacje o autorze znajdziesz \ref team "tutaj".
        </p>
        <p>
        Na pocz¹tek polecam tekst \ref develop.
        </p>
        \subsection linki Odsy³acze:
        - http://www.konnekt.info/
        - \ref beta
        - \ref info
        - \ref port
        - \ref gr_examples

        \section licencja Licencja

        (c)2003 Stamina<br><Br>
        SDK s³u¿y do przygotowywania oprogramowania wspó³pracuj¹cego
        z programem Konnekt. Grupa Stamina nie ponosi odpowiedzialnoœci
        za wykorzystanie tego kodu. <br>

        </p>
*/



// --------------------------------------------------------------------------------------------
/**\page team Autor

		Dok³adne informacje o tym, kto przyczyni³ siê do projektu znajdziesz tutaj:
		http://www.konnekt.info/kweb/open.php?inc=site/projekt

        &copy; 2003 Rafa³ "HAO" Lindemann  ,  <b><a href=http://www.stamina.eu.org/>Stamina</a></b><br>
        kontakt : hao@konnekt.info
        <br><br>
        Dziêkujê wszystkim beta-testerom za wsparcie!
*/

// --------------------------------------------------------------------------------------------


/*\page info *Zasady dzia³ania
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/**\page beta Beta testing
  W tej chwili ca³y projekt jest w wersji BETA. Najœwie¿sze informacje znajdziesz na stronie domowej
  projektu - http://www.konnekt.info
*/

// --------------------------------------------------------------------------------------------
/*\page action *Obs³uga akcji
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page msg *Obs³uga wiadomoœci tekstowych
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page cfg *Obs³uga konfiguracji
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page cnt *Obs³uga kontaktów
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page core *Rdzeñ
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page ui *UI
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page plug *Wtyczka
  Materia³ jeszcze nie zosta³ przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page actt *Rodzaje akcji
  Materia³ jeszcze nie zosta³ przygotowany.
        \subsection act_menu Menu
        \subsection act_tb ToolBar
        \subsection act_cfg Cfg
        \subsection act_band Band
        \subsection act_bar Bar

*/

// --------------------------------------------------------------------------------------------
/**@page howto_network HOWTO: Wtyczka sieciowa
  Obs³uga ró¿nych protoko³ów komunikacyjnych to podstawowa rola Konnekta. Poni¿szy artyku³
  ma za zadanie tylko wskazaæ wszystkie komunikaty, kolumny w tabelach itp. potrzebne
  do pe³nej obs³ugi sieci.

  @warn Na jedn¹ z przysz³ych wersji Konnekta jest ju¿ przygotowana zmiana obs³ugi statusów
     na bardziej automatyczn¹, dlatego pomijam tutaj sprawê statusów opisowych...

  
  <h3>Na pocz¹tek</h3>
  (Najlepiej u¿yæ przyk³adu template.cpp jako bazy dla wtyczki)
  Pracê zaczynamy w zasadzie od ustalenia wartoœci NET dla naszej wtyczki. Wartoœæ
  ta powinna byæ mniejsza od 127, gdy¿ w przeciwnym razie nie bêdziemy mogli korzystaæ
  z ikonek dla sieci. Najlepiej zg³osiæ siê do autora i uzgodniæ t¹ wartoœæ, lub
  wykorzystaæ t¹, która jest ju¿ w nag³ówkach (@ref net_) i pasuje do naszej sieci.
  
  <h3>Obs³uga podstawowych komunikatów</h3>
   - #IM_PLUG_TYPE  
	   zwracaj¹c po³¹czenie co najmniej: #IMT_MESSAGE | #IMT_PROTOCOL | #IMT_CONTACT | # IMT_NET | #IMT_MESSAGEACK | #IMT_NETUID
	   Je¿eli wtyczka ma obs³ugiwaæ wyszukiwanie kontaktów trzeba te¿ dodaæ: #IMT_NETSEARCH
   - #IM_PLUG_NETNAME zwracaj¹c nazwê sieci
   - #IM_PLUG_PRIORITY wszystkie istniej¹ce wtyczki sieciowe zwracaj¹ PLUGP_LOW, nie nale¿y siê wiêc wy³amywaæ...
   - #IM_CONNECT i #IM_DISCONNECT
   - #IM_GET_STATUS
   - #IM_GET_STATUSINFO
   - #IM_GET_UID
   - #IM_ISCONNECTED
  <h3>Odbieranie wiadomoœci</h3>
   Gdy odbieramy wiadomoœæ z sieci, trzeba wstawiæ j¹ do kolejki, ¿eby u¿ytkownik
   i pozosta³e wtyczki mog³y j¹ przeczytaæ.
   Tworzymy wiêc i wype³niamy obiekt typu cMessage. W cMessage::flag najlepiej
   ustawiæ #MF_HANDLEDBYUI. W ten sposób otwieraniem wiadomoœci, ustawieniem ikonki
   itd. zajmie siê samo UI.
   Gotowe wiadomoœci trzeba dodaæ do kolejki i t¹ kolejkê przejrzeæ...
   Dla jednej wiadomoœci:
   @code
   cMessage msg;
   // ...
   cMESSAGESELECT ms;
   // Chcemy przejrzeæ kolejkê TYLKO dla tej jednej wiadomoœci!
   if ((ms.id = Ctrl->ICMessage(IMC_NEWMESSAGE , (int)&msg)) > 0) {
      Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   }
   @endcode
   Dla kilku:
   @code
   // Dodajemy kilka wiadomoœci wywo³uj¹c IMC_NEWMESSAGE dla ka¿dej dodawanej
   Ctrl->ICMessage(IMC_NEWMESSAGE , (int)&msg);
   // Wywo³ujemy kolejkê dla wiadomoœci o okreœlonym typie, sieci i NIE wysy³anych!
   cMESSAGESELECT ms;
   ms.net = NET_NASZA_SIEÆ;
   ms.woflag = MF_SEND;
   ms.type = MT_TYP_DODAWANYCH_WIADOMOŒCI;
   Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   @endcode
  <h3>Wysy³anie wiadomoœci</h3>
   Po pierwsze obs³ugujemy #IM_MSG_CHARLIMIT zwracaj¹c w nim maksymaln¹ dopuszczaln¹
   d³ugoœæ wiadomoœci.
   Po drugie, po ka¿dym nawi¹zaniu po³¹czenia sprawdzamy kolejkê, czy nie ma czegoœ
   do wys³ania:
   @code
   cMESSAGESELECT ms;
   ms.net = NET_NASZA_SIEÆ;
   ms.wflag = MF_SEND;
   Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   @endcode
   Po trzecie, musimy zadeklarowaæ, ¿e obs³ugujemy dan¹ wiadomoœci w #IM_MSG_RCV. Np:
   @code
    case IM_MSG_RCV: {
        cMessage * m = (cMessage*)msg->p1;
		if ((m->net == NET_NASZA_SIEÆ) && (m->type == MT_MESSAGE) && (m->flags & MF_SEND))
		    return IM_MSG_ok; // otrzymamy póŸniej IM_MSG_SEND
		else	   	
			return 0;
	}
   @endcode
   Po czwarte, musimy obs³u¿yæ wysy³an¹ wiadomoœæ.
   Je¿eli proces zajmuje wiêcej czasu, KONIECZNIE trzeba go wykonaæ w osobnym w¹tku.
   Do tworzenia w¹tków najlepiej u¿yæ Controler::BeginThread(), lub Controler::BeginThreadAndWait()
   @code
    case IM_MSG_SEND: {
        cMessage * m = (cMessage*)msg->p1;
		// Lepiej sprawdziæ jeszcze raz... dowolna wtyczka mo¿e równie¿ przes³aæ nam IM_MSG_SEND
		if ((m->net == NET_NASZA_SIEÆ) && (m->type == MT_MESSAGE)) {
			// Po uruchomieniu w¹tku wysy³aj¹cego funkcja bêdzie czekaæ na jego zakoñczenie
			// jednoczeœnie osb³uguj¹c interfejs... W ten sposób nie blokujemy programu
			// i nie musimy robiæ kopii wiadomoœci. Gdy jednak zajdzie taka potrzeba
			// mo¿emy u¿yæ messageDuplicate() i messageFree().
			Ctrl->BeginThreadAndWait(FunkcjaWysylajaca , m);
			// Zakladamy, ¿e operacja ta jest zawsze pomyœlna... Wys³an¹ wiadomoœæ trzeba
			// usun¹æ z kolejki... Mo¿na to zrobiæ równie¿ przy pomocy IMC_MESSAGEREMOVE
			return IM_MSG_delete;
		}
		return 0;
   @endcode
  <h3>Lista kontaktów</h3>
   Gdy trzeba pobraæ jakiœ kontakt z serwera, otrzymamy #IM_CNT_DOWNLOAD.
   Gdy dane kontaktu zostan¹ zapisane (w oknie "wiêcej") otrzymamy #IM_CNT_UPLOAD.
   Gdy ulegnie zmianie UID lub sieæ dostaniemy #IM_CNT_CHANGED.
   Gdy kontakt zostanie dodany - #IM_CNT_ADD
   Gdy kontakt zostanie usuniêty - #IM_CNT_REMOVE
  <h3>Lista ignorowanych</h3>
   ¯eby sprawdziæ czy kontakt znajduje siê na liœcie ignorowanych wysy³amy #IMC_IGN_FIND.
   Je¿eli lista ta ulegnie zmianie - dostaniemy #IM_IGN_CHANGED.
  <h3>Statusy</h3>   
   Ka¿dy kontakt, jak i wtyczka sieciowa posiadaj¹ swój status. Na status sk³ada
   siê stan (online, offline itd.), oraz flagi. ¯eby odczytaæ stan, trzeba na³o¿yæ
   na status maskê - status & ST_MASK.
   Gdy kontakt zmieni swój status otrzymamy #IM_CNT_STATUSCHANGE
   Gdy któraœ z wtyczek zmieni swój status otrzymamy #IM_CHANGESTATUS
   Gdy z kolei my zmieniamy status kontaktu u¿ywamy CntStatusChange().
   Gdy musimy zmieniæ status naszej wtyczki - PlugStatusChange().
   ¯eby status móg³ byæ widoczny w tray'u musimy dodaæ swoj¹ grupê akcji do #IMIG_STATUS
   przy pomocy UIActionAdd.
   ¯eby na belce statusów, w tray'u i na liœcie kontaktów wyœwietlane by³y prawid³owe ikony statusów,
   musimy zarejestrowaæ je pod odpowiednimi identyfikatorami. Np:
   @code
    // W zasobach wtyczki mamy dwie 5 ikonek:
	// 100 - status online
	// 101 - status offline
	// 102-103 - dwie ikonki ³¹czenia
	// 104 - logo sieci
	// Rejestrujemy na liœcie IML_16 (ikony 16x16) ikonki statusu:
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIEÆ , ST_ONLINE , 0) , Ctrl->hDll() , 100);
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIEÆ , ST_OFFLINE , 0) , Ctrl->hDll() , 101);
	// Rejestrujemy animacjê ³¹czenia:
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIEÆ , ST_CONNECTING , 0) , Ctrl->hDll() , 102);
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIEÆ , ST_CONNECTING , 1) , Ctrl->hDll() , 103);
	// I na koñcu logo
	IconRegister(IML_16 , UIIcon(IT_LOGO , NET_NASZA_SIEÆ , 0 , 0) , Ctrl->hDll() , 104);
   @endcode
   Dodatkowo wypada obs³u¿yæ komunikaty #IM_AWAY i #IM_BACK  
  <h3>Wyszukiwanie</h3>
   Rozpoczêcie wyszukiwania sygnalizowane jest dostarczeniem komunikatu #IM_CNT_SEARCH
   razem z parametrami. Kolejne znalezione pozycje sygnalizujemy przez 
   #IMI_CNT_SEARCH_FOUND.
  <h3>Dodatkowo...</h3>
   Oprócz tego, ¿e d³u¿sze (nawet paro sekundowe!) operacje powinny byæ wykonywane
   w osobnych w¹tkach, powinno siê równie¿ informowaæ u¿ytkownika, ¿e operacja jest
   w toku. S³u¿¹ do tego komunikaty #IMI_LONGSTART, #IMI_LONGSET i #IMI_LONGEND
   razem ze struktur¹ sDIALOG_long...
  <h3>Na koniec.</h3>
   To z grubsza wszystko, co potrzebne jest do obs³ugi sieci w Konnekcie. Razem
   z nadejœciem wersji .19.x postaramy siê trochê uporz¹dkowaæ API i zmieniæ obs³ugê
   niektórych kluczowych interfejsów (np. kolumn konfiguracji, kolejki wiadomoœci, 
   statusów)...

   Powodzenia! :)
   

*/

/**@page howto_subclass HOWTO: Subclassowanie akcji
   Czasami trzeba obs³u¿yæ akcjê, która nie zosta³a stworzona przez nasz¹ wtyczkê (na przyk³ad zmieniæ trochê jej pierwotne reagowanie na zdarzenia)...
   Jak to zrobiæ? 
   Zasada:
    - wtyczka tworz¹ca akcjê otrzymuje wszystkie dotycz¹ce jej powiadomienia
	- skoro nie tworzymy akcji, nie dostajemy jej powiadomieñ...
	- Konnekt pozwala na swobodne przesy³anie komunikatów pomiêdzy wtyczkami...
   Praktyka:
    - pobieramy parametry istniej¹cej akcji
	- zapisujemy ID dotychczasowego "stwórcy"
	- kasujemy akcjê
	- TWORZYMY duplikat
	- jesteœmy nowym w³aœcicielem, wiêc dostajemy komunikaty...
	- gdy podczas obs³ugi komunikatów, gdy zrobimy co chcemy, przesy³amy komunikat dalej, do poprzedniego w³aœciciela akcji, aby wykona³ swoje zadania...
   Wykonanie: (przyk³ad dla akcji "Wiêcej" w menu kontaktów)
   Podczas tworzenia akcji:
   @code
	// tworzymy obiekt z parametrami akcji "Wiêcej"	
    sUIActionInfo nfo(IMIG_CNT , IMIA_CNT_INFO);
	nfo.mask = UIAIM_ALL; // chcemy pobraæ wszystko...
	nfo.txt = new char [100];
	nfo.txtSize = 99;
	UIActionGet(nfo); // pobieramy wszystkie dane
	// Zapisujemy w zmiennej globalnej (typu int) poprzedniego w³aœciciela
	cnt_info_prevOwner = ICMessage(IMI_ACTION_GETOWNER , (int)&nfo.act);
	// Jak dostaniemy b³êdny wynik (np. gdy jest stare UI) uznajemy ¿e w³aœcicielem jest interfejs
	if (cnt_info_prevOwner == 0) {cnt_info_prevOwner = ICMessage(IMC_PLUG_ID , 0);}
	// Niszczymy star¹ akcjê
	ICMessage(IMI_ACTION_REMOVE , (int)&nfo.act);
	// I reinkarnujemy (powstanie w tym samym miejscu, z tymi samymi parametrami! Nic nie trzeba ustawiaæ, chyba ¿e coœ sami dodajemy...)
	ICMessage(IMI_ACTION , (int)&nfo);
	delete [] nfo.txt;
	// i ju¿... voilla
   @endcode
   Ju¿ NASZ¥ akcjê pozostaje tylko obs³u¿yæ... W obs³udze IM_UIACTION (patrz ActionProc z przyk³adów)
   @code
     switch(anBase->act.id) { // typowy switch w ActionProc
	    //...
		case IMIA_CNT_INFO:
			// w tym przypadku, tylko wyœwietlimy monit...
			// Ty mo¿esz zrobiæ absolutnie co zechcesz, pamiêtaj tylko, ¿eby blokowaæ (nie przekazywaæ dalej) komunikaty ŒWIADOMIE! Pamiêtaj, ¿e ktoœ inny, równie¿ mo¿e wykonaæ podobny trick zaraz po tobie...
			if (anBase->code == ACTN_ACTION) {
				if (!ICMessage(IMI_CONFIRM , (int)"Przycisk 'wiêcej' nale¿y do MNIE! Oddaæ go?")) 
					return 0; // nie przekazujemy zdarzenia do UI - UI go nie dostanie - okno siê nie otworzy.. to tak proste!
			}
			// przekazujemy do poprzedniego w³aœciciela...
			return IMessageDirect(IM_UIACTION , cnt_info_prevOwner , (int)anBase);
	    //...
	 }
   @endcode

*/
