<?

include('cmdline.php');

//$optOld = getCmdVar('-old');

$optOldDL = getCmdOption('-oldDL');
$dataSuffix = ($optOldDL ? '_1' : '');

if ($optOldDL) {
  echo "\n#Stare URLe#\n";
}

if (@$optOld) {
//  $optRebuild = true;
}

$DATA_DIR = 's:/home/konnekt/public_html/update/data/';

$UPDATE_URL = "";
//$UPDATE_URL = "http://test.konnekt.info/update/";

?>