<?
echo "\nKonnekt - pack\n\n";
define("NSIS_DIR" , "\"c:\\program files (x86)\\nsis\\makensis.exe\"");
// -------------- to wywal

// pobiera wersjà pliku
function getVersion($file , $type = 'n')
{
    $s = trim(exec("get_version \"$file\" $type"));
    if (substr($s, 0, 4) == "Plik") $s = '';
    return $s;
} 

$VERSION = getVersion("__main\\konnekt.exe", 't');
$OUTDIR = "s:/home/konnekt/public_html/download/";
$BASEDIR = dirname(__FILE__);
// --------------


//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DoutName=full_ /DPRODUCT_VERSION=".$VERSION." konnekt.nsi");  

passthru (NSIS_DIR." /DoutDir=$OUTDIR /DnoExtra /DbaseDir=$BASEDIR\\___trunk /DPRODUCT_VERSION=".$VERSION." ___trunk\\konnekt.nsi");  
//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DnoExtra /DActio /DbaseDir=$BASEDIR\\___trunk /DPRODUCT_VERSION=".$VERSION." ___trunk\\konnekt.nsi");  
//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DoutName=actio_full_ /DnoExtra /DActio /DbaseDir=$BASEDIR\\___trunk /DPRODUCT_VERSION=".$VERSION." ___trunk\\konnekt.nsi");  
//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DoutName=actio_ /DnoPlugs /DnoExtra /DActio  /DbaseDir=$BASEDIR\\___trunk /DPRODUCT_VERSION=".$VERSION." ___trunk\\konnekt.nsi");  
//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DoutName=mini_ /DnoPlugs /DnoExtra /DbaseDir=$BASEDIR\\___trunk /DPRODUCT_VERSION=".$VERSION." ___trunk\\konnekt.nsi ");  

//passthru (NSIS_DIR." /DoutDir=$OUTDIR /DActio /DnoExtra /DoutName=actio_ /DPRODUCT_VERSION=".$VERSION." konnekt.nsi ");  

?>
