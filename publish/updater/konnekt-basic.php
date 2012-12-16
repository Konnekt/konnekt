<?
/*
  Generuje XMLa i paczki dla kUpdate
*/



include("kU-Paczka.php");
include("kU-Config.inc");

$branch = 'konnekt-basic';

showWelcome();

define('DIR' , '..\\');

$DATA_DIR = 's:/home/konnekt/update/data/basic/';
//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../../data-basic'.$dataSuffix.'.xml';


$sections = new cRootSection($branch);
$sections->title = "kUpdate";
$sections->info  = "Podstawowe pliki wymagane przez wtyczk� kUpdate i wszystkie centralki.";
if ($optOldDL) {
	 $sections->dlUrl = $UPDATE_URL . 'dlpack-basic.php?sectionPath=%sectionpath%&pack=%pack%&file=%file%&type=%type%';
} else {
	 $sections->dlUrl = $UPDATE_URL . 'data/basic/%_sectionname%-%_pack%-%_file%.%type%';
}

  $sBasic = new cSection('sBasic' , 'Wymagane' , 'Wymagane pliki'); // Tworzymy sekcj� z parametrami - identyfikator, tytu�, info
  if (@$optOld) {
      $pack = new cPack('UPD' , 'Instalator' , ''); // Tworzymy paczk� - identyfikator , tytu� , KATALOG w kt�rym skrypt ma zacz�� SZUKA� plik�w
    	$pack->type = ''; // Domy�lny typ to 'gz', mo�na ustawi� go na 'zip' lub na pusty (bez kompresji) 
      $pack->info = 'Programik instaluj�cy pobrane pliki. Najlepiej nie usuwa� go z katalogu tymczasowego.';
      $pack->addFiles('upd.exe');  // dodajemy plik do paczki...
      $pack->required = true;
    $sBasic->addPack($pack);
  } else {  
      $pack = new cPack('UPD2' , 'Instalator' , ''); // Tworzymy paczk� - identyfikator , tytu� , KATALOG w kt�rym skrypt ma zacz�� SZUKA� plik�w
    	$pack->type = ''; // Domy�lny typ to 'gz', mo�na ustawi� go na 'zip' lub na pusty (bez kompresji) 
      $pack->info = 'Programik instaluj�cy pobrane pliki. Najlepiej nie usuwa� go z katalogu tymczasowego.';
      $pack->path =  "%konnektTemp%\\kUpdate\\"; // �cie�ka na docelowym komputerze gdzie dane maj� by� ROZPAKOWANE
      $pack->addFiles('upd.exe');  // dodajemy plik do paczki...
      $pack->searchPath = '%KonnektData%\\kUpdate';
      $pack->required = false;
  	  $pack->hidden = false;
      $pack->additional = true;
    $sBasic->addPack($pack);
  }
	/*
    $pack = new cPack('k_update' , 'Update' , DIR.'__main');
    $pack->info = 'Wtyczka kUpdate.';
    $pack->addFiles('plugins\update.dll');
    $pack->addFiles('data\kupdate\centrals.xml');
    $pack->depends = 'lib_msvc71;UPD2';
    $pack->required = true;
    $pack->restart = true;
  $sBasic->addPack($pack);
*/

$sections->addSection($sBasic);

  $sLibs = new cSection('sLibs' , 'Biblioteki' , 'Biblioteki, kt�re mog� by� wymagane przez wtyczki. Gdy b�d� potrzebne zostan� pobrane automatycznie!');

    $pack = new cPack('lib_devil2' , 'Biblioteki obrazu' , DIR.'_lib');
    $pack->addFiles('~data/dll/dev(il|ilu|ilut)\.dll', true);
    $pack->restart = true;
	  $pack->library = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_luaplus' , 'Silnik skryptowy LuaPlus' , DIR.'_lib');
    $pack->addFiles('data/dll/luaplus.dll', true);
    $pack->restart = true;
	  $pack->library = true; /*LUAPLUS JEST MODYFIKOWANA!!!!!*/
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_openssl' , 'OpenSSL' , DIR.'_lib');
    $pack->addFiles('~data/dll/(lib|ssl)eay32\.dll', true);
    $pack->restart = true;
 	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_sigc' , 'SigC' , DIR.'_lib');
    $pack->addFiles('data/dll/sigc.dll', true);
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_msvc71' , 'MSVC 7.1' , DIR.'msvc');
	  $pack->info = "Podstawowe biblioteki, kt�re mog� by� wymagane przez niekt�re wtyczki";
    $pack->addFiles('msvcp71.dll');
    $pack->addFiles('msvcr71.dll');
    $pack->restart = true;
	  $pack->system = true;
    $pack->required = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_msvc70' , 'MSVC 7.0' , DIR.'msvc');
	  $pack->info = "Podstawowe biblioteki, kt�re mog� by� wymagane przez niekt�re wtyczki";
    $pack->addFiles('msvcp70.dll');
    $pack->addFiles('msvcr70.dll');
    $pack->restart = true;
  	$pack->system = true;
    $pack->required = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_gnet' , 'GNet' , DIR.'_lib');
    $pack->addFiles('data/dll/gnet.dll', true);
	  $pack->depends = "lib_glib";
    $pack->restart = true;
	  $pack->library = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_iconv' , 'IConv' , DIR.'_lib');
    $pack->addFiles('data/dll/iconv.dll', true);
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_glib' , 'GLib' , DIR.'_lib');
    $pack->addFiles('data/dll/libglib-2.0-0.dll', true);
	  $pack->depends = "lib_intl";
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_gthread' , 'GThread' , DIR.'_lib');
    $pack->addFiles('data/dll/libgthread-2.0-0.dll', true);
	  $pack->depends = "lib_glib";
    $pack->restart = true;
    $pack->additional = true;
	$pack->system = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_libxml2' , 'LibXML2' , DIR.'_lib');
    $pack->addFiles('data/dll/libxml2.dll', true);
	  $pack->depends = "lib_zlib;lib_iconv";
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_zlib' , 'ZLib' , DIR.'_lib');
    $pack->addFiles('data/dll/zlib.dll', true);
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_intl' , 'Intl' , DIR.'_lib');
    $pack->addFiles('data/dll/intl.dll', true);
  	$pack->depends = "lib_iconv";
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);
    $pack = new cPack('lib_stamina' , 'Stamina.Lib' , DIR.'_lib');
    $pack->addFiles('data/dll/SMemory.dll', true);
  	$pack->depends = "lib_msvc71";
    $pack->restart = true;
	  $pack->system = true;
    $pack->additional = true;
  $sLibs->addPack($pack);


$sections->addSection($sLibs);

/**/

doOneBranch($sections , $branch , $DATA_XML);


?>
