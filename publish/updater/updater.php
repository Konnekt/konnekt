<?
/*
  Generuje XMLa i paczki dla update.dll
*/

include("updater_engine.php");

echo "\nUPDATEr - start\n";


define('DIR' , '..\\');
define('DATA_DIR' , 's:/home/konnekt/update/data/');
//define('DL_URL' , 'http://test.konnekt.info/update/dlpack.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%'); // Og�lny URL do pobierania plik�w/paczek
define('DL_URL' , 'http://konnekt.info/update/dlpack.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%'); // Og�lny URL do pobierania plik�w/paczek


$sections = new cBaseSection();

  $section = new cSection('sKonnekt' , 'Konnekt '.getVersion(DIR.'__main\\konnekt.exe','t') , 'Podstawowe pliki');
    $pack = new cPack('UPD' , 'Instalator' , '');
		$pack->type = '';
    $pack->info = 'Programik instaluj�cy pobrane pliki. Najlepiej nie usuwa� go z katalogu tymczasowego.';
    $pack->path =  "%konnektTemp%\\kUpdate\\";
    $pack->addFiles('upd.exe');
    $pack->required = true;
		$pack->hidden = false;
  $section->addPack($pack);
    $pack = new cPack('k_konnekt' , 'Program' , DIR.'__main');
    $pack->addFiles('konnekt.exe');
    $pack->required = true;
    $pack->restart = true;
  $section->addPack($pack);
    $pack = new cPack('k_ui' , 'Interfejs' , DIR.'__main');
    $pack->addFiles('ui.dll');
    $pack->required = true;
    $pack->restart = true;
  $section->addPack($pack);
    $pack = new cPack('k_update' , 'Update' , DIR.'__main');
    $pack->addFiles('plugins\\update.dll');
    $pack->required = true;
    $pack->restart = true;
  $section->addPack($pack);
    $pack = new cPack('k_doc' , 'Dokumentacja' , DIR.'__main');
		$pack->type='zip';
//    $pack->addDesc('~.+\.(txt)');
    $pack->addFiles('~.+\.(txt|html?|chm)',true);
		$pack->depends='k_doc_img';
  $section->addPack($pack);
    $pack = new cPack('k_doc_img' , 'Dokumentacja - grafika' , DIR.'__main');
		$pack->type='zip';
//    $pack->addDesc('~.+\.(txt)');
    $pack->addFiles('~doc/.+\.(gif|jpg|png)' , true);
		$pack->hidden = true;
  $section->addPack($pack);
    $pack = new cPack('k_hist' , 'Historia' , DIR.'__main');
    $pack->info='Ustawienia kolumn historii';
		$pack->type='zip';
    $pack->addFiles('~history/.+' , true);
  $section->addPack($pack);
$sections->addSection($section);


  $sPlugins = new cSection('sPlugins' , 'Wtyczki' , 'Wtyczki');

	
    $pack = new cPack('plug_expimp' , 'exPiMP' , DIR.'_expimp');
    $pack->info = "Export / import ustawie� i kontakt�w do XMLi";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_gg' , 'GG' , DIR.'_gg');
    $pack->depends = 'lib_openssl';
    $pack->info = "Obs�uga sieci GG";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_klan' , 'kLAN' , DIR.'_klan');
    $pack->info = "Obs�uga sieci lokalnej";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);

    $pack = new cPack('plug_crypt' , 'Crypt' , DIR.'_crypt');
    $pack->info = "Szyfrowanie rozm�w dla wtyczek GG i kLAN.";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_ktransfer' , 'kTransfer' , DIR.'_ktransfer');
    $pack->info = "Obs�uga przesy�ania plik�w dla wtyczek GG i kLAN.";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_konnferencja' , 'Konnferencja' , DIR.'_konnferencja');
    $pack->info = "Obs�uga konferencji w GG.";
    $pack->depends = "plug_gg";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);
    $pack = new cPack('plug_kjabber' , 'kJabber' , DIR.'_kjabber');
    $pack->info = "Obs�uga protoko�u Jabber.";
    $pack->hidden = false;
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sPlugins->addPack($pack);

