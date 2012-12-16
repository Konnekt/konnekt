<?
set_time_limit(0); 
ob_end_clean();
echo "\nKonnekt - update prepare\n\n";
define("CUR_DIR" , ($argc>1)?$argv[1]."\\" : "___current\\"); // aktualna wersja
define("NEW_DIR" , "");
define("UPD_DIR" , "___update\\");
define("NSIS_DIR" , "..\\..\\__tools\\nsis\makensis.exe");
// Czyscimy update dir...
echo " -> Czyszcze katalog UPD\n";
exec("rmdir ".UPD_DIR." /q /s");
exec("mkdir ".UPD_DIR);
$delFile=array(); // Pliki ktore sa do usuniecia, z tej tablicy powstanie skrypt...
$delDir =array(); // Katalogi ktore sa do usuniecia, z tej tablicy powstanie skrypt...
$noSection = array();

function compareFiles($f1 , $f2) {
  if (!file_exists($f2)) return 1;
  if (filemtime($f1) != filemtime($f2)) return 1;
  if (filesize($f1) != filesize($f2)) return 1;
}

// Porownuje katalogi NEW i CUR i to co sie rozni wrzuca do UPD 
function UCnewFiles($dirName) {
  if (!is_dir(CUR_DIR."$dirName")) 
     system("mkdir ".UPD_DIR."$dirName"); 
  $path = NEW_DIR.$dirName;
  if ($dir = opendir($path)) {
    while (($file = readdir($dir)) !== false) {
      if ($file != "." && $file != "..") {
         if (is_dir($path."\\$file")) {
             UCnewFiles($dirName."\\$file");
         } else {
           if (compareFiles($path."\\$file" , CUR_DIR."$dirName\\$file")) {
             echo("\n    > Znalazlem nowy - $dirName\\$file");
             if (!is_dir(UPD_DIR."$dirName")) system("mkdir ".UPD_DIR."$dirName");
             exec("copy \"$path\\$file\" \"".UPD_DIR."$dirName\\$file\"");
           }
         }  
      }  
    }
    closedir($dir);
  }  else {die ("Nie mog³em otworzyæ katalogu $dirName!");}
} 

function updCompare($dirName) {
  global $noSection;
  echo "\n -> UCnewFiles $dirName";
  UCnewFiles($dirName);
  if (!is_dir(UPD_DIR.$dirName)) { // Nie ma ¿adnych zmian
    $noSection[$dirName] = true;
  }
}
$prevVer = "";
$s = join("\n" , file(CUR_DIR."konnekt.nsi"));
preg_match('/!define *MUI_VERSION *"([0-9.]+)"/' , $s , $tab);
unset($s);
$prevVer = $tab[1];
echo "\n -> update from $prevVer";

updCompare('__main');
$path = ".\\";
if ($dir = opendir($path)) {
    while (($file = readdir($dir)) !== false) {
      if (preg_match('/^_[a-z]/' , $file)) {
        updCompare($file);
      }  
    }
    closedir($dir);
  }

echo "\n\n ---------------------------------------- \n Building update ... \n";
$nsCmd = "";
foreach ($noSection as $key=>$value) 
  if ($value) $nsCmd.=" /Dno$key";
passthru (NSIS_DIR." /Dupdate=$prevVer /DnoLib /DoutDir=../_www/download/ /DbaseDir=".substr(UPD_DIR , 0 , -1)."$nsCmd konnekt.nsi");  

?>
