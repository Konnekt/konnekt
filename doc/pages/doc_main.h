// -----------------------------------------------------------------------------------------------
/** \mainpage

        \section wstep Wst�p
        \warning Dokumentacja jak i ca�y projekt s� nieustannie modyfikowane! 
        Zawarte tu informacje mog� i ulegn� zmianie! Spora cz�� dokumentacji jest nieuko�czona, a
        wiele pozycji jest s�abo udokumentowanych! Mo�e si� zdarzy�, �e niekt�re sekcje
        nie s� ju� aktualne!!! <br>
        \warning API jest w trakcie gruntownych zmian. Wtyczki powinny kompilowa�
		si� bez problem�w, najprawdopodobniej jednak, trzeba b�dzie cz�ciej
		zagl�da� do nag��wk�w ni� dokumentacji.


		\warning Pami�taj aby @b koniecznie przejrze� \ref changelog , dokona� ewentualnych
		zmian i skompilowa� swoj� wtyczk� aby by�a zgodna z now� wersj� API.

        <h3>Co? gdzie? jak?</h3>
         - Zacznij przegl�danie dokumentacji od \ref develop. Przeczytaj to uwa�nie i w @b ca�o�ci!
		 - <a href=pages.html>"Dodatkowe strony"</a> to dodatkowe dokumenty, kt�re mog� okaza� si� pomocne...
         - Nast�pnie przejrzyj <a href=modules.html>"Modu�y"</a>. To w nich pogrupowane
           s� referencje plik�w nag��wkowych... Ka�dy modu� opatrzony jest kr�tkim
           komentarzem, kt�ry powinien pom�c rozezna� si� o co chodzi...
         - \ref gr_examples powinny pom�c w rozszyfrowaniu zawarto�ci dokumentacji...
         - Gdy tylko API si� troch� ustabilizuje i czas na to pozwoli pojawi� si� artyku�y
           opisuj�ce poszczeg�lne elementy systemu i spos�b ich wykorzystania...

        Je�eli ju� znasz SDK - zobacz co si� zmieni�o:
        \ref changelog

        <h3>Info</h3>
        <p> \b Konnekt jest programem - \e serwerem o otwartej architekturze.
        Pozwala na do��czenie do \e rdzenia (g��wna aplikacja *.exe)
        dowolnej liczby wzajemnie wsp�pracuj�cych ze sob� \e wtyczek (pliki .dll).
        </p><p>
        Celem tej dokumentacji jest przedstawienie wszystkich materia��w
        potrzebnych do przygotowania wtyczki wsp�pracuj�cej z systemem.
        Do dokumentacji do��czone s� (i w niej opisane) wymagane pliki nag��wkowe.
        </p><p>
        Kody �r�d�owe b�d�ce podstaw� tej dokumentacji by�y testowane w �rodowisku 
		Microsoft Visual C++ 7. Na wersji 6, 7.1 jak i w kompilatorach Borlanda
		nie powinny wyst�pi� �adne problemy...
        </p><p>
        Przed przyst�pieniem do pracy z kodem zapoznaj si� z \ref licencja "licencj�"!
        </p><p>
        Informacje o autorze znajdziesz \ref team "tutaj".
        </p>
        <p>
        Na pocz�tek polecam tekst \ref develop.
        </p>
        \subsection linki Odsy�acze:
        - http://www.konnekt.info/
        - \ref beta
        - \ref info
        - \ref port
        - \ref gr_examples

        \section licencja Licencja

        (c)2003 Stamina<br><Br>
        SDK s�u�y do przygotowywania oprogramowania wsp�pracuj�cego
        z programem Konnekt. Grupa Stamina nie ponosi odpowiedzialno�ci
        za wykorzystanie tego kodu. <br>

        </p>
*/



