<?php
/*
*  kU-Paczka ;)
* 
*  Generuje XMLa i paczki dla kUpdate
*   
* 
* Kod ten, jest wy³¹czn¹ w³asnoœci¹ Rafa³a Lindemanna (mail@konnekt.info). Rozpowszechnianie, edycja, 
* wykorzystywanie kodu w czêœci lub ca³oœci bez _wyraŸnej_ zgody autora jest zabronione!
* Jako wyj¹tek od tej zasady, dopuszcza siê _u¿ywania_ skryptu, w procesie
* przygotowywania wtyczek dzia³aj¹cych w obrêbie projektu Konnekt (http://www.konnekt.info)
* 
* This code is proprietary! Distribution, editing, re-using and even _using_ is stricly PROHIBITED unless
* clear permission is given from the owner of this code (Rafa³a Lindemann - mail@konnekt.info)
* The permission to _use_ this script is given to all Konnekt plugin developers.
* 
*/
set_time_limit(0);
include_once("xmllib.php");
// $XML_decode = "windows-1250";
// Kodowanie konsoli
$XML_decode = "CP852";
// Kodowanie XMLi (kUpdate przyjmuje TYLKO windows-1250!!!)
$XML_CHARSET = "windows-1250";
$FILES = array(); // cache informacji o plikach
$LAST = array(); // stan $FILES z ostatnio generowanego update'u
define('FILE_TIME' , 0);
define('FILE_VER' , 1);
define('FILE_CRC' , 2);

define('FILEOPT_TARGET' , 'target');

define('TARGET_LIBRARY', 2);
define('TARGET_SYSTEM', 3);
define('TARGET_TEMP', 4);

$optIgnore = getCmdVar('-i');
$optAuto = getCmdVar('-auto');
$optRebuild = getCmdVar('-r');
$optNoLog = getCmdVar('-nolog');

// $DOC   - DOMXML z dodatkowymi danymi
/*
$in = fopen('php://stdin' , 'r');
$out = fopen('test.txt' , 'w');
fwrite($out , fgets($in));
fclose($out);
fclose($in);

die ('œ¹¹¹¹œœœæœææœæœæœæœœæœæœæœæ');
*/

/*
 .XML:
 * <kUpdate>
 *  <branch id=''>
 *  <log>  - historia zmian
 *    <revision id='' date=''>  - aktualizacja
 *      <pack id='sciezka_sekcji/nazwa_paczki' time='czas_najstarszego_pliku'>info</pack>
 *    </revision>
 *   
 *   </pack> 
 *  </log>
 *  <ignore>  - lista paczek do zignorowania
 *    <pack id='' time=''/>
 *  </ignore>
 *  </branch> 
 * </kUpdate>

 */

function getCmdVar($name , $getVar = false , $def = '')
{
    for ($i = 1; $i < @$_SERVER['argc']; $i++) {
        $tab = array();
        if (preg_match('#^' . $name . '(?:=(.*))?$#i' , $_SERVER['argv'][$i] , $tab)) {
            if ($getVar)
                return $tab[1]? $tab[1] : $def;
            else
                return true;
        } 
    } 
    return false;
} 

function showWelcome()
{

    ?>
kU-Paczka 2.0 (2004-03-05) Generator paczek dla wtyczki kUpdate
	
Dodatkowe parametry do skryptu:
  -i		- resetuje listê ignorowanych paczek 	
  -r    	- konstruuje tylko nowego XMLa nie sprawdzaj¹c zmian w plikach
  -auto 	- akceptuje wszystkie zmiany automatycznie (bez opisów)
  -nolog	- nie zapisuje rewizji do historii
	<?php
    if (getCmdVar('-\?') || getCmdVar('-help')) {
        exit(0);
    } 
} 

