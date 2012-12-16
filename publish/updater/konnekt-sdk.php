<?
/*
  Generuje XMLa i paczki dla kUpdate
*/


include("kU-Config.inc");
include("kU-Paczka.php");

$branch = 'konnekt-sdk';

showWelcome();


$DATA_DIR = 's:/home/konnekt/update/data/sdk/';
//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../../data-sdk'.$dataSuffix.'.xml';

define('DIR' , "..\\..\\__sdk\\__output\\");


$sections = new cRootSection($branch);
$sections->title = "SDK";
$sections->info  = "Aktualizacje dla osób pisz¹cych wtyczki.<br/>Dodatkowe zasoby znajduj¹ siê pod adresem: <b>http://www.konnekt.info/sdk/</b>";
if ($optOldDL) {
  $sections->dlUrl = $UPDATE_URL . 'dlpack-sdk.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%';
} else {
  $sections->dlUrl = $UPDATE_URL . 'data/sdk/%_sectionname%-%_pack%-%_file%.%type%';
}

/**/

/*
  $sDevExe = new cSection('sDevExe' , 'Program' , 'Wersje Konnekta dla developerów');
    $pack = new cPack('sdk_dev' , 'Dev '.getVersion(DIR.'konnekt_dev.exe','t') , DIR);
    $pack->info = "Nie wysy³a raportów o b³êdach. Nale¿y jej u¿ywaæ podczas testowania tworzonych wtyczek.";
    $pack->addFiles('konnekt_dev.exe');
  $sDevExe->addPack($pack);
    $pack = new cPack('sdk_dev_log' , 'Dev-Log '.getVersion(DIR.'konnekt_dev_log.exe','t') , DIR);
    $pack->info = "Nie wysy³a raportów o b³êdach, oraz loguje wszystkie przesy³ane pomiêdzy wtyczkami komunikaty.";
    $pack->addFiles('konnekt_dev_log.exe');
  $sDevExe->addPack($pack);
$sections->addSection($sDevExe);
*/

  $sSource = new cSection('sSource' , 'ród³a' , 'Potrzebne/przydatne pliki Ÿród³owe');
  $sSource->path = 'SDK\\';
  
    $pack = new cPack('sdk_headers' , 'Nag³ówki' , DIR.'headers');
	$pack->path = $sSource->path . 'headers\\';
	$pack->type = 'zip';
    $pack->info = "Nag³ówki potrzebne do kompilowania wtyczek, oraz plug_defs.cpp, który powinien byæ skompilowany razem z projektem.";
    $pack->addFiles('~.+' , true);
  $sSource->addPack($pack);
    $pack = new cPack('sdk_examples' , 'Przyk³ady' , DIR.'examples');
	$pack->type = 'zip';
	$pack->path = $sSource->path . 'examples\\';
    $pack->info = "ród³a przyk³adowych wtyczek w jêzyku C++. Wiêcej Ÿróde³ znajdziesz pod adresem http://www.konnekt.info/sdk/";
    $pack->addFiles('~.+' , true);
  $sSource->addPack($pack);
$sections->addSection($sSource);

  $sDocs = new cSection('sDocs' , 'Dokumentacja' , 'Dokumentacja API');
  $sDocs->path = 'SDK\\';
  
    $pack = new cPack('sdk_docs' , 'SDK' , DIR);
    $pack->info = "Podstawowa dokumentacja. W razie problemów, proszê usun¹æ plik *.chi.";
    $pack->addFiles('~konnekt_sdk\.ch[m]');
  $sDocs->addPack($pack);

    $pack = new cPack('sdk_docsmsvs2003' , 'SDK dla MS Visual Studio 2003' , DIR);
    $pack->info = "SDK Konnekta zintegrowane z dokumentacj¹ programu Microsoft Visual Studio 2003 (i <b>tylko</b> tej wersji!).";
//    $pack->runnable = true;
    $pack->addFiles('konnekt_sdk_msvs2003.msi');
//  $sDocs->addPack($pack);

$sections->addSection($sDocs);


/**/

doOneBranch($sections , $branch , $DATA_XML);


?>