// --------------------------------------------------------------------------------------------
/**\page team Autor

		Dok�adne informacje o tym, kto przyczyni� si� do projektu znajdziesz tutaj:
		http://www.konnekt.info/kweb/open.php?inc=site/projekt

        &copy; 2003 Rafa� "HAO" Lindemann  ,  <b><a href=http://www.stamina.eu.org/>Stamina</a></b><br>
        kontakt : hao@konnekt.info
        <br><br>
        Dzi�kuj� wszystkim beta-testerom za wsparcie!
*/

// --------------------------------------------------------------------------------------------


/*\page info *Zasady dzia�ania
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/**\page beta Beta testing
  W tej chwili ca�y projekt jest w wersji BETA. Naj�wie�sze informacje znajdziesz na stronie domowej
  projektu - http://www.konnekt.info
*/

// --------------------------------------------------------------------------------------------
/*\page action *Obs�uga akcji
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page msg *Obs�uga wiadomo�ci tekstowych
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page cfg *Obs�uga konfiguracji
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page cnt *Obs�uga kontakt�w
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page core *Rdze�
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page ui *UI
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page plug *Wtyczka
  Materia� jeszcze nie zosta� przygotowany.
*/

// --------------------------------------------------------------------------------------------
/*\page actt *Rodzaje akcji
  Materia� jeszcze nie zosta� przygotowany.
        \subsection act_menu Menu
        \subsection act_tb ToolBar
        \subsection act_cfg Cfg
        \subsection act_band Band
        \subsection act_bar Bar

*/

