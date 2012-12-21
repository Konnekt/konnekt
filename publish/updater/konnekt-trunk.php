<?

/*
  Generuje XMLa i paczki dla kUpdate
*/

/* 
*  WA¯NE
* 
*  Ka¿da paczka musi mieæ swoj¹ _unikaln¹_ nazwê w obrêbie sekcji w której siê znajduje!
*  Je¿eli jest to paczka, od której inne mog¹ byæ zale¿ne - nazwa powinna byæ unikalna
*  w obrêbie ca³ego kUpdate (w³¹cznie z innymi plikami definicji!)
*  To samo tyczy siê sekcji!
*  Nazwy mog¹ zawieraæ TYLKO znaki a-z A-Z 0-9 '-' i '_' 
* 
*  Branch - czyli ga³¹Ÿ - czyli nazwa pliku definicji powinna byæ w miarê opisowa i znowu - unikalna!
* 
*  Dobrym nawykiem jest nazywaæ sekcje:
*  s + nazwa sekcji z Du¿ej litery
*  A paczki:
*  ma³ymi literami. Najlepiej dodaj¹c w nazwie fragmenty kolejnych sekcji
*  oddzielone znakiem _ (dla zapewnienia unikalnoœci) ...
* 
*  Zak³adamy, ¿e u¿ytkownik jest na tyle inteligenty, ¿e nie usunie linka do 
*  podstawowej centralki konnekt.info. Mo¿emy wiêc spokojnie ustawiaæ 
*  atrybuty 'depends' .. Paczka UPD (i program upd.exe) powinna znajdowaæ siê TYLKO w 
*  podstawowej centralce...
* 
*  !!! Wszystkie wpisywane teksty interpretowane s¹ jako XHTML !!!
*      Zamiast <br/> mo¿na u¿ywaæ jednak normalnego znaku nowej linii...
* 
*  Tworzenie podzia³u na sekcje i paczki jest w miarê prosty... Przejrzyj poni¿szy
*  schemat jako przyk³ad, gdzieniegdzie wpisane s¹ odpowiednie komentarze, przejrzyj je
*  wszystkie dok³adnie... 
* 
*  Pamiêtaj, ¿eby przy du¿ej iloœci ma³ych plików u¿ywaæ dla paczki typu zip w³¹cznie 
*  z podawaniem tylko KILKU najbardziej istotnych plików jako plików opisowych (na 
*  podstawie _tylko ich_ dat modyfikacji kUpdate rozpoznaje czy aktualizacja jest
*  potrzebna). U¿ywa siê do tego funkcji addDesc. Gdy nie u¿yjemy AddDesc, wszystkie
*  dodane pliki uznawane s¹ za pliki opisowe... 
*  Dzia³a to _tylko_ dla typu zip! 
*  
*  Jedna istotna informacja dotyczy œcie¿ek szukania plików i ich rozpakowywania na
*  docelowym komputerze:
*  Na przyk³ad: w katalogu c:\a\b\c znajduje siê plik d.txt
*  Chcemy ¿eby zosta³ rozpakowany u u¿ytkownika w c:\x\y\c\d.txt
*  Przy tworzeniu paczki podajemy startowy katalog jako "c:\a\b"
*  W AddFiles podajemy œcie¿kê pliku relatywnie do œcie¿ki startowej czyli
*  "c\d.txt"
*  W zmiennej path paczki podajemy katalog bazowy do rozpakowywania (je¿eli nie podamy
*  nic, bêdzie nim katalog Konnekta) czyli "c:\x\y\"
*  kUpdate podczas instalacji po prostu po³¹czy obie œcie¿ki czyli wyjdzie:
*  "c:\x\y\" + "c\d.txt"
* 
*  Wszystkie œcie¿ki katalogów podajemy z '\' na koñcu! (wyj¹tkiem mo¿e byæ tylko
*  œcie¿ka startowa przy tworzeniu paczki...
* 
*/


include("kU-Paczka.php");
include("kU-Config.inc");

$branch = 'konnekt-trunk';

showWelcome();

define('DIR' , '..\\___trunk\\');

//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../data'.$dataSuffix.'.xml';


$sections = new cRootSection($branch);
$sections->title = "Konnekt";
$sections->info  = "Oficjalne aktualizacje komunikatora Konnekt";

