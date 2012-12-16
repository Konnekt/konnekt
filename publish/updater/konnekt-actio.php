<?
/*
  Generuje XMLa i paczki dla kUpdate
*/


include("kU-Config.inc");
include("kU-Paczka.php");

$branch = 'konnekt-actio';

showWelcome();

define('DIR' , '..\\');

$DATA_DIR = 's:/home/konnekt/update/dev/';
//$DATA_DIR = 'data/';
$BACKUP_DIR = 'backup/';

$DATA_XML = $DATA_DIR.'../data-actio__.xml';


$sections = new cRootSection($branch);
$sections->title = "Konnekt - Actio";
$sections->info  = "Centralka <b>tylko</b> dla wybranych! Pobieraj¹c st¹d materia³y zgadzasz siê z postanowieniami "
  ."niepisanego regulaminu, który mówi, ¿e za rozpowszechnianie pobranych st¹d danych, nastêpuje natychmiastowy przepadek mienia - "
  ."Twojego, Twojej rodziny i Twoich s¹siadów w promieniu kilometra. Beware!"

;

$sections->dlUrl = $UPDATE_URL . 'dev/%_sectionname%-%_pack%-%_file%.%type%';

/**/
  $sActio = new cSection('sActio' , 'Konnekt - Actio - VoIP' , '...');

	/*
	  $pack = new cPack('k_konnekt' , 'Program' , DIR.'__main');
    $pack->addFiles('konnekt.exe');
    $pack->addFiles('ui.dll');
    $pack->restart = true;
    $pack->depends = 'lib_msvc71';
  $sActio->addPack($pack);
*/

    $pack = new cPack('k_actio' , 'Wtyczka' , DIR.'_actio');
    $pack->addFiles('plugins/actio.dll');
    $pack->addFiles('~data/dll/.+', true);
    $pack->restart = true;
    $pack->depends = 'lib_msvc71;lib_devil;k_actio_data';
  $sActio->addPack($pack);

    $pack = new cPack('k_actio_data' , 'Dane' , DIR.'_actio');
    $pack->addFiles('~data/actio/.+', true);
    $pack->addFiles('~sounds/.+', true);
		$pack->type='zip';
    $pack->depends = 'lib_msvc71;lib_devil;k_actio';
  $sActio->addPack($pack);

$sections->addSection($sActio);



/**/

doOneBranch($sections , $branch , $DATA_XML);


?>
