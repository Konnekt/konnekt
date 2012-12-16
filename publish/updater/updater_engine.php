<?
/*
  Generuje XMLa i paczki dla update.dll
*/
set_time_limit(0); 

function getPackFile($sectionPath , $pack , $file , $type) {
  $s = str_replace('/','_',$sectionPath).'-'.$pack;
	if ($file) $s.= '-'.preg_replace('#[\\\\/]#','_',$file); // Je¿eli nie ma par. file jest to ca³a paczka!
	$s.='.'.$type;
  return DATA_DIR.$s;
}

function getVersion($file , $type = 'n') {
  $s = trim(exec("get_version \"$file\" $type"));
	if (substr($s,0,4) == "Plik") $s = '';
  return $s;
}

$FILES = array();

function isExeFile($file) {
  $file = substr(strtolower($file),-4);
  return $file=='.dll' || $file=='.exe';
}
function filePrepare($file) {
  global $FILES;
	if (!array_key_exists($file , $FILES)) {
	  $FILES[$file]=array(filemtime($file) , isExeFile($file)?getVersion($file):0);
	}
}
function getFileDate($file) {
  global $FILES;
  filePrepare($file);
	return $FILES[$file][0]; 
}
function getFileVersion($file) {
  global $FILES;
  filePrepare($file);
	return $FILES[$file][1]; 
}

class cBaseSection {
	var $sections = array();
	var $sectionPath = '';
	function addSection(&$section) {
	  $section->sectionPath = ($this->sectionPath?$this->sectionPath.'/':'').$section->name;
    echo "\n[".$section->sectionPath."/".$this->sectionPath."/".$section->name."]\n";
		for ($i = 0; $i < count($section->packs); $i++) {
		  $section->packs[$i]->section = $section;
		}
		
	  $this->sections[] = $section;
	}
  function toXML($xml) {
	  fwrite($xml , '<?xml version="1.0" encoding="windows-1250"?>'."\n");
		fwrite($xml , "<update><date>".strftime('%Y-%m-%d %H:%M')."</date><section name=\"ROOT\" dlURL=\"".htmlspecialchars(DL_URL)."\">\n");
		for ($i = 0; $i < count($this->sections); $i++)
		  $this->sections[$i]->toXML($xml);
		fwrite($xml , "</section></update>\n");
	}
}
class cSection extends cBaseSection{
  var $name;
	var $title;
	var $info;
	var $packs = array();
	
	function cSection($name , $title , $info) {
	  $this->name = $name;
	  $this->title = $title;
	  $this->info = $info;
	}
	
	function addPack(&$pack) {
	  $pack->files = array_unique($pack->files);
	  $pack->descriptive = array_unique($pack->descriptive);
		// Rozpoznajemy czy single...
		$pack->single = preg_match('/zip|tar/' , $pack->type);
	//	$pack->section = &$this;
	  $this->packs[] = $pack;
	}
  function toXML($xml) {
		fwrite($xml , "  <section name=\"{$this->name}\" title=\"".htmlspecialchars($this->title)."\">");
		if ($this->info)
  		fwrite($xml , "<info>{$this->info}</info>");
		for ($i = 0; $i < count($this->sections); $i++)
		  $this->sections[$i]->toXML($xml);
		for ($i = 0; $i < count($this->packs); $i++)
		  $this->packs[$i]->toXML($xml);
		fwrite($xml , "\n  </section>\n");
	}

};