if ($optOldDL) {
  $sections->dlUrl = $UPDATE_URL . 'dlpack.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%';
} else {
  $sections->dlUrl = $UPDATE_URL . 'data/%_sectionname%-%_pack%-%_file%.%type%';
}

  $sKonnekt = new cSection('sKonnekt' , 'Konnekt '/*.getVersion(DIR.'__main\\konnekt.exe','t')*/ , 'Podstawowe pliki'); // Tworzymy sekcjê z parametrami - identyfikator, tytu³, info

  
    $pack = new cPack('k_konnekt' , 'Program' , DIR.'__main');
    $pack->addFiles('konnekt.exe');
    $pack->required = true;
    $pack->restart = true;
  $sKonnekt->addPack($pack);
    $pack = new cPack('k_ui' , 'Interfejs' , DIR.'__main');
    $pack->addFiles('ui.dll');
    $pack->required = true;
    $pack->restart = true;
    $pack->depends = 'lib_devil2';
  $sKonnekt->addPack($pack);
  
    $pack = new cPack('k_update' , 'Update' , DIR.'__main');
    $pack->info = 'Wtyczka kUpdate.';
    $pack->addFiles('plugins\update.dll');
    $pack->addFiles('data\kupdate\centrals.xml');
    $pack->depends = 'lib_stamina;UPD2';
    $pack->required = true;
    $pack->restart = true;
  $sKonnekt->addPack($pack);
	
    $pack = new cPack('k_doc' , 'Dokumentacja' , DIR.'__main');
	  $pack->type='zip';
//    $pack->addDesc('~.+\.(txt)');
    $pack->addFiles('~.+\.(txt|html?|chm|rtf)',true); // ~ na pocz¹tku oznacza, ¿e wzorzec jest wyra¿eniem regularnym które bêdzie dopasowywane do relatywnej œcie¿ki znalezionych plików. Drugi parametr oznacza, ¿eby przegl¹daæ równie¿ podkatalogi
    $pack->required = true;
	  $pack->depends='k_doc_img'; // oddzielona przecinkami lista paczek, których instalacja jest wymagana przy instalacji tej paczki...
  $sKonnekt->addPack($pack);
    $pack = new cPack('k_doc_img' , 'Dokumentacja - grafika' , DIR.'__main');
	  $pack->type='zip';
//    $pack->addDesc('~.+\.(txt)');
    $pack->addFiles('~doc/.+\.(gif|jpg|png|css)' , true);
    $pack->required = true;
	  $pack->hidden = true;
  $sKonnekt->addPack($pack);
    $pack = new cPack('k_hist' , 'Historia' , DIR.'__main');
    $pack->info='Ustawienia kolumn historii';
	  $pack->type='zip';
    $pack->required = true;
    $pack->addFiles('~data/history/.+' , true);
  $sKonnekt->addPack($pack);
    $pack = new cPack('k_img' , 'Grafika' , DIR.'__main');
    $pack->info='Grafika';
    $pack->required = true;
	  $pack->type='';
    $pack->addFiles('~data/img/.+' , true);
  $sKonnekt->addPack($pack);
  
$sections->addSection($sKonnekt);