function dataLoad($fn , $xmlfn)
{
    global $LAST , $DOC , $FILES, $optRebuild;
    $lastData = @file_get_contents($fn);
    $LAST = $lastData ? unserialize($lastData) : array();
    if ($optRebuild) {
        $FILES = $LAST;
    } 
    $DOC = XMLLoad($xmlfn , '<?xml version="1.0" encoding="windows-1250"?><kUpdate/>');
} 
function dataSave($fn , $xmlfn)
{
    global $FILES , $DOC;
    $f = fopen($fn , 'w');
    fputs($f , serialize($FILES));
    fclose($f);
    XMLSave($DOC , $xmlfn , false , true);
} 

function doOneBranch(&$sections , $branch , $dataXML)
{
    global $DATA_DIR , $BACKUP_DIR;
    flush();
    echo ".Sekcje przygotowane\n";
    dataLoad($branch . '.last.txt' , $branch . '.xml'); 
    // $LAST = array(); // Na razie testujemy co innego...
    echo ".last wczytany\n";
    flush();
    $xmlf = fopen($dataXML , 'w');
    $sections->toXML($xmlf);
    fclose($xmlf);
    echo ".XML zapisany\n";
    dataSave($branch . '.last.txt' , $branch . '.xml');
    echo ".dane zapisane\n"; 
    // od razu backup danych
    if ($sections->xmlRev) {
        dataSave($BACKUP_DIR . $branch . '.last.txt.' . $sections->revision , $BACKUP_DIR . $branch . '.xml.' . $sections->revision);
        echo ".backup zrobiony\n";
    } 
}
function makeSafeName($name) {
	 return preg_replace("/[^a-z0-9_.]/i", "_", $name);
}
// zwraca nazwê paczki...
function getPackFile($sectionPath , $pack , $file , $type)
{
    global $DATA_DIR;
    $s = makeSafeName($sectionPath) . '-' . makeSafeName($pack);
    $s .= '-' . makeSafeName($file); // Je¿eli nie ma par. file jest to ca³a paczka!
		if ($type == '') $type = 'plain';
    $s .= '.' . $type;
    return $DATA_DIR . $s;
} 
// pobiera wersjê pliku
function getVersion($file , $type = 'n')
{
    $s = trim(exec("get_version \"$file\" $type"));
    if (substr($s, 0, 4) == "Plik") $s = '';
    return $s;
} 

function isExeFile($file)
{
    $file = substr(strtolower($file), -4);
    return $file == '.dll' || $file == '.exe';
} 
// wstawia plik do $FILES
function filePrepare($file)
{
    global $FILES , $LAST;
    if (!array_key_exists($file , $FILES)) {
        $mtime = filemtime($file);
        /* Oszczêdzamy trochê czasu/pamiêci/procka */
        if (array_key_exists($file , $LAST) && ($mtime == $LAST[$file][0] || abs($mtime - $LAST[$file][0])==3600) ) {
            $FILES[$file] = $LAST[$file];
        } else {
            $FILES[$file] = array(filemtime($file) , isExeFile($file) ? getVersion($file):0);
        } 
    } 
} 
function getFileDate($file)
{
    global $FILES;
    filePrepare($file);
    return $FILES[$file][FILE_TIME];
} 
function getFileVersion($file)
{
    global $FILES;
    filePrepare($file);
    return $FILES[$file][FILE_VER];
} 
function getFileCRC($file)
{
    global $FILES;
    filePrepare($file);
    if (!@$FILES[$file][FILE_CRC]) {
        $FILES[$file][FILE_CRC] = crc32(file_get_contents($file));
    } 
    return $FILES[$file][FILE_CRC];
} 

class cBaseSection {
    var $dlUrl;
    var $name;
    var $title;
    var $info;
    var $sections = array();
    var $sectionPath = '';
    var $path = ''; // œcie¿ka w której zostanie rozpakowana paczka - def = '%KONNEKT%'
    function addSection(&$section)
    {
        $section->sectionPath = ($this->sectionPath?$this->sectionPath . '/':'') . $section->name;
        echo "\n[" . $section->sectionPath . "/" . $this->sectionPath . "/" . $section->name . "]\n";
        for ($i = 0; $i < count($section->packs); $i++) {
            $section->packs[$i]->section = $section;
        } 
        $this->sections[] = &$section;
        $this->sections[count($this->sections) - 1]->parent = &$this;
    } 
    function xmlOpenTag($xmlf)
    {
        fwrite($xmlf , "<section name=\"{$this->name}\" title=\"" . htmlspecialchars($this->title) . "\"");
        if ($this->dlUrl)
            fwrite($xmlf , " dlUrl=\"" . htmlspecialchars($this->dlUrl) . "\"");
        if ($this->path)
            fwrite($xmlf , " path=\"" . htmlspecialchars($this->path) . "\"");
        fwrite($xmlf , '>');
        if ($this->info)
            fwrite($xmlf , "<info>".htmlspecialchars($this->info)."</info>");
    } 
} 