// --------------------------------------------------------------------------------------------
/**@page howto_network HOWTO: Wtyczka sieciowa
  Obs�uga r�nych protoko��w komunikacyjnych to podstawowa rola Konnekta. Poni�szy artyku�
  ma za zadanie tylko wskaza� wszystkie komunikaty, kolumny w tabelach itp. potrzebne
  do pe�nej obs�ugi sieci.

  @warn Na jedn� z przysz�ych wersji Konnekta jest ju� przygotowana zmiana obs�ugi status�w
     na bardziej automatyczn�, dlatego pomijam tutaj spraw� status�w opisowych...

  
  <h3>Na pocz�tek</h3>
  (Najlepiej u�y� przyk�adu template.cpp jako bazy dla wtyczki)
  Prac� zaczynamy w zasadzie od ustalenia warto�ci NET dla naszej wtyczki. Warto��
  ta powinna by� mniejsza od 127, gdy� w przeciwnym razie nie b�dziemy mogli korzysta�
  z ikonek dla sieci. Najlepiej zg�osi� si� do autora i uzgodni� t� warto��, lub
  wykorzysta� t�, kt�ra jest ju� w nag��wkach (@ref net_) i pasuje do naszej sieci.
  
  <h3>Obs�uga podstawowych komunikat�w</h3>
   - #IM_PLUG_TYPE  
	   zwracaj�c po��czenie co najmniej: #IMT_MESSAGE | #IMT_PROTOCOL | #IMT_CONTACT | # IMT_NET | #IMT_MESSAGEACK | #IMT_NETUID
	   Je�eli wtyczka ma obs�ugiwa� wyszukiwanie kontakt�w trzeba te� doda�: #IMT_NETSEARCH
   - #IM_PLUG_NETNAME zwracaj�c nazw� sieci
   - #IM_PLUG_PRIORITY wszystkie istniej�ce wtyczki sieciowe zwracaj� PLUGP_LOW, nie nale�y si� wi�c wy�amywa�...
   - #IM_CONNECT i #IM_DISCONNECT
   - #IM_GET_STATUS
   - #IM_GET_STATUSINFO
   - #IM_GET_UID
   - #IM_ISCONNECTED
  <h3>Odbieranie wiadomo�ci</h3>
   Gdy odbieramy wiadomo�� z sieci, trzeba wstawi� j� do kolejki, �eby u�ytkownik
   i pozosta�e wtyczki mog�y j� przeczyta�.
   Tworzymy wi�c i wype�niamy obiekt typu cMessage. W cMessage::flag najlepiej
   ustawi� #MF_HANDLEDBYUI. W ten spos�b otwieraniem wiadomo�ci, ustawieniem ikonki
   itd. zajmie si� samo UI.
   Gotowe wiadomo�ci trzeba doda� do kolejki i t� kolejk� przejrze�...
   Dla jednej wiadomo�ci:
   @code
   cMessage msg;
   // ...
   cMESSAGESELECT ms;
   // Chcemy przejrze� kolejk� TYLKO dla tej jednej wiadomo�ci!
   if ((ms.id = Ctrl->ICMessage(IMC_NEWMESSAGE , (int)&msg)) > 0) {
      Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   }
   @endcode
   Dla kilku:
   @code
   // Dodajemy kilka wiadomo�ci wywo�uj�c IMC_NEWMESSAGE dla ka�dej dodawanej
   Ctrl->ICMessage(IMC_NEWMESSAGE , (int)&msg);
   // Wywo�ujemy kolejk� dla wiadomo�ci o okre�lonym typie, sieci i NIE wysy�anych!
   cMESSAGESELECT ms;
   ms.net = NET_NASZA_SIE�;
   ms.woflag = MF_SEND;
   ms.type = MT_TYP_DODAWANYCH_WIADOMO�CI;
   Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   @endcode
  <h3>Wysy�anie wiadomo�ci</h3>
   Po pierwsze obs�ugujemy #IM_MSG_CHARLIMIT zwracaj�c w nim maksymaln� dopuszczaln�
   d�ugo�� wiadomo�ci.
   Po drugie, po ka�dym nawi�zaniu po��czenia sprawdzamy kolejk�, czy nie ma czego�
   do wys�ania:
   @code
   cMESSAGESELECT ms;
   ms.net = NET_NASZA_SIE�;
   ms.wflag = MF_SEND;
   Ctrl->ICMessage(IMC_MESSAGEQUEUE , (int)&ms);
   @endcode
   Po trzecie, musimy zadeklarowa�, �e obs�ugujemy dan� wiadomo�ci w #IM_MSG_RCV. Np:
   @code
    case IM_MSG_RCV: {
        cMessage * m = (cMessage*)msg->p1;
		if ((m->net == NET_NASZA_SIE�) && (m->type == MT_MESSAGE) && (m->flags & MF_SEND))
		    return IM_MSG_ok; // otrzymamy p�niej IM_MSG_SEND
		else	   	
			return 0;
	}
   @endcode
   Po czwarte, musimy obs�u�y� wysy�an� wiadomo��.
   Je�eli proces zajmuje wi�cej czasu, KONIECZNIE trzeba go wykona� w osobnym w�tku.
   Do tworzenia w�tk�w najlepiej u�y� Controler::BeginThread(), lub Controler::BeginThreadAndWait()
   @code
    case IM_MSG_SEND: {
        cMessage * m = (cMessage*)msg->p1;
		// Lepiej sprawdzi� jeszcze raz... dowolna wtyczka mo�e r�wnie� przes�a� nam IM_MSG_SEND
		if ((m->net == NET_NASZA_SIE�) && (m->type == MT_MESSAGE)) {
			// Po uruchomieniu w�tku wysy�aj�cego funkcja b�dzie czeka� na jego zako�czenie
			// jednocze�nie osb�uguj�c interfejs... W ten spos�b nie blokujemy programu
			// i nie musimy robi� kopii wiadomo�ci. Gdy jednak zajdzie taka potrzeba
			// mo�emy u�y� messageDuplicate() i messageFree().
			Ctrl->BeginThreadAndWait(FunkcjaWysylajaca , m);
			// Zakladamy, �e operacja ta jest zawsze pomy�lna... Wys�an� wiadomo�� trzeba
			// usun�� z kolejki... Mo�na to zrobi� r�wnie� przy pomocy IMC_MESSAGEREMOVE
			return IM_MSG_delete;
		}
		return 0;
   @endcode
  <h3>Lista kontakt�w</h3>
   Gdy trzeba pobra� jaki� kontakt z serwera, otrzymamy #IM_CNT_DOWNLOAD.
   Gdy dane kontaktu zostan� zapisane (w oknie "wi�cej") otrzymamy #IM_CNT_UPLOAD.
   Gdy ulegnie zmianie UID lub sie� dostaniemy #IM_CNT_CHANGED.
   Gdy kontakt zostanie dodany - #IM_CNT_ADD
   Gdy kontakt zostanie usuni�ty - #IM_CNT_REMOVE
  <h3>Lista ignorowanych</h3>
   �eby sprawdzi� czy kontakt znajduje si� na li�cie ignorowanych wysy�amy #IMC_IGN_FIND.
   Je�eli lista ta ulegnie zmianie - dostaniemy #IM_IGN_CHANGED.
  <h3>Statusy</h3>   
   Ka�dy kontakt, jak i wtyczka sieciowa posiadaj� sw�j status. Na status sk�ada
   si� stan (online, offline itd.), oraz flagi. �eby odczyta� stan, trzeba na�o�y�
   na status mask� - status & ST_MASK.
   Gdy kontakt zmieni sw�j status otrzymamy #IM_CNT_STATUSCHANGE
   Gdy kt�ra� z wtyczek zmieni sw�j status otrzymamy #IM_CHANGESTATUS
   Gdy z kolei my zmieniamy status kontaktu u�ywamy CntStatusChange().
   Gdy musimy zmieni� status naszej wtyczki - PlugStatusChange().
   �eby status m�g� by� widoczny w tray'u musimy doda� swoj� grup� akcji do #IMIG_STATUS
   przy pomocy UIActionAdd.
   �eby na belce status�w, w tray'u i na li�cie kontakt�w wy�wietlane by�y prawid�owe ikony status�w,
   musimy zarejestrowa� je pod odpowiednimi identyfikatorami. Np:
   @code
    // W zasobach wtyczki mamy dwie 5 ikonek:
	// 100 - status online
	// 101 - status offline
	// 102-103 - dwie ikonki ��czenia
	// 104 - logo sieci
	// Rejestrujemy na li�cie IML_16 (ikony 16x16) ikonki statusu:
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIE� , ST_ONLINE , 0) , Ctrl->hDll() , 100);
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIE� , ST_OFFLINE , 0) , Ctrl->hDll() , 101);
	// Rejestrujemy animacj� ��czenia:
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIE� , ST_CONNECTING , 0) , Ctrl->hDll() , 102);
	IconRegister(IML_16 , UIIcon(IT_STATUS , NET_NASZA_SIE� , ST_CONNECTING , 1) , Ctrl->hDll() , 103);
	// I na ko�cu logo
	IconRegister(IML_16 , UIIcon(IT_LOGO , NET_NASZA_SIE� , 0 , 0) , Ctrl->hDll() , 104);
   @endcode
   Dodatkowo wypada obs�u�y� komunikaty #IM_AWAY i #IM_BACK  
  <h3>Wyszukiwanie</h3>
   Rozpocz�cie wyszukiwania sygnalizowane jest dostarczeniem komunikatu #IM_CNT_SEARCH
   razem z parametrami. Kolejne znalezione pozycje sygnalizujemy przez 
   #IMI_CNT_SEARCH_FOUND.
  <h3>Dodatkowo...</h3>
   Opr�cz tego, �e d�u�sze (nawet paro sekundowe!) operacje powinny by� wykonywane
   w osobnych w�tkach, powinno si� r�wnie� informowa� u�ytkownika, �e operacja jest
   w toku. S�u�� do tego komunikaty #IMI_LONGSTART, #IMI_LONGSET i #IMI_LONGEND
   razem ze struktur� sDIALOG_long...
  <h3>Na koniec.</h3>
   To z grubsza wszystko, co potrzebne jest do obs�ugi sieci w Konnekcie. Razem
   z nadej�ciem wersji .19.x postaramy si� troch� uporz�dkowa� API i zmieni� obs�ug�
   niekt�rych kluczowych interfejs�w (np. kolumn konfiguracji, kolejki wiadomo�ci, 
   status�w)...

   Powodzenia! :)
   

*/