/**/

  $sPlugins = new cSection('sPlugins' , 'Wtyczki' , 'Wtyczki');

	
    $pack = new cPack('plug_expimp' , 'exPiMP' , DIR.'_expimp');
    $pack->info = "Export / import ustawieñ i kontaktów do XMLi";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_gg' , 'GG' , DIR.'_gg');
    $pack->depends = 'lib_openssl';
    $pack->info = "Obs³uga sieci GG";
    $pack->addFiles('~.+' , true);
    $pack->setFileOptions('~.*libgaduw32\.dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_klan' , 'kLAN' , DIR.'_klan');
    $pack->info = "Obs³uga sieci lokalnej";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);

    $pack = new cPack('plug_crypt' , 'Crypt' , DIR.'_crypt');
    $pack->info = "Szyfrowanie rozmów dla wtyczek GG i kLAN.";
    $pack->addFiles('~.+' , true);
    $pack->setFileOptions('~.*libsim\.dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_ktransfer' , 'kTransfer' , DIR.'_ktransfer');
    $pack->info = "Obs³uga przesy³ania plików dla wtyczek GG i kLAN.";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_konnferencja' , 'Konnferencja' , DIR.'_konnferencja');
    $pack->info = "Obs³uga konferencji w GG.";
    $pack->depends = "plug_gg";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_kjabber' , 'kJabber' , DIR.'_kjabber');
    $pack->info = "Obs³uga protoko³u Jabber.";
  	$pack->depends = "lib_sigc";
    $pack->addFiles('~.+' , true);
    $pack->setFileOptions('~.*jabberoo\.dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
  $sPlugins->addPack($pack);
  
    $pack = new cPack('plug_dwutlenek' , 'DwuTlenek' , DIR.'_tlen');
    $pack->info = "Obs³uga sieci Tlen.";
	$pack->depends = "lib_gnet;lib_glib;lib_gthread;lib_libxml2";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
  
    $pack = new cPack('plug_mejlik' , 'mejliK' , DIR.'_mejlik');
    $pack->info = "Sprawdzanie nowych wiadomoœci w skrzynkach pocztowych.";
    $pack->hidden = false;
	  $pack->depends = "lib_msvc71";
    $pack->addFiles('~.+' , true);
    $pack->setFileOptions('~.*libmail\.dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
  $sPlugins->addPack($pack);

    $pack = new cPack('plug_kboard' , 'kBoard' , DIR.'_kboard');
   	$pack->depends = "Wtyczka umo¿liwia wspó³ne rysowanie na tablicy we wszystkich sieciach obs³ugiwanych przez Konnekt.";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
  

    $pack = new cPack('plug_checky' , 'Checky' , DIR.'_checky');
    $pack->info = "Wtyczka do okresowego wykonywania podanego polecenia (np. skryptu PHP / JS) i sprawdzania / pokazywania jego wyniku. Idealna dla administratorów i majsterkowiczów :)";
    $pack->depends = "lib_msvc71";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
	
// ------------------------------------------------------------------------------ z podgrupami

    $sKStyle = new cSection('sKStyle' , 'kStyle' , 'Zmiana wygl¹du programu (ikony, kolory)');
      $pack = new cPack('plug_kstyle' , 'Wtyczka' , DIR.'_kstyle');
      $pack->addFiles('~.+~themes/.+' , true); // ~ na pocz¹tku oznacza jak poprzednio - dopasowanie jako wyra¿enie regularne. Druga ~ oznacza drugie wyra¿enie regularne które nie mo¿e byæ spe³nione... W efekcie dodajemy wszystko co znajdziemy z wyj¹tkiem zawartoœci katalogu "themes/"
      $pack->restart = true;
    $sKStyle->addPack($pack);
      $pack = new cPack('plug_kstyle_themes' , 'zestawy kolorystyczne' , DIR.'_kstyle');
      $pack->depends='plug_kstyle';
  	  $pack->type = 'zip';
      $pack->addFiles('~themes/[^/]+\.xml' , true); 
    $sKStyle->addPack($pack);
    $sKStyleThemes = new cSection('sKStyleThemes' , 'zestawy ikon' , 'Zestawy z ikonkami');
        function KStyleTheme($name , $dir , $info , $author) {
          $pack = new cPack('plug_kstyle_themes_'.$dir , $name , DIR.'_kstyle/themes/'.$dir);
          $pack->depends='plug_kstyle';
      	  $pack->type = 'zip';
          $pack->path = 'themes\\'.$dir.'\\';
		  if ($info) $info .="\r\n\r\n";
          $pack->info = $info . "Autor: ".$author;
          $pack->addFiles('~.+' , true); // Dodajemy wszystkie pliki stylu do jednego ZIPa.
          $pack->addDesc('~.+\.xml' , false); // Plików jest mnóstwo i zmieniaj¹ siê rzadko. ¯eby nie zaœmiecaæ XMLa z paczkami, jako pliki opisowe (czyli te, których daty modyfikacji przechowywane s¹ w tym XMLu) wybieramy tylko definicje zestawów i style z rozszerzeniami xml i css. Je¿eli zmienimy tylko jedn¹ z grafik, musimy równie¿ zmodyfikowaæ cokolwiek (choæby datê stworzenia pliku) w którymœ *.xml, lub *.css ¿eby kUpdate móg³ wykry¿ zmianê...
          $pack->addDesc('~.+\.css' , false); 
          return $pack;
        }
        $sKStyleThemes->addPack(KStyleTheme('Shine','shine','','Torn / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Cube','cube','','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Konnekted people','konnekted people','Dodatkowo do zestawu - skórka dla kNotify','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Strych','strych','Stare ikonki sprzed wersji 0.6.18.105','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('XMas special','xmas','','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('ArkCrystal','ArkCrystal',"Ikonki pochodz¹ w wiêkszoœci z Crystal Icon Theme by Everaldo Coelho (www.everaldo.com),\na ikonki WPK i Tlena z pakietu Amirose.\n\nAby dla WPK mieæ odpowiednie ikonki, nale¿y skopiowaæ wtyczkê kJabbera i ustawiæ j¹ jako drug¹ ³aduj¹c¹ siê wtyczkê Jabbera.",'Arkadyjczyk'));
    $sKStyle->addSection($sKStyleThemes);
  $sPlugins->addSection($sKStyle);

  
  
    $sSMS = new cSection('sSMS' , 'SMS' , 'Przesy³anie krótkich wiadomoœci tekstowych');
      $pack = new cPack('plug_sms' , 'Wtyczka' , DIR.'_sms');
      $pack->depends = "lib_luaplus";
      $pack->addFiles('~.+~sms/.+' , true); // Wszystko oprócz tego co w sms
      $pack->addFiles('sms/const.lua' , true); // const.lua
      $pack->restart = true;
    $sSMS->addPack($pack);
    $sSMSGates = new cSection('sSMSGates' , 'Bramki' , 'Bramki do obs³ugi wiadomoœci');
        function SMSGate($name , $dir , $mask , $info='' , $url='') {
          $pack = new cPack('plug_sms_gates_'.$name , $name , DIR.$dir);
          $pack->depends='plug_sms';
      	  $pack->type = 'zip';
          $pack->info = $info;
          $pack->url = $url;
          $pack->addFiles('~sms/'.$mask.'\..+' , true); 
          $pack->addDesc('~sms/'.$mask.'\..+' , true); 
          return $pack;
        }
//        $sSMSGates->addPack(SMSGate('EraOmnix','_sms','era[omnixp]*' , 'Bramka wymaga rejestracji' , 'http://www.eraomnnix.pl'));
        $sSMSGates->addPack(SMSGate('EraOmnix','_sms','omnixs' , 'EraOmnix Sponsorowana (Bramka wymaga rejestracji)' , 'http://www.eraomnnix.pl'));
        //$sSMSGates->addPack(SMSGate('1rstWAP','_sms','1stwap' , 'SMSy na ca³y œwiat.' , 'http://www.1rstwap.com'));
        $sSMSGates->addPack(SMSGate('Orange','_sms','orange' , '' , 'http://www.orange.pl'));
        $sSMSGates->addPack(SMSGate('Orange MultiBox','_sms','multibox' , 'Bramka wymaga rejestracji' , 'http://www.orange.pl'));
        $sSMSGates->addPack(SMSGate('Plus','_sms','plus' , '' , 'http://www.plusgsm.pl'));
        $sSMSGates->addPack(SMSGate('Miasto Plusa','_sms','new_mplus','Wysy³anie smsów przez bramkê Miasto Plusa.' , 'http://www.miastoplusa.pl'));
//        $sSMSGates->addPack(SMSGate('Jabber','__add','jabber','Umo¿liwia wysy³anie wiadomoœci na Jabbera. by Olórin'));
//        $sSMSGates->addPack(SMSGate('Mail','__add','mail','Umo¿liwia wysy³anie emaili. by http://Sija.net' , 'http://Sija.net'));
//        $sSMSGates->addPack(SMSGate('W-SMS','__add','wsms','Wysy³anie smsów przez p³atn¹ bramkê Wawer - SMS.' , 'http://www.wsms.pl'));
    $sSMS->addSection($sSMSGates);
  $sPlugins->addSection($sSMS);

    $sSound = new cSection('sSound' , 'DŸwiêk' , 'Zdarzenia dŸwiêkowe');
      $pack = new cPack('plug_sound' , 'Wtyczka' , DIR.'_sound');
      $pack->info = "+ domyœlny zestaw";
      $pack->addFiles('~.+' , true); 
      $pack->restart = true;
    $sSound->addPack($pack);
    $sSoundSchemes = new cSection('sSoundSchemes' , 'Schematy' , 'Schematy dŸwiêkowe');
        function SoundScheme($name , $dir , $info='' , $url='') {
          $pack = new cPack('plug_sound_schemes_'.$dir , $name , DIR.'__add/sounds');
          $pack->depends='plug_sound';
      	  $pack->type = 'zip';
          $pack->info = $info;
          $pack->url = $url;
          $pack->path = "sounds\\";
          $pack->addFiles("$dir.xml" , false); 
          $pack->addFiles("~$dir/.+" , true); 
          $pack->addDesc("$dir.xml" , false); 
          return $pack;
        }
        $sSoundSchemes->addPack(SoundScheme('Elektro','elektro','by Torn / Stamina'));
        $sSoundSchemes->addPack(SoundScheme('SpeedFreak','speedfreak','by Torn / Stamina'));
        $sSoundSchemes->addPack(SoundScheme('Tech','tech','by Torn / Stamina'));
        $sSoundSchemes->addPack(SoundScheme('Voyager','voyager','by Torn / Stamina'));
        $sSoundSchemes->addPack(SoundScheme('Time squad','time-squad','by Torn / Stamina'));
        $sSoundSchemes->addPack(SoundScheme('Summer Sun','summersun','by Torn / Stamina'));
    $sSound->addSection($sSoundSchemes);
  $sPlugins->addSection($sSound);
  
    $sKNotify = new cSection('sKNotify' , 'K.Notify' , 'Wtyczka K.Notify wyœwietla okienko informacji o zdarzeniach (zmiany statusu kontaktów oraz nadchodz¹ce wiadomoœci). Informacje wyœwietlane s¹ nad zegarkiem systemowym. Wygl¹d okienka mo¿na modyfikowaæ stosuj¹c skórki.');
    $sKNotify->info.= "";
      $pack = new cPack('plug_knotify' , 'Wtyczka' , DIR.'_knotify');
      $pack->addFiles('~.+~knotify/.+' , true); // Wszystko oprócz tego co w knotify
      $pack->restart = true;
    $sKNotify->addPack($pack);
    $sKNotifySkins = new cSection('sKNotifySkins' , 'Skórki' , 'Dodatkowe skórki');
        $pack = new cPack('plug_knotify_skins' , 'Podstawowe' , DIR.'_knotify');
        $pack->type = 'zip';
        $pack->addFiles('~knotify/.+' , true); // Wszystko co w kNotify
        $pack->addDesc('~knotify/.+\.ini' , true); // Wszystko co w kNotify
      $sKNotifySkins->addPack($pack);
        function KNSkin($name , $mask , $author='' , $info='' , $url='') {
          $pack = new cPack('plug_knotify_skins_'.$name , $name , DIR.'__add/knotify');
          $pack->depends='plug_knotify';
      	  $pack->type = 'zip';
          if ($author) $info.="\r\n\r\nAutor: $author";
          $pack->info = $info;
          $pack->url = $url;
		  $pack->path = "knotify\\";
          $pack->addFiles('~'.$mask.'\..+' , true); 
          $pack->addDesc('~'.$mask.'\.ini' , true); 
          return $pack;
        }
      $sKNotifySkins->addPack(KNSkin('Hed Kandi', 'hed-kandi' , 'Torn / Stamina'));
      $sKNotifySkins->addPack(KNSkin('Konnekted people', 'konnekted people' , 'Hao / Stamina'));
    $sKNotify->addSection($sKNotifySkins);
  $sPlugins->addSection($sKNotify);


    $sKIEview = new cSection('sKIEview' , 'kIEview' , 'Kontrolka IE jako okno rozmowy. Emotikony, rozbudowane style graficzne (CSS), grupowanie czasu i rozmówcy i wiele wiêcej!');
    $sKIEview->info.= "";
      $pack = new cPack('plug_kieview' , 'Wtyczka' , DIR.'_kieview');
      $pack->addFiles('~.+~(emots|themes)/.+' , true); // Wszystko oprócz tego co w emots i themes
      $pack->restart = true;
    $sKIEview->addPack($pack);
    $sKIEviewEmots = new cSection('sKIEviewEmots' , 'Emotikonki' , 'Zestawy emotikon');
        function KIEEmots($name , $dir , $author='' , $info='' , $url='') {
          $pack = new cPack('plug_kieview_emots_'.$name , $name , DIR.'_kieview/emots');
          $pack->depends='plug_kieview';
      	  $pack->type = 'zip';
          if ($author) $info.="\r\n\r\nAutor: $author";
          $pack->info = $info;
          $pack->url = $url;
		  $pack->path = "emots\\";
          $pack->addFiles('~'.$dir.'/.+' , true); 
          $pack->addDesc('~'.$dir.'/.+\.txt' , true); 
          return $pack;
        }
      $sKIEviewEmots->addPack(KIEEmots('Fluffy Smiley', '_default' , 'Torn / Stamina' , 'Domyœlny zestaw podstawowych emotikon (16x16 px)'));
      $sKIEviewEmots->addPack(KIEEmots('sir Quadrat', 'sir Quadrat' , 'Hao / Stamina' , 'Zestaw podstawowych emotikon (12x10 px)'));
        $pack = new cPack('plug_kieview_emots_ggcompat' , 'Definicje dla GG' , DIR.'_kieview');
        $pack->depends='plug_kieview';
		$pack->info = 'Je¿eli zamierzasz u¿yæ zestawy prosto z GG, mo¿esz podmieniæ pliki emots.txt tymi, zoptymalizowanymi zamiennikami.';
        $pack->type = 'zip';
        $pack->addFiles('~emots/ggemots[12]\\.txt' , true); // Wszystko co w kNotify
      $sKIEviewEmots->addPack($pack);

    $sKIEview->addSection($sKIEviewEmots);

    $sKIEviewStyles = new cSection('sKIEviewStyles' , 'Style' , 'Style okna rozmów');
        $pack = new cPack('plug_kieview_styles' , 'Podstawowe' , DIR.'_kieview');
        $pack->type = 'zip';
        $pack->addFiles('~themes/.+' , true); // Wszystko co w kNotify
      $sKIEviewStyles->addPack($pack);
    $sKIEview->addSection($sKIEviewStyles);
  $sPlugins->addSection($sKIEview);

  
	
$sections->addSection($sPlugins);

/*

  $sLibs = new cSection('sLibs' , 'Biblioteki' , 'Biblioteki, które mog¹ byæ wymagane przez wtyczki. Gdy bêd¹ potrzebne zostan¹ pobrane automatycznie!');
    $pack = new cPack('lib_devil' , 'Biblioteki obrazu' , DIR.'_lib');
    $pack->addFiles('~(devil|ilu|ilut)\.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_luaplus' , 'Silnik skryptowy LuaPlus' , DIR.'_lib');
    $pack->addFiles('luaplus.dll');
    $pack->restart = true;
	$pack->system = false; 
  $sLibs->addPack($pack);
    $pack = new cPack('lib_openssl' , 'OpenSSL' , DIR.'_lib');
    $pack->addFiles('~(lib|ssl)eay32\.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_sigc' , 'SigC' , DIR.'_lib');
    $pack->addFiles('sigc.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_msvc71' , 'MSVC 7.1' , DIR.'msvc');
	$pack->info = "Podstawowe biblioteki, które mog¹ byæ wymagane przez niektóre wtyczki";
    $pack->addFiles('msvcp71.dll');
    $pack->addFiles('msvcr71.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_msvc70' , 'MSVC 7.0' , DIR.'msvc');
	$pack->info = "Podstawowe biblioteki, które mog¹ byæ wymagane przez niektóre wtyczki";
    $pack->addFiles('msvcp70.dll');
    $pack->addFiles('msvcr70.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_gnet' , 'GNet' , DIR.'_lib');
    $pack->addFiles('gnet.dll');
  	$pack->depends = "lib_glib";
    $pack->restart = true;
	  $pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_iconv' , 'IConv' , DIR.'_lib');
    $pack->addFiles('iconv.dll');
    $pack->restart = true;
 	  $pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_glib' , 'GLib' , DIR.'_lib');
    $pack->addFiles('libglib-2.0-0.dll');
	  $pack->depends = "lib_intl";
    $pack->restart = true;
	  $pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_gthread' , 'GThread' , DIR.'_lib');
    $pack->addFiles('libgthread-2.0-0.dll');
	  $pack->depends = "lib_glib";
    $pack->restart = true;
	  $pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_libxml2' , 'LibXML2' , DIR.'_lib');
    $pack->addFiles('libxml2.dll');
	  $pack->depends = "lib_zlib;lib_iconv";
    $pack->restart = true;
	  $pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_zlib' , 'ZLib' , DIR.'_lib');
    $pack->addFiles('zlib.dll');
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_intl' , 'Intl' , DIR.'_lib');
    $pack->addFiles('intl.dll');
	$pack->depends = "lib_iconv";
    $pack->restart = true;
	$pack->system = true;
  $sLibs->addPack($pack);


$sections->addSection($sLibs);
*/
/**/

doOneBranch($sections , $branch , $DATA_XML);


?>
