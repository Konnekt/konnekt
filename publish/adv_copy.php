<?
function advCopy($src , $dest , $mask , $recursive=true) {
  global $verbose;
  if (strrpos($src , "*")!==false || strrpos($src,"?")!==false) {
    $mask = dirname($src) .'/'. preg_replace(array('/\\./','/\\*/','/\\?/') , array('\\.','.*','.') , basename($src)); 
    $src = dirname($src);
    $dest = dirname($dest);
    $recursive = false;
  }
  if (is_dir($src)) {
    @mkdir($dest);
    if ($dir = @opendir($src)) {
     if ($verbose==2) {echo "$src -> $dest\n";} else if ($verbose==1) {echo ",";}
     while (($file = readdir($dir)) !== false) {
       if ((!is_dir($src.'/'.$file) || $recursive) && $file != '.' && $file != '..') {
         advCopy($src.'/'.$file , $dest.'/'.$file , $mask , $recursive);
       }
     }  
     closedir($dir);
    }
  } else if (is_file($src)) {
    if (!$mask || preg_match('#'.$mask.'#i' , $src)) {
	  if (file_exists($dest) && filemtime($dest) >= filemtime($src)) {
          if ($verbose==2) {echo "$src -> $dest || $mask - N\n";} else if ($verbose==1) {echo ".";}
	  } else {
          @mkdir(dirname($dest));
          copy($src , $dest);
          touch($dest , filemtime($src));
          if ($verbose==2) {echo "$src -> $dest || $mask\n";} else if ($verbose==1) {echo "^";}
	  }
    }
  }
}

function doCopy($copy,$src , $dest , $recursive = true) {
  $cmd = "echo Copying";
  foreach($copy as $key=>$file) {
    advCopy($src.$file , $dest.$file , is_string($key)?$key : '' , $recursive = true);
    //$cmd.="& copy /Y {$src}{$file} $dest{$file}";
  }
//  system($cmd);
}

?>
