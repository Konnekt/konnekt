<?

$src = "";
$dest = "___trunk/";
$verbose = 1;
include('adv_copy.php');
set_time_limit(0); 


function doCopyAll($dir) {
  global $dest;
	echo "\n[$dir :: ";
  $copyAll = array('.+'=>'');
	doCopy($copyAll , $dir.'/' , $dest.$dir);
}


doCopyAll('__main');

doCopyAll('_sms');
doCopyAll('_kjabber');
doCopyAll('_gg');
doCopyAll('_actio');



?>