/**@page howto_subclass HOWTO: Subclassowanie akcji
   Czasami trzeba obs�u�y� akcj�, kt�ra nie zosta�a stworzona przez nasz� wtyczk� (na przyk�ad zmieni� troch� jej pierwotne reagowanie na zdarzenia)...
   Jak to zrobi�? 
   Zasada:
    - wtyczka tworz�ca akcj� otrzymuje wszystkie dotycz�ce jej powiadomienia
	- skoro nie tworzymy akcji, nie dostajemy jej powiadomie�...
	- Konnekt pozwala na swobodne przesy�anie komunikat�w pomi�dzy wtyczkami...
   Praktyka:
    - pobieramy parametry istniej�cej akcji
	- zapisujemy ID dotychczasowego "stw�rcy"
	- kasujemy akcj�
	- TWORZYMY duplikat
	- jeste�my nowym w�a�cicielem, wi�c dostajemy komunikaty...
	- gdy podczas obs�ugi komunikat�w, gdy zrobimy co chcemy, przesy�amy komunikat dalej, do poprzedniego w�a�ciciela akcji, aby wykona� swoje zadania...
   Wykonanie: (przyk�ad dla akcji "Wi�cej" w menu kontakt�w)
   Podczas tworzenia akcji:
   @code
	// tworzymy obiekt z parametrami akcji "Wi�cej"	
    sUIActionInfo nfo(IMIG_CNT , IMIA_CNT_INFO);
	nfo.mask = UIAIM_ALL; // chcemy pobra� wszystko...
	nfo.txt = new char [100];
	nfo.txtSize = 99;
	UIActionGet(nfo); // pobieramy wszystkie dane
	// Zapisujemy w zmiennej globalnej (typu int) poprzedniego w�a�ciciela
	cnt_info_prevOwner = ICMessage(IMI_ACTION_GETOWNER , (int)&nfo.act);
	// Jak dostaniemy b��dny wynik (np. gdy jest stare UI) uznajemy �e w�a�cicielem jest interfejs
	if (cnt_info_prevOwner == 0) {cnt_info_prevOwner = ICMessage(IMC_PLUG_ID , 0);}
	// Niszczymy star� akcj�
	ICMessage(IMI_ACTION_REMOVE , (int)&nfo.act);
	// I reinkarnujemy (powstanie w tym samym miejscu, z tymi samymi parametrami! Nic nie trzeba ustawia�, chyba �e co� sami dodajemy...)
	ICMessage(IMI_ACTION , (int)&nfo);
	delete [] nfo.txt;
	// i ju�... voilla
   @endcode
   Ju� NASZ� akcj� pozostaje tylko obs�u�y�... W obs�udze IM_UIACTION (patrz ActionProc z przyk�ad�w)
   @code
     switch(anBase->act.id) { // typowy switch w ActionProc
	    //...
		case IMIA_CNT_INFO:
			// w tym przypadku, tylko wy�wietlimy monit...
			// Ty mo�esz zrobi� absolutnie co zechcesz, pami�taj tylko, �eby blokowa� (nie przekazywa� dalej) komunikaty �WIADOMIE! Pami�taj, �e kto� inny, r�wnie� mo�e wykona� podobny trick zaraz po tobie...
			if (anBase->code == ACTN_ACTION) {
				if (!ICMessage(IMI_CONFIRM , (int)"Przycisk 'wi�cej' nale�y do MNIE! Odda� go?")) 
					return 0; // nie przekazujemy zdarzenia do UI - UI go nie dostanie - okno si� nie otworzy.. to tak proste!
			}
			// przekazujemy do poprzedniego w�a�ciciela...
			return IMessageDirect(IM_UIACTION , cnt_info_prevOwner , (int)anBase);
	    //...
	 }
   @endcode

*/