class cPack {
  var $section;
  var $name;
	var $hidden=false;
	var $required=false;
	var $depends=''; // nazwy innych paczek, które s¹ wymagane oddzielane przecinkiem
	var $title = '';
	var $info = '';
	var $url = '';
	var $path = ''; // œcie¿ka w której zostanie rozpakowana paczka - def = '%KONNEKT%'
	var $src = ''; // œcie¿ka do katalogu w którym teraz siedz¹ pliki...
	var $type = 'gz'; // Typ. Zbiorcze: zip  Pojedyncze: gz , bz2
	var $single = false; // Czy paczka to jedn plik (zip/tar) czy pojedyncze gz/bz2
	var $dlURL = '';   
	var $descriptive = array(); // Pliki opisuj¹ce paczkê
  var $files = array(); // Pliki do wrzucenia do paczki
	var $new = false; // true gdy coœ nowego znajdzie sie w tablicy...
  var $restart = false; // Czy wymaga restartu?	
	function cPack($name , $title , $src) {
	  $this->name = $name;
		$this->title = $title;
		$this->src = $src;
		if ($src && substr($this->src , -1) != '\\') $this->src.='\\';
	}
	function addFilesToTable(&$table , $files , $rec , $curDir) {
	  if (is_array($files) || $files{0}!='~') {
		  if (!is_array($files)) $table[]=$curDir.$files;
			else {
			  foreach ($files as $file) {
					$table[]=$curDir.$file;
				}
			}
		} else {
		  $dir = @opendir($curDir ? substr($this->src.$curDir , 0 , -1) : $this->src);
      if (!$dir) {die ("\nNie mog³em otworzyæ ".($curDir ? substr($this->src.$curDir , 0 , -1) : $this->src));}
			preg_match('/^\~([^\~]+)(?:\~(.+))?$/' , $files , $tab);
			$add = @$tab[1];
			$exclude = @$tab[2];
			while (($file = readdir($dir))!==false) {
			  if (is_dir($this->src.$curDir.$file)) {
				  if ($rec && $file!='.' && $file!='..')
  				  $this->addFilesToTable($table , $files , true , $curDir.$file.'\\');
				} else {
				  $checkfile = str_replace('\\','/',$curDir.$file);
  			  if (preg_match('#^'.$add.'$#i' , $checkfile) && !preg_match('#^'.$exclude.'$#i' , $checkfile))
					  $table[]=$curDir.$file;
				}
			}
			closedir($dir);
		}
	}
	function addFiles($files , $rec=false , $dir='') {
	  $this->addFilesToTable($this->files , $files , $rec , $dir);
    if (!count($this->files)) {echo "\n-x $dir  /  $files ";}
	}
	function addDesc($files , $rec=false , $dir='') {
	  $this->addFilesToTable($this->descriptive , $files , $rec , $dir);
	}
  function toXML($xml) {
	  global $FILES;
		if (!count($this->files)) return;
	  $this->makePack();
		fwrite($xml , "\n    <pack name=\"{$this->name}\" title=\"".htmlspecialchars($this->title)."\""
		  ." url=\"".htmlspecialchars($this->url)."\"");
		if ($this->depends) fwrite($xml , " depends=\"{$this->depends}\"");	
		if ($this->path) fwrite($xml , " path=\"{$this->path}\"");	
		if ($this->hidden) fwrite($xml , " hidden=\"1\"");	
		if ($this->required) fwrite($xml , " required=\"1\"");	
		if ($this->restart) fwrite($xml , " restart=\"1\"");	
		if ($this->dlURL) fwrite($xml , " required=\"{$this->dlURL}\"");	
		fwrite($xml , " type=\"{$this->type}\"");	
		if ($this->single) fwrite($xml , " single=\"1\"");	
		if ($this->single) fwrite($xml , " size=\"".@filesize(getPackFile($this->section->sectionPath , $this->name , '' , $this->type))."\"");	
		if ($this->new) fwrite($xml , " new=\"1\"");	
//		fwrite($xml , " packURL=\"".PACK_URL , $this->name)."\"");
		fwrite($xml , ">");
		if ($this->info)
  		fwrite($xml , "<info>{$this->info}</info>");
    $files = ($this->single && count($this->descriptive)) ? $this->descriptive : $this->files;
		// Wpisujemy do XMLa wszystkie pliki nale¿¹ce do paczki lub j¹ opisuj¹ce...
		foreach ($files as $file) {
		  filePrepare($this->src.$file); // Raczej sie nie przyda ale co tam...
			fwrite($xml , "\n     <file name=\"{$file}\" date=\"".$FILES[$this->src.$file][0]."\"");
			if ($FILES[$this->src.$file][1]) fwrite($xml , ' version="'.$FILES[$this->src.$file][1].'"');
			if (!$this->single) {
			  // path i dlURL na razie sobie darujmy...
				fwrite($xml , " size=\"".@filesize(getPackFile($this->section->sectionPath , $this->name , $file , $this->type))."\"");
			}
  		fwrite($xml , "/>");
		} 	
		fwrite($xml , "\n    </pack>");
	}
	function makePack() {
	  global $FILES , $LAST;
		// Robimy listê dla ZIPa albo bezpoœrednio pakujemy...
		// Sprawdzamy przy okazji czy w ogóle jest sens cokolwiek robiæ...
		foreach ($this->files as $file) {
		  filePrepare($this->src.$file);
 		  if (!$this->single) $dest = getPackFile($this->section->sectionPath , $this->name , $file , $this->type);
			if ($FILES[$this->src.$file][0] > @$LAST[$this->src.$file][0] || (!$this->single && !file_exists($dest))) {
			  $this->new = true;
				if (!$this->single) { // gzip/bzip2
				  $f = fopen($this->src.$file , "rb");
					$data = fread($f , filesize($this->src.$file));
					fclose($f);
					echo "   -> $file to $dest\n";
				  if ($this->type=='gz') {
					  $c = gzopen($dest , 'w9');
						gzwrite($c , $data);
						gzclose($c);
					} else if ($this->type=='bz2') {
					  $c = bzopen($dest , 'w9');
						bzwrite($c , $data);
						bzclose($c);
					} else if ($this->type=='') {
					  $c = fopen($dest , 'wb');
						fwrite($c , $data);
						fclose($c);
					}
              //touch($dest , filemtime($this->src.$file));  
				}
			}
			if ($this->single)
			  $fileList .= ' "'.$file.'"'; 
		}
		if ($this->single) $dest = /*getcwd().'\\'.*/  realpath(getPackFile($this->section->sectionPath , $this->name , '' , $this->type));
		if ($this->new || ($this->single && !file_exists($dest))) {
		  // ZIP
		  echo " -> {$this->name} is NEW\n";
			if ($this->single) {
				echo "   -> {$this->name} to $dest\n";
    		$oldDir = getcwd();
    		chdir($this->src);
				@unlink($dest);
				system("zip -9 \"$dest\"{$fileList}");
    		chdir($oldDir);
			}
		}
	}

};

?>