class cRootSection extends cBaseSection {
    var $branch;
    var $revision;
    var $xmlBranch; // el. <branch> w xmlu
    var $xmlRev;
    var $xmlLog;
    var $xmlIgnore;
    function cRootSection($branch)
    {
        echo "\n Nowa ga³¹Ÿ - $branch \n";
        $this->revision = strftime('%y%m%d%H%M');
        $this->branch = $branch;
        $this->name = $branch;
    } 
    function toXML($xmlf)
    {
        global $DOC, $optIgnore, $optNoLog;
        /* trzeba przygotowaæ co nieco XML'a */
        $root = $DOC->document_element();
        if (!$root) $root = $DOC->append_child($DOC->create_element("kUpdate"));
        if (!($this->xmlBranch = XMLGetNode($root , "branch[@id='{$this->branch}']"))) {
            $this->xmlBranch = $root->append_child($DOC->create_element("branch"));
            $this->xmlBranch->set_attribute("id" , xE($this->branch));
        } 
        if (!($this->xmlLog = XMLGetNode($this->xmlBranch , "log"))) {
            $this->xmlLog = $this->xmlBranch->append_child($DOC->create_element("log"));
        } 

        if (($this->xmlIgnore = XMLGetNode($this->xmlBranch , "ignore")) && $optIgnore) {
            $this->xmlIgnore->unlink_node($this->xmlIgnore);
            $this->xmlIgnore = 0;
        } 
        if (!$this->xmlIgnore) {
            $this->xmlIgnore = $this->xmlBranch->append_child($DOC->create_element("ignore"));
        } 
        $this->xmlRev = XMLaddFirst($this->xmlLog , $DOC->create_element("revision"));
        $this->xmlRev->set_attribute("id" , xE($this->revision));
        $this->xmlRev->set_attribute("date" , strftime('%Y-%m-%d %H:%M:%S'));
        fwrite($xmlf , '<?xml version="1.0" encoding="windows-1250"?>');
        $date = strftime('%Y-%m-%d %H:%M:%S');
        fwrite($xmlf , "<update branch='{$this->branch}' revision='{$this->revision}' date='$date'><date>$date</date>");
        $this->xmlOpenTag($xmlf);
        for ($i = 0; $i < count($this->sections); $i++)
        $this->sections[$i]->toXML($xmlf);
        fwrite($xmlf , "</section></update>");
        /* Skoro nic siê nie zmieni³o, w ogóle nie zapisujemy rewizji! */
        if ($optNoLog || !$this->xmlRev->has_child_nodes()) {
            $this->xmlRev->unlink_node($this->xmlRev);
            $this->xmlRev = 0;
        } 
    } 
    function &getRoot()
    {
        return $this;
    } 
} 
class cSection extends cBaseSection {
    var $packs = array();
    var $parent;

    function cSection($name , $title , $info)
    {
        $this->name = $name;
        $this->title = $title;
        $this->info = $info;
    } 
    function getRoot()
    {
        return $this->parent->getRoot();
    } 

    function addPack(&$pack)
    {
        $pack->files = array_unique($pack->files);
        $pack->descriptive = array_unique($pack->descriptive); 
        // Rozpoznajemy czy single...
        $pack->single = preg_match('/zip|tar/' , $pack->type);
        $this->packs[] = $pack;
        $this->packs[count($this->packs) - 1]->parent = &$this;
    } 
    function toXML($xml)
    {
        $this->xmlOpenTag($xml);
        for ($i = 0; $i < count($this->sections); $i++)
        $this->sections[$i]->toXML($xml);
        for ($i = 0; $i < count($this->packs); $i++)
        $this->packs[$i]->toXML($xml);
        fwrite($xml , "</section>");
    } 
} 

