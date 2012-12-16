<?
/*
  Generuje XMLa i paczki dla kUpdate
*/


include("kU-Paczka.php");
include("kU-Config.inc");

$branch = 'konnekt-beta';

showWelcome();

define('DIR' , '..\\');

$DATA_DIR = 's:/home/konnekt/update/data/beta/';
//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../../data-beta'.$dataSuffix.'.xml';


$sections = new cRootSection($branch);
$sections->title = "Konnekt - Beta";
$sections->info  = "Wersje beta 'oficjalnych' uaktualnie�. Zestaw mo�e by� <u>pusty</u>, gdy nie b�dzie co testowa�..."
."<br/><div align='center' class='inform'><b>UWAGA!</b> Konnekt po tych uaktualnieniach mo�e zachowywa� si� <b>niestabilnie</b>. Zaleca si� zrobienie <b><u>kopii zapasowej</u></b> profilu!"
."<br/>Je�eli nie jeste� <u>do�wiadczonym</u> u�ytkownikiem, lepiej poczekaj troch� na wydanie tych poprawek w zwyk�ych aktualizacjach!"
."<br/><b><u>Instalowanie i u�ywanie tych poprawek na w�asn� odpowiedzialno��! Zosta�e� ostrze�ony...</u></b>"
."<br/>Koniecznie podziel si� spostrze�eniami na <b>http://www.konnekt.info/forum/</b></div>";

if ($optOldDL) {
  $sections->dlUrl = $UPDATE_URL . 'dlpack-beta.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%';
} else {
  $sections->dlUrl = $UPDATE_URL . 'data/beta/%_sectionname%-%_pack%-%_file%.%type%';
}

/**/
  $sTest = new cSection('sTest' , 'Do testowania' , 'Wtyczki/pliki do przetestowania...');

/*
    $pack = new cPack('k_ui' , 'Interfejs' , DIR.'__main');
    $pack->addFiles('ui.dll');
//    $pack->addFiles('history\\print.xsl');
    $pack->restart = true;
    $pack->depends = 'lib_msvc71;lib_devil2;k_hist2;k_img2';
  $sTest->addPack($pack);
*/
	
	
/*    $pack = new cPack('k_hist2' , 'Historia' , DIR.'__main');
    $pack->info='Ustawienia kolumn historii';
	  $pack->type='zip';
    $pack->addFiles('~data/history/.+' , true);
  $sTest->addPack($pack);
    $pack = new cPack('k_img2' , 'Grafika' , DIR.'__main');
    $pack->info='Grafika';
	  $pack->type='';
    $pack->addFiles('~data/img/.+' , true);
  $sTest->addPack($pack);
*/
  
	/*
    $pack = new cPack('plug_gg' , 'GG' , DIR.'_gg');
    $pack->depends = 'lib_openssl;lib_msvc71';
    $pack->addFiles('~.+/.+' , true);
    $pack->setFileOptions('~data.+dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
  $sTest->addPack($pack);
  */
  
  
/*    $pack = new cPack('plug_sms' , 'SMS' , DIR.'_sms');
    $pack->depends = 'lib_luaplus';
    $pack->addFiles('~plugins/.+' , true);
    $pack->addFiles('sms/idea.xml');
    $pack->addFiles('sms/mbox.xml');
    $pack->restart = true;
  $sTest->addPack($pack); */
/*
    $pack = new cPack('smsgate_orange' , 'Bramka orange' , DIR.'_sms');
    $pack->depends = 'plug_sms';
    $pack->addFiles('~sms/orange.+', true);
  $sTest->addPack($pack);
*/
  
/*    $pack = new cPack('plug_sound' , 'kSound' , DIR.'_sound');
    $pack->addFiles('~plugins/.+' , true);
    $pack->depends = 'lib_msvc71';
    $pack->restart = true;
  $sTest->addPack($pack); */

  
/*    $pack = new cPack('k_konnekt' , 'Program' , DIR.'__main');
    $pack->addFiles('konnekt.exe');
    $pack->restart = true;
		$pack->required = true;
    $pack->depends = 'lib_msvc71';
  $sTest->addPack($pack);*/
/*
    $pack = new cPack('plug_konnferencja' , 'Konnferencja' , DIR.'_konnferencja');
    $pack->info = "Obs�uga konferencji w GG.";
    $pack->depends = "plug_gg;lib_msvc71";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/
	/*
    $pack = new cPack('plug_checky' , 'Checky' , DIR.'_checky');
    $pack->info = "Wtyczka do okresowego wykonywania podanego polecenia (np. skryptu PHP / JS) i sprawdzania / pokazywania jego wyniku. Idealna dla administrator�w i majsterkowicz�w :)";
    $pack->depends = "lib_msvc71";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
	*/
	
/*  
    $pack = new cPack('plug_klan' , 'kLAN' , DIR.'_klan');
    $pack->info = "Obs�uga sieci lokalnej";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/
/*
    $pack = new cPack('plug_kjabber' , 'kJabber' , DIR.'_kjabber');
    $pack->addFiles('~.+/.+' , true);
    $pack->setFileOptions('~data.+dll', array(FILEOPT_TARGET => TARGET_LIBRARY));
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
  $sTest->addPack($pack);
  */
