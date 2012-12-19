<?



function getCmdOption($name , $getVar = false , $def = '')
{
    for ($i = 1; $i < @$_SERVER['argc']; $i++) {
        $tab = array();
        if (preg_match('#^' . $name . '(?:=(.*))?$#i' , $_SERVER['argv'][$i] , $tab)) {
            if ($getVar)
                return $tab[1] ? $tab[1] : $def;
            else
                return true;
        } 
    } 
    return $getVar ? $def : false;
} 


function getCmdFile($index = 0, $sep = '-') {
    for ($i = 1; $i < @$_SERVER['argc']; $i++) {
    	if (substr($_SERVER['argv'][$i], 0, 1) != $sep) {
    		if ($index-- == 0) 
    			return $_SERVER['argv'][$i];
    	}
    	
    }
    return "";
}


function getInput() {
	$in = fopen("php://stdin", "rb");
	fflush($in);
	$s = trim(fgets($in));
	fclose($in); 
	return $s;
}

function confirmInput($str, $case=true) {
  while (($in = trim(getInput()))=='') {
	}
	if ($case == false) {
		$in = str_tolower($in);
		$str = str_tolower($str);
	} 
	return $in == $str;
}

?>