class cPack {
    var $parent;
    var $name;
    var $hidden = false;
    var $required = false;
    var $depends = ''; // nazwy innych paczek, które s¹ wymagane oddzielane ŒREDNIKIEM
    var $title = '';
    var $info = '';
    var $url = '';
    var $path = ''; // œcie¿ka w której zostanie rozpakowana paczka - def = '%KONNEKT%'
    var $src = ''; // œcie¿ka do katalogu w którym teraz siedz¹ pliki...
    var $type = 'gz'; // Typ. Zbiorcze ($single=true): zip  Pojedyncze: gz , bz2
    var $single = false; // Czy paczka to jedn plik (zip/tar) czy pojedyncze gz/bz2
    var $dlURL = '';
    var $descriptive = array(); // Pliki opisuj¹ce paczkê
    var $files = array(); // Pliki do wrzucenia do paczki
    var $fileOpts = array(); // Dodatkowe opcje plików
    var $new = false; // true gdy coœ nowego znajdzie sie w tablicy...
    var $restart = false; // Czy wymaga restartu?	
    var $noinstall = false; // Paczka mo¿e byæ tylko AKTUALIZOWANA!
    var $system = false; // Czy paczka jest bibliotek¹ systemow¹?
    var $library = false;
    var $additional = false;
    var $recommended = false;
    var $searchPath = '';
    function cPack($name , $title , $src)
    {
        $this->name = $name;
        $this->title = $title;
        $this->src = $src;
        if ($src && substr($this->src , -1) != '\\') $this->src .= '\\';
    } 
    function getRoot()
    {
        return $this->parent->getRoot();
    } 
    function getPath()
    {
        return $this->parent->sectionPath . '/' . $this->name;
    } 
    function getPackFile($file)
    {
        return getPackFile($this->parent->sectionPath , $this->name , $file , $this->type);
    } 