/*	
    $pack = new cPack('plug_kieview' , 'kIEview' , DIR.'_kieview');
    $pack->depends = 'lib_msvc71';
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/

  
/*  
    $pack = new cPack('plug_kieviewxp' , 'kIEview (XP)' , DIR.'__add\\kieviewXP');
	$pack->depends = "";
	$pack->info = "<div align='center' class='inform'><b>UWAGA!</b> Wersja mo�e nie dzia�a� (i najpewniej nie zadzia�a) na windows 98/ME! Przysz�e wersje b�d� pozbawione tego problemu... dzi�kuj� :)</div>";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/ 
 /*
    $pack = new cPack('plug_knotify' , 'kNotify' , DIR.'_knotify');
	$pack->depends = "";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
  */
/*
    $pack = new cPack('plug_kboard' , 'kBoard' , DIR.'_kboard');
	$pack->depends = "";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/
  /*
    $pack = new cPack('plug_mejlik' , 'mejliK' , DIR.'_mejlik');
	$pack->depends = "lib_msvc71";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/
/*
    $pack = new cPack('plug_dwutlenek' , 'dwuTleneK' , DIR.'_tlen');
	$pack->depends = "lib_gnet;lib_glib;lib_gthread;lib_libxml2;lib_msvc71";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
  
    $pack = new cPack('plug_expimp' , 'exPiMP' , DIR.'_expimp');
    $pack->info = "Export / import ustawie� i kontakt�w do XMLi";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);

    $pack = new cPack('plug_kstyle' , 'kStyle' , DIR.'_kstyle');
    $pack->addFiles('~plugins/.+' , true);
    $pack->addFiles('themes/shine/theme.xml' , true);
    $pack->addFiles('themes/shine/preview.gif' , true);
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
  $sTest->addPack($pack);
*/

 /*
   $pack = new cPack('plug_ggimage' , 'GG Image' , DIR.'__beta');
    $pack->depends = 'plug_gg';
    $pack->addFiles('plugins/ggimage.dll');
    $pack->restart = true;
  $sTest->addPack($pack);
 */
/*
   $pack = new cPack('plug_ktransfer' , 'KTransfer' , DIR.'_ktransfer');
    $pack->depends = '';
    $pack->addFiles('plugins\\kTransfer.dll');
    $pack->restart = true;
  $sTest->addPack($pack);

  */
  /*
    $pack = new cPack('UPD2' , 'Instalator' , ''); // Tworzymy paczk� - identyfikator , tytu� , KATALOG w kt�rym skrypt ma zacz�� SZUKA� plik�w
  	$pack->type = ''; // Domy�lny typ to 'gz', mo�na ustawi� go na 'zip' lub na pusty (bez kompresji) 
    $pack->info = 'Programik instaluj�cy pobrane pliki. Najlepiej nie usuwa� go z katalogu tymczasowego.';
    $pack->path =  "%konnektTemp%\\kUpdate\\"; // �cie�ka na docelowym komputerze gdzie dane maj� by� ROZPAKOWANE
    $pack->addFiles('upd.exe');  // dodajemy plik do paczki...
//    $pack->required = false;
    $pack->searchPath = '%KonnektData%\\kUpdate';
	  $pack->hidden = false;
  $sTest->addPack($pack);
	*/
/*
    $pack = new cPack('k_update' , 'kUpdate' , DIR.'__main');
    $pack->addFiles('plugins\\update.dll');
    $pack->addFiles('data\\kupdate\\centrals.xml');
    $pack->depends = 'lib_stamina';
    $pack->required = true;
    $pack->restart = true;
  $sTest->addPack($pack); 
*/
/*
    $pack = new cPack('lib_msvc71' , 'MSVC 7.1' , DIR.'msvc');
 	  $pack->info = "Podstawowe biblioteki, kt�re mog� by� wymagane przez niekt�re wtyczki";
    $pack->addFiles('msvcp71.dll');
    $pack->addFiles('msvcr71.dll');
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);


    $pack = new cPack('lib_devil2' , 'Biblioteki obrazu' , DIR.'_lib');
    $pack->addFiles('~data/dll/dev(il|ilu|ilut)\.dll', true);
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);
    $pack = new cPack('lib_gnet' , 'GNet' , DIR.'_lib');
    $pack->addFiles('data/dll/gnet.dll', true);
  	$pack->depends = "lib_glib";
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);
    $pack = new cPack('lib_glib' , 'GLib' , DIR.'_lib');
    $pack->addFiles('data/dll/libglib-2.0-0.dll', true);
	  $pack->depends = "lib_intl";
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);
    $pack = new cPack('lib_gthread' , 'GThread' , DIR.'_lib');
    $pack->addFiles('data/dll/libgthread-2.0-0.dll', true);
	  $pack->depends = "lib_glib";
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);

  */

$sections->addSection($sTest);



/**/

doOneBranch($sections , $branch , $DATA_XML);


?>
