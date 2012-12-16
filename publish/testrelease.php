<?

include('testsuite.php');  

$src = './';
$dest = 'c:\\konnekt\\';

echo "\n\nC:\\Konnekt\n\n";

$verbose = 2;


doCopy(array('actio.dll') , $src . '_actio/plugins/' , $dest . 'plugins/' , false);
doCopy(array('sipxtapi.dll') , $src . '_actio/data/dll/' , $dest . 'data/dll/' , false);


?>