// ------------------------------------------------------------------------------ z podgrupami

    $sKStyle = new cSection('sKStyle' , 'kStyle' , 'Zmiana wygl�du programu (ikony, kolory)');
      $pack = new cPack('plug_kstyle' , 'Wtyczka' , DIR.'_kstyle');
      $pack->addFiles('~.+~themes/.+' , true); // Wszystko opr�cz tego co w themes
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
          $pack->addFiles('~.+' , true); 
          $pack->addDesc('~.+\.xml' , false); 
          $pack->addDesc('~.+\.css' , false); 
          return $pack;
        }
        $sKStyleThemes->addPack(KStyleTheme('Shine','shine','','Torn / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Cube','cube','','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Konnekted people','konnekted people','Dodatkowo do zestawu - sk�rka dla kNotify','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('Strych','strych','Stare ikonki sprzed wersji 0.6.18.105','Hao / Stamina'));
        $sKStyleThemes->addPack(KStyleTheme('XMas special','xmas','','Hao / Stamina'));
    $sKStyle->addSection($sKStyleThemes);
  $sPlugins->addSection($sKStyle);

  
  
    $sSMS = new cSection('sSMS' , 'SMS' , 'Przesy�anie kr�tkich wiadomo�ci tekstowych');
      $pack = new cPack('plug_sms' , 'Wtyczka' , DIR.'_sms');
      $pack->depends = "lib_luaplus";
      $pack->addFiles('~.+~sms/.+' , true); // Wszystko opr�cz tego co w sms
      $pack->addFiles('sms/const.lua' , true); // const.lua
      $pack->restart = true;
    $sSMS->addPack($pack);
    $sSMSGates = new cSection('sSMSGates' , 'Bramki' , 'Bramki do obs�ugi wiadomo�ci');
        function SMSGate($name , $dir , $mask , $info='' , $url='') {
          $pack = new cPack('plug_sms_gates_'.$name , $name , DIR.$dir);
          $pack->depends='plug_sms';
      	  $pack->type = 'zip';
          $pack->info = $info;
          $pack->url = $url;
          $pack->addFiles('~sms/'.$mask.'\..+' , true); 
          $pack->addDesc('~sms/'.$mask.'\.xml' , true); 
          return $pack;
        }
        $sSMSGates->addPack(SMSGate('EraOmnix','_sms','era[omnixp]*' , 'Bramka wymaga rejestracji' , 'http://www.eraomnnix.pl'));
        $sSMSGates->addPack(SMSGate('1rstWAP','_sms','1stwap' , 'SMSy na ca�y �wiat.' , 'http://www.1rstwap.com'));
        $sSMSGates->addPack(SMSGate('Idea','_sms','idea' , '' , 'http://www.idea.pl'));
        $sSMSGates->addPack(SMSGate('Idea MBox','_sms','mbox' , 'Bramka wymaga rejestracji' , 'http://www.idea.pl'));
        $sSMSGates->addPack(SMSGate('Plus','_sms','plus' , '' , 'http://www.plusgsm.pl'));
/*        $sSMSGates->addPack(SMSGate('Jabber','__add','jabber','Umo�liwia wysy�anie wiadomo�ci na Jabbera. by Ol�rin')); */
        $sSMSGates->addPack(SMSGate('Mail','__add','mail','Umo�liwia wysy�anie emaili. by http://Sija.net' , 'http://Sija.net'));
        $sSMSGates->addPack(SMSGate('W-SMS','__add','wsms','Wysy�anie sms�w przez p�atn� bramk� Wawer - SMS.' , 'http://www.wsms.pl'));
    $sSMS->addSection($sSMSGates);
  $sPlugins->addSection($sSMS);

    $sSound = new cSection('sSound' , 'D�wi�k' , 'Zdarzenia d�wi�kowe');
      $pack = new cPack('plug_sound' , 'Wtyczka' , DIR.'_sound');
      $pack->info = "+ domy�lny zestaw";
      $pack->addFiles('~.+' , true); 
      $pack->restart = true;
    $sSound->addPack($pack);
    $sSoundSchemes = new cSection('sSoundSchemes' , 'Schematy' , 'Schematy d�wi�kowe');
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
  
    $sKNotify = new cSection('sKNotify' , 'K.Notify' , 'Wtyczka K.Notify wy�wietla okienko informacji o zdarzeniach (zmiany statusu kontakt�w oraz nadchodz�ce wiadomo�ci). Informacje wy�wietlane s� nad zegarkiem systemowym. Wygl�d okienka mo�na modyfikowa� stosuj�c sk�rki.');
    $sKNotify->$info.= "";
      $pack = new cPack('plug_knotify' , 'Wtyczka' , DIR.'_knotify');
      $pack->addFiles('~.+~knotify/.+' , true); // Wszystko opr�cz tego co w knotify
      $pack->restart = true;
    $sKNotify->addPack($pack);
    $sKNotifySkins = new cSection('sKNotifySkins' , 'Sk�rki' , 'Dodatkowe sk�rki');
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


    $sKIEview = new cSection('sKIEview' , 'kIEview' , 'Kontrolka IE jako okno rozmowy. Emotikony, rozbudowane style graficzne (CSS), grupowanie czasu i rozm�wcy i wiele wi�cej!');
    $sKIEview->$info.= "";
      $pack = new cPack('plug_kieview' , 'Wtyczka' , DIR.'_kieview');
      $pack->addFiles('~.+~(emots|themes)/.+' , true); // Wszystko opr�cz tego co w emots i themes
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
      $sKIEviewEmots->addPack(KIEEmots('Fluffy Smiley', '_default' , 'Torn / Stamina' , 'Domy�lny zestaw podstawowych emotikon (16x16 px)'));
      $sKIEviewEmots->addPack(KIEEmots('sir Quadrat', 'sir Quadrat' , 'Hao / Stamina' , 'Zestaw podstawowych emotikon (12x10 px)'));
        $pack = new cPack('plug_kieview_emots_ggcompat' , 'Definicje dla GG' , DIR.'_kieview');
        $pack->depends='plug_kieview';
		$pack->info = 'Je�eli zamierzasz u�y� zestawy prosto z GG, mo�esz podmieni� pliki emots.txt tymi, zoptymalizowanymi zamiennikami.';
        $pack->type = 'zip';
        $pack->addFiles('~emots/ggemots[12]\\.txt' , true); // Wszystko co w kNotify
      $sKIEviewEmots->addPack($pack);

    $sKIEview->addSection($sKIEviewEmots);

    $sKIEviewStyles = new cSection('sKIEviewStyles' , 'Style' , 'Style okna rozm�w');
        $pack = new cPack('plug_kieview_styles' , 'Podstawowe' , DIR.'_kieview');
        $pack->type = 'zip';
        $pack->addFiles('~themes/.+' , true); // Wszystko co w kNotify
      $sKIEviewStyles->addPack($pack);
    $sKIEview->addSection($sKIEviewStyles);
  $sPlugins->addSection($sKIEview);

  
	
$sections->addSection($sPlugins);



  $section = new cSection('sLibs' , 'Biblioteki' , 'Wymagane biblioteki');
    $pack = new cPack('lib_devil' , 'Biblioteki obrazu' , DIR.'__main');
    $pack->addFiles('~(devil|ilu|ilut)\.dll');
    $pack->restart = true;
  $section->addPack($pack);
    $pack = new cPack('lib_luaplus' , 'Silnik skryptowy LuaPlus' , DIR.'__main');
    $pack->addFiles('luaplus.dll');
    $pack->restart = true;
  $section->addPack($pack);
    $pack = new cPack('lib_openssl' , 'OpenSSL' , DIR.'__main');
    $pack->addFiles('~(lib|ssl)eay32\.dll');
    $pack->restart = true;
  $section->addPack($pack);
$sections->addSection($section);


echo ".Sekcje przygotowane\n";
$LAST = unserialize(@file_get_contents('updater.last'));
//$LAST = array(); // Na razie testujemy co innego...
echo ".last wczytany\n";

$xml = fopen(DATA_DIR.'../data.xml' , 'w');
$sections->toXML($xml);
fclose($xml);
echo ".XML zapisany\n";
$f = fopen('updater.last' , 'w');
fputs($f , serialize($FILES));
fclose($f);
echo ".last zapisany\n";
?>