    function addFilesToTable(&$table , $files , $rec , $curDir)
    {
        if (is_array($files) || $files{0} != '~') {
            if (!is_array($files)) $table[] = $curDir . $files;
            else {
                foreach ($files as $file) {
                    $table[] = $curDir . $file;
                } 
            } 
        } else {
            $dir = @opendir($curDir ? substr($this->src . $curDir , 0 , -1) : $this->src);
            if (!$dir) {
                die ("\nNie mog³em otworzyæ " . ($curDir ? substr($this->src . $curDir , 0 , -1) : $this->src));
            } 
            preg_match('/^\~([^\~]+)(?:\~(.+))?$/' , $files , $tab);
            $add = @$tab[1];
            $exclude = @$tab[2];
            while (($file = readdir($dir)) !== false) {
                if (is_dir($this->src . $curDir . $file)) {
                    if ($rec && $file != '.' && $file != '..')
                        $this->addFilesToTable($table , $files , true , $curDir . $file . '\\');
                } else {
                    $checkfile = str_replace('\\', '/', $curDir . $file);
                    if (!in_array($curDir . $file , $table) && preg_match('#^' . $add . '$#i' , $checkfile) && !preg_match('#^' . $exclude . '$#i' , $checkfile))
                        $table[] = $curDir . $file;
                } 
            } 
            closedir($dir);
        } 
    } 
    function setFileOptions($filename, $opts) {
        if (is_array($filename) || $filename{0} != '~') {
            if (!is_array($filename)) {
                if (!isset($this->fileOpts[$filename])) 
                    $this->fileOpts[$filename] = $opts;
                else 
                    $this->fileOpts[$filename] = array_merge($this->fileOpts[$filename] , $opts);
            } else {
                foreach ($filename as $file) {
                  $this->setFileOptions($file, $opts);
                } 
            } 
        } else {
            preg_match('/^\~([^\~]+)(?:\~(.+))?$/' , $filename , $tab);
            $add = @$tab[1];
            $exclude = @$tab[2];
            foreach ($this->files as $file) {
                if (preg_match('#^' . $add . '$#i' , $file) && !preg_match('#^' . $exclude . '$#i' , $file)) {
                    $this->setFileOptions($file, $opts);
                }
            }
        }
    }
    function addFiles($files , $rec = false , $dir = '')
    {
        $this->addFilesToTable($this->files , $files , $rec , $dir);
        if (!count($this->files)) {
            echo "\n-x $dir  /  $files ";
        } 
    } 
    function addDesc($files , $rec = false , $dir = '')
    {
        $this->addFilesToTable($this->descriptive , $files , $rec , $dir); 
        // pliki opisowe MUSZ¥ znajdywaæ siê w paczce!
        $this->addFiles($files , $rec , $dir);
    } 
    function toXML($xml)
    {
        global $FILES , $XML_CHARSET;
        if (!count($this->files)) return;
        $this->makePack();
        fwrite($xml , "<pack name=\"" . htmlspecialchars($this->name) . "\" title=\"" . htmlspecialchars($this->title) . "\""
             . " url=\"" . htmlspecialchars($this->url) . "\"");
        if ($this->depends) fwrite($xml , " depends=\"{$this->depends}\"");
        if ($this->path) fwrite($xml , " path=\"{$this->path}\"");
        if ($this->hidden) fwrite($xml , " hidden=\"1\"");
        if ($this->required) fwrite($xml , " required=\"1\"");
        if ($this->restart) fwrite($xml , " restart=\"1\"");
        if ($this->noinstall) fwrite($xml , " noinstall=\"1\"");
        if ($this->system) fwrite($xml , " system=\"1\"");
        if ($this->library) fwrite($xml , " library=\"1\"");
        if ($this->recommended) fwrite($xml , " recommended=\"1\"");
        if ($this->additional) fwrite($xml , " additional=\"1\"");
        if ($this->dlURL) fwrite($xml , " required=\"{$this->dlURL}\"");
        if ($this->searchPath) fwrite($xml , " searchpath=\"{$this->searchPath}\"");
        fwrite($xml , " type=\"{$this->type}\"");
        if ($this->single) {
            fwrite($xml , " single=\"1\"");
            $packFile = $this->getPackFile('');
            fwrite($xml , " size=\"" . @filesize($packFile) . "\"");
            fwrite($xml , sprintf(' crc32="%u"' , @getFileCRC($packFile)));
        } 
        if ($this->new) fwrite($xml , " new=\"1\""); 
        // fwrite($xml , " packURL=\"".PACK_URL , $this->name)."\"");
        fwrite($xml , ">");
        /* <pack> jest otwarty */
        if ($this->info)
            fwrite($xml , "<info>" . htmlspecialchars($this->info) . "</info>");
        $files = ($this->single && count($this->descriptive)) ? $this->descriptive : $this->files; 
        // Wpisujemy do XMLa wszystkie pliki nale¿¹ce do paczki lub j¹ opisuj¹ce...
        foreach ($files as $file) {
            filePrepare($this->src . $file); // Raczej sie nie przyda ale co tam...
            fwrite($xml , "<file name=\"{$file}\" date=\"" . $FILES[$this->src . $file][0] . "\"");
            if ($FILES[$this->src . $file][1]) fwrite($xml , ' version="' . $FILES[$this->src . $file][1] . '"');
            if (!$this->single) {
                // path i dlURL na razie sobie darujmy...
                $packFile = $this->getPackFile($file);
                fwrite($xml , " size=\"" . @filesize($packFile) . "\"");
                fwrite($xml , sprintf(' crc32="%u"' , @getFileCRC($packFile)));
            }
            if (isset($this->fileOpts[$file]) && is_array($this->fileOpts[$file])) {
                foreach ($this->fileOpts[$file] as $name=>$value) {
                  fwrite($xml , " $name=\"" . htmlspecialchars($value) . "\"");
                }
            } 
            fwrite($xml , "/>");
        } 
        // changelog - wstawiamy wszystko co siedzi w logu dla tej paczki...
        $root = $this->getRoot();
//        $revs = XMLGetNodes($root->xmlLog , "revision/pack[@id='" . $this->getPath() . "' and @inc!='no']/info");
        $revs = XMLGetNodes($root->xmlLog , "revision/pack[@id='" . $this->getPath() . "' and string(@inc)!='no']/info");
//        echo "\n---------[[" . $this->getPath() . "]]--------\n";
//        var_dump($revs);
//        die();
        if ($revs) {
            foreach ($revs->nodeset as $rev) {
                $node = $rev->parent_node();
                $revNode = $node->parent_node();
                fwrite($xml , "<rev id='" . xD($revNode->get_attribute('id') , $XML_CHARSET) . "' time='" . $node->get_attribute('time') . "'>" . htmlspecialchars(xD($rev->get_content() , $XML_CHARSET)) . "</rev>");
            } 
        } 
        fwrite($xml , "</pack>");
    } 
    /* Menu do decydowania o znalezionych zmianach... */
    function confirmDialog(&$newFiles , $newest)
    {
        global $optRebuild, $optAuto;
        /* skoro jest 'rebuild' tylko generujemy XMLa */
        if ($optRebuild)
            return false;

        global $DOC;
        $root = $this->getRoot();
        $info = '';
        $ret = false; 
        // Pomijamy...
        if (XMLGetNode($root->xmlIgnore , "pack[@id='" . $this->getPath() . "' and @time='{$newest}']")) {
            echo "\n --- Pomijam " . $this->getPath() . "\n";
            $ret = false;
        } else {
	        echo "\n +++ Wykry³em zmiany w '" . $this->getPath() . "' w nast. plikach:  ";
	        echo implode('; ' , $newFiles);
	        echo ".\n";
	        if ($optAuto) {
	            echo " +++ Auto\n";
	            $this->new = true;
	            $ret = true;
	        } else {
	            echo "\n +++ Podaj opis zmian (XHTML) [2xEnter na koniec] / [Enter] - bez opisu\n";
	            echo " +++ [dowolny 1 znak,Enter] pomiñ plik / [Ctrl+C] przerwij operacjê...\n";
	            flush();
	
	            $in = fopen("php://stdin", "rb");
	            fflush($in);
	            $ret = false;
	            $c = fgets($in); // fread($in , 1);
	            if (!$c || ord($c) < 10 || strlen($c) - 2 == 1) {
	                $this->new = false;
	                $ret = false;
	                echo " +++ Pominiêty +++\n"; 
	                // Zapisujemy na liœcie ignorowanych
	                if (!($pack = XMLGetNode($root->xmlIgnore , "pack[@id='" . $this->getPath() . "']"))) {
	                    $pack = $root->xmlIgnore->append_child($DOC->create_element("pack"));
	                    $pack->set_attribute("id" , $this->getPath());
	                } 
	                $pack->set_attribute("time" , $newest);
	            } else { // opis
	                $this->new = true;
	                $ret = true;
	                if (trim($c)) {
	                    $info = trim($c , "\r\n");
	                    while (($line = trim(fgets($in) , "\r\n"))) {
	                        $info .= "\n" . $line;
	                    } 
	                } 
	                // $info = trim($info);
	                /* zamiana kodowania znaków konsoli... */ 
	                // $info = strtr($info ,  "¥†©ˆä¢˜¾«¤¨ãà—½" , "¹æê³ñóœ¿Ÿ¥ÆÊ£ÑÓŒ¯");
	            } 
	            fclose($in);
	        } // auto
		} // XMLIGNORE
        if ($ret) {
            if (!($pack = XMLGetNode($root->xmlRev , "pack[@id='" . $this->getPath() . "']"))) {
                $pack = $root->xmlRev->append_child($DOC->create_element("pack"));
                $pack->set_attribute("id" , $this->getPath());
            } 
            $pack->set_attribute("time" , $newest);
            if ($info) {
                echo " +++ Do³¹czony z opisem '$info' +++\n";
                $node = $pack->append_child($DOC->create_element("info"));
                $node->append_child($DOC->create_text_node(xE($info)));
            } else {
                echo " +++ Do³¹czony bez opisu +++\n";
            } 
            $changed = $pack->append_child($DOC->create_element("changed"));
            $changed->append_child($DOC->create_text_node(implode('; ' , $newFiles)));
        } else {
            global $FILES , $LAST; 
            // Je¿eli pomijamy, trzeba "cofn¹æ" siê w tablicach z czasami plików
            foreach($newFiles as $file) {
	            if (array_key_exists($this->src . $file , $LAST))
	                $FILES[$this->src . $file] = $LAST[$this->src . $file];
			}
        } 

        echo "\n";
        // $ret = false; // udajemy Cancel ¿eby nie pakowaæ ...
        return $ret;
    } 
    function makePack()
    {
        global $FILES , $LAST; 
        // Robimy listê dla ZIPa albo bezpoœrednio pakujemy...
        // Sprawdzamy przy okazji czy w ogóle jest sens cokolwiek robiæ...
        // Je¿eli zip docelowy nie istnieje, uznajemy, ¿e wszystko jest nowe!
        if ($this->single && !file_exists($this->getPackFile(''))) {
            $this->new = true;
            echo "\n ++  Paczka '" . $this->getPackFile('') . "' jest nowa! \n";
        } 

        $fileList = '';
        $newFiles = array(); // pliki wymagaj¹ce aktualizacji
        $newest = 0;
        foreach ($this->files as $file) {
            filePrepare($this->src . $file);
            if (($missing = (!$this->single && !file_exists($this->getPackFile($file))))) {
                echo "\n ++  Paczka '" . $this->getPackFile($file) . "' jest nowa! \n";
            } 
			if ( abs($FILES[$this->src . $file][0] - @$LAST[$this->src . $file][0]) == 3600)
				$FILES[$this->src . $file][0] = @$LAST[$this->src . $file][0];
            if ($this->new || $FILES[$this->src . $file][0] > @$LAST[$this->src . $file][0] || $missing) {
                if ($FILES[$this->src . $file][0] > $newest)
                    $newest = $FILES[$this->src . $file][0];
                array_push($newFiles , $file);
            } 
            if ($this->single)
                $fileList .= ' "' . $file . '"';
        } 

        if (count($newFiles)) { // pojawi³o siê coœ nowego, trzeba spytaæ co z tym fantem zrobiæ...
            if (!$this->confirmDialog($newFiles , $newest))
                return;
        } 
        if (!$this->new)
            return; // anulowane, lub nie ma co pakowaæ... 
        // Pakujemy...
        echo " +++ > Pakujê " . strtoupper($this->type) . "\n";
        if (!$this->single) { // gzip/bzip2
            foreach ($newFiles as $file) {
                $dest = $this->getPackFile($file);
                $f = fopen($this->src . $file , "rb");
                $data = fread($f , filesize($this->src . $file));
                fclose($f);
                echo "   -> $file to $dest\n";
                if ($this->type == 'gz') {
                    $c = gzopen($dest , 'w9');
                    gzwrite($c , $data);
                    gzclose($c);
                } else if ($this->type == 'bz2') {
                    $c = bzopen($dest , 'w9');
                    bzwrite($c , $data);
                    bzclose($c);
                } else if ($this->type == '') {
                    $c = fopen($dest , 'wb');
                    fwrite($c , $data);
                    fclose($c);
                } 
            } 
            // touch($dest , filemtime($this->src.$file));
        } else { // ZIP
            $dest = $this->getPackFile('');
            $dest = realpath(dirname($dest)) . '/' . basename($dest);
            if (!$dest) die(' Œcie¿ka docelowa nie istnieje! ');
            $oldDir = getcwd();
            chdir($this->src);
            @unlink($dest);
            system("zip -9 \"$dest\"{$fileList}");
            chdir($oldDir);
        } 
    } 
} 

?>
