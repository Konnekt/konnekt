<?

$src = "";
$dest = "test/";
$verbose = 1;
include('adv_copy.php');
set_time_limit(0); 

$src = "__main/";
echo ("\n$src\n");
$copy = array('.+'=>'');
doCopy($copy , $src , $dest);


if ($dir = opendir("./")) {
    while (($file = readdir($dir)) !== false) {
      if (preg_match('/^_[a-z]+$/i' , $file)) {
        $src = $file.'/';
        echo ("\n$src\n");
        $copy = array('.+'=>'');
        doCopy($copy , $src , $dest);
      }  
    }
    closedir($dir);
  }
  



?>
