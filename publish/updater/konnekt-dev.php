<?
/*
  Generuje XMLa i paczki dla kUpdate
*/


include("kU-Config.inc");
include("kU-Paczka.php");

$branch = 'konnekt-dev';

showWelcome();

define('DIR' , '..\\');

$DATA_DIR = 's:/home/konnekt/update/dev/';
//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../data-dev__.xml';


$sections = new cRootSection($branch);
$sections->title = "Konnekt - DEV";
$sections->info  = "Centralka <b>tylko</b> dla wybranych! Pobieraj¹c st¹d materia³y zgadzasz siê z postanowieniami "
  ."niepisanego regulaminu, który mówi, ¿e za rozpowszechnianie pobranych st¹d danych, nastêpuje natychmiastowy przepadek mienia - "
  ."Twojego, Twojej rodziny i Twoich s¹siadów w promieniu kilometra. Beware!"

;
$sections->dlUrl = 'dlpack-dev.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%';

/**/
  $sTest = new cSection('sDev' , 'Wersje rozwojowe' , 'Wtyczki/pliki do przetestowania...');


    $pack = new cPack('k_ui' , 'Interfejs' , DIR.'__main');
    $pack->addFiles('ui.dll');
    $pack->restart = true;
    $pack->depends = 'lib_msvc71;lib_devil';
  $sTest->addPack($pack);
	
/*	
    $pack = new cPack('k_hist' , 'Historia' , DIR.'__main');
    $pack->info='Ustawienia kolumn historii';
	  $pack->type='zip';
    $pack->addFiles('~data/history/.+' , true);
  $sTest->addPack($pack);
*/
/*
    $pack = new cPack('k_img' , 'Grafika' , DIR.'__main');
    $pack->info='Grafika';
//	  $pack->type='zip';
    $pack->addFiles('~data/img/.+' , true);
  $sTest->addPack($pack);
  */
    $pack = new cPack('plug_gg' , 'GG' , DIR.'_gg');
    $pack->depends = 'lib_openssl;lib_msvc71';
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
  
    $pack = new cPack('plug_sms' , 'SMS' , DIR.'_sms');
    $pack->depends = 'lib_luaplus';
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
  
	
    $pack = new cPack('plug_sound' , 'kSound' , DIR.'_sound');
    $pack->addFiles('~plugins/.+' , true);
    $pack->depends = 'lib_msvc71';
    $pack->restart = true;
  $sTest->addPack($pack);

  
    $pack = new cPack('k_konnekt' , 'Program' , DIR.'__main');
    $pack->addFiles('konnekt.exe');
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
  $sTest->addPack($pack);

  
/*    $pack = new cPack('plug_klan' , 'kLAN' , DIR.'_klan');
    $pack->info = "Obs³uga sieci lokalnej";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);*/

/*
    $pack = new cPack('plug_kjabber' , 'kJabber' , DIR.'_kjabber');
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
  $sTest->addPack($pack);
*/  
/*    $pack = new cPack('plug_kieview' , 'kIEview' , DIR.'_kieview');
    $pack->depends = 'lib_msvc71';
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/

  /*
    $pack = new cPack('plug_dwutlenek' , 'dwuTleneK' , DIR.'_tlen');
	$pack->depends = "lib_gnet;lib_glib;lib_gthread;lib_libxml2";
    $pack->addFiles('~.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
  */

/*
    $pack = new cPack('plug_expimp' , 'exPiMP' , DIR.'_expimp');
    $pack->info = "Export / import ustawieñ i kontaktów do XMLi";
    $pack->addFiles('~plugins/.+' , true);
    $pack->restart = true;
  $sTest->addPack($pack);
*/
/*

    $pack = new cPack('plug_kstyle' , 'kStyle' , DIR.'_kstyle');
    $pack->addFiles('~plugins/.+' , true);
    $pack->addFiles('themes/shine/theme.xml' , true);
    $pack->addFiles('themes/shine/preview.gif' , true);
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
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
    $pack = new cPack('UPD2' , 'Instalator' , ''); // Tworzymy paczkê - identyfikator , tytu³ , KATALOG w którym skrypt ma zacz¹æ SZUKAÆ plików
  	$pack->type = ''; // Domyœlny typ to 'gz', mo¿na ustawiæ go na 'zip' lub na pusty (bez kompresji) 
    $pack->info = 'Programik instaluj¹cy pobrane pliki. Najlepiej nie usuwaæ go z katalogu tymczasowego.';
    $pack->path =  "%konnektTemp%\\kUpdate\\"; // œcie¿ka na docelowym komputerze gdzie dane maj¹ byæ ROZPAKOWANE
    $pack->addFiles('upd.exe');  // dodajemy plik do paczki...
//    $pack->required = false;
	  $pack->hidden = false;
  $sTest->addPack($pack);
  */
	/*
    $pack = new cPack('k_update' , 'kUpdate' , DIR.'__main');
    $pack->addFiles('plugins\\update.dll');
    $pack->addFiles('data\\kupdate\\centrals.xml');
    $pack->depends = 'lib_msvc71';
//    $pack->required = true;
    $pack->restart = true;
  $sTest->addPack($pack);
*/
/*
    $pack = new cPack('lib_msvc71' , 'MSVC 7.1' , DIR.'msvc');
 	  $pack->info = "Podstawowe biblioteki, które mog¹ byæ wymagane przez niektóre wtyczki";
    $pack->addFiles('msvcp71.dll');
    $pack->addFiles('msvcr71.dll');
    $pack->restart = true;
	  $pack->system = true;
  $sTest->addPack($pack);
    $pack = new cPack('lib_devil' , 'Biblioteki obrazu' , DIR.'__main');
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
