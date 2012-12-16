<?php
/*
 * XMLfunctionsLIBrary   beta
 *  (c)Copyright 2002-2003  Rafa³ Lindemann | Stamina
 *  http://www.stamina.eu.org/
 *  this script is in early beta . No comments or optimizations were written . wait for final :)

 * written to make reading/writing XML easier (few lines , few to worry about)
 
 *  please , notify me about ANY changes made to this script and alter $*_cp & $*_ver
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */  
 global $XML_sig , $XML_cp , $XML_ver , $XML_decode;
 // use only documented functions ...
 
$XML_sig="<a href='http://www.stamina.eu.org/projekty/php/xmllib.php' target='_blank'>xmlLib</a>";
$XML_cp="&copy; 2002-04 hao|stamina";
$XML_ver="0.3.5";

// Charset to decode to from UTF-8 (func xE & xD) . Used EVERYWHERE
// when reading/writing XML node-data
$XML_decode="ISO-8859-2";

//define("XMLLIB_DOMXML", extension_loaded('domxml'));
define("XMLLIB_DOMXML", 1);
define("XMLLIB_DOM", extension_loaded('dom'));
if (XMLLIB_DOM) {
	require_once(dirname(__FILE__) . '/domxml-php4-to-php5.php');

	//require_once(dirname(__FILE__) . '/xmllib_domxmlemu.php');
}

// Class returned by some functions
// use XMLResult->node to access node object directly
// use XMLResult->get/set_attribute to use them automatically with Coding

class XMLResult {
  var $tagname , $content , $type , $node;
  function XMLResult ($n) {
    $this->type=XMLNodeType($n);
    $this->tagname=Xd($this->type==1?$n->tagname:"");
    $this->content=XMLGetContent($n);
    $this->node=&$n;
  }
  function get_content() {
//  	if (!this->content
  	return XMLGetContent($this->node);
  }
  function get_attribute($n) {
  	if (XMLLIB_DOMXML) {
	    return xD($this->node->get_attribute($n));
  	} else {
	    return xD($this->node->getAttribute($n));
  	}
  }
  function set_attribute($n , $v) {
  	if (XMLLIB_DOMXML) {
    	$this->node->set_attribute($n , xE($v));
  	} else {
    	$this->node->setAttribute($n , xE($v));
  	}
  }
  function has_attribute($n) {
  	if (XMLLIB_DOMXML) {
	    return $this->node->has_attribute($n);
  	} else {
	    return $this->node->hasAttribute($n);
  	}
  }
  
}

// Decodes from UTF-8 to $XML_decode ... Used when READING
function xD ($s , $charset = false) {
	global $XML_decode;
	if (!@$XML_decode && !$charset) return $s;
	return iconv("UTF-8",($charset ? $charset : $XML_decode),$s);
}

// Decodes from $XML_decode to UTF-8 ... Used when WRITING
function xE ($s , $charset = false) {
	global $XML_decode;
	if (!@$XML_decode && !$charset) return $s;
	return iconv(($charset ? $charset : $XML_decode),"UTF-8",$s);
}

//  Counts md5 signature of node
//	$path & $d are used by XMLed
function XMLmd5(&$node , $path='' , $d=0) {
  return md5($path.$node->tagname.count($node->child_nodes()).XMLGetContent($node).($d?$d->name.$d->location:"..."));
}

// do wywalenia!
function XMLfind(&$xml,$name) {
  $children=$xml->children();
  for ($i=0;$i<count($children);$i++) {

    if (XMLNodeType($children[$i])==1 && $children[$i]->tagname==$name) {return $children[$i];}

  }
  return 0;
}

// do wywalenia w sumie ...
function XMLattributes(&$node,$attribs) {
  if ($node->type!=1) {return;}
  //$ret=new Array();
  for ($i=0;$i<count($attribs);$i++) {$ret[$attribs[$i]]='';}
  $temp=$node->attributes();
  if (!$temp) {return;}
  foreach ($temp as $t) {
    $ret[xD($t->name)]=xD($t->value);
  }
  return $ret;
}

function XML2HTMLattrib(&$node) {
  $r="";
    if (!$temp=$node->attributes()) {return "";}
  foreach ($temp as $t) {
    $r.=$t->name."=\"".$t->value."\" ";
  }
  return Xd($r);
}

function XML2HTML(&$node , $closer="", $txt="") {
  if ($node->type!=1) {return;}
  $child=$node->children();
  for ($i=0; $i<count($child); $i++) {
    if (XMLNodeType($child[$i])==3 || XMLNodeType($child[$i])==4) {$txt.=$child[$i]->content;}
    else if (XMLNodeType($child[$i])==1) {
      if (!$child[$i]->children()) {$txt.= "<".$child[$i]->tagname." ".XML2HTMLattrib($child[$i]).">";}
      else {$txt.= "<".$child[$i]->tagname." ".XML2HTMLattrib($child[$i])."$closer>";  
      $txt=XML2HTML($child[$i] , $closer , $txt);
      $txt.=   "</".$child[$i]->tagname.">";
      }
    }
  }
  return Xd($txt);
}


//  Returns all contents of node (all childrens of type-text concatenated with $sep)
// 
function XMLGetContent(&$Pnode , $sep="") {
  if ($Pnode->type!=1 || !$Pnode->has_child_nodes()) {return "";}
  $child=$Pnode->child_nodes();
  $txt="";
  foreach ($child as $node) {
    if (XMLNodeType($node) == 3 || XMLNodeType($node) == 4) $txt.=$node->content.$sep;
  }
  return xD($txt);
//return $txt;
}

//  Deletes all text childrens and adds CDATA sections with $str as contents
function XMLSetContent(&$node , $str) {
  $unlink=$node->node("unlink");
  if ($node->has_child_nodes()) { 
   	 foreach ($node->child_nodes() as $n) {
		if (XMLNodeType($n) ==3 || XMLNodeType($n) == 4) $n->unlink_node($n);
	 }
  }
  $dom=&XMLOwnerDocument($node);
  $node->add_child($dom->create_cdata_section(Xe($str)));

}

function XMLCheckPath($node, $path) {
	if (@$path{0}=='/' && @$path{1}!='/') {
	    $root = XMLOwnerDocument($node);
	    $root = $root->document_element();
		if ($root == $node) {
			return substr($path, 1);
		}
		return '/'.$root->tagname.$path;
	} else return $path;
}

// Searches $root using XPath ($path)
// Returns array of XMLResult objects ...
function XMLGetAll(&$root , $path) {
  $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  if (!$xe || count($xe->nodeset)==0) return array();
  $r=array();
  foreach ($xe->nodeset as $n) {
    array_push($r , new XMLResult($n));
  }
  return $r;
}

// Searches $root using XPath ($path) ... CallBack
// For every element found , calls $func with XMLResult as parameter 
function XMLGetAllCB(&$root , $path , $func) {
  $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  if (!$xe || count($xe->nodeset)==0) return false;
  foreach ($xe->nodeset as $n) {
    $func(new XMLResult($n));
  }
  return true;
}


// Gets value of XPath ($path) search
function XMLGetValue(&$root , $path) {
  $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  if (!empty($xe) && !empty($xe->value)) return Xd($xe->value);
}

// Gets only first (or null) XMLResult from table of search results (Xpath - $path)
function &XMLGet(&$root , $path) {
  $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  if (!$xe || count($xe->nodeset)==0) return false;
  return  new XMLResult($xe->nodeset[0]);
}

// Gets node from first result of XPath search ($path)
function &XMLGetNode(&$root , $path , $ctx = 0) {
  if (!$ctx) $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  if (!$xe || count($xe->nodeset)==0) return false;
  return $xe->nodeset[0];
}

function &XMLGetNodes(&$root , $path , $ctx = 0) {
  if (!$ctx) $ctx=xpath_new_context(XMLOwnerDocument($root));
  $xe=xpath_eval($ctx , XMLCheckPath($root , $path) , $root);
  return $xe;
}


function toIso2($str) {
return strtr($str , "¹œŸ¥Œ" , "±¶¼¡¦¬");
}

// Loads XML document from file
// returns XML DOM object
function &XMLLoad($xmlFileName , $defContent = '') {
	$buff = @file_get_contents($xmlFileName);
	
	if (!$buff) {
		$buff = $defContent;
	}
	if (XMLLIB_DOMXML) {
		if (!$buff) {
			return domxml_new_doc("1.0");
		} else {
			if (!($XML=domxml_open_mem($buff)))
				trigger_error("$xmlFileName - ten plik nie jest plikiem XML , lub jest w nim blad!", E_USER_ERROR);
			return $XML;
		}
	} else {
		$XML = new DOMDocument;
		if ($buff) {
			if (!$XML->loadXML($buff)) {
				trigger_error("$xmlFileName - ten plik nie jest plikiem XML , lub jest w nim blad!", E_USER_ERROR);
			}
		}
		return $XML;
	}
}




// Saves DOM object to file
function XMLSave (&$XML , $path , $compress = false , $format = false) {
      if (function_exists('lock_file')) lock_file($path);
	  if (XMLLIB_DOMXML) {
		  if (!$XML->dump_file($path , $compress , $format)) {
		  	// drupa próba...
			$src = $XML->dump_mem($compress , $format);
			if (!$src)
				trigger_error("$path - xml->dump_file zwróci³ 0, podobnie dump_mem! [$compress , $format]", E_USER_ERROR);
	  	    $f = fopen($path , 'wt');
			fwrite($f , $src);
		    fclose($f);
		  }
	  } else {
	  	$XML->save($path);
	  }
      if (function_exists('unlock_file')) unlock_file($path);
}
function &XMLRoot(&$XML) {
	if (XMLLIB_DOMXML) {
		return $XML->root();
	} else {
		return $XML->documentElement;
	}

//	extension_loaded
}
function &XMLOwnerDocument(&$node) {
	if (XMLLIB_DOMXML) {
		return $node->owner_document();
	} else {
		return $node->ownerDocument;
	}
}

// Deletes node (since domxml_unlink doesn't work)
function XMLunlink (&$node) {
//  $doc=$node->owner_document();
//	$unlink=$doc->node('unlink');
//	$unlink->append_child($node);
  $node->unlink_node($node);
}
function XMLNodeType(&$node) {
	if (XMLLIB_DOMXML) {
		return $node->node_type();
	} else {
		return $node->nodeType;
	}	
}


// Prints out XMLdoc source enclosed with <Textarea>
function XMLsource ($XML , $txta='cols=80 rows=30') {
        if ($txta) echo "<textarea $txta>";
				echo $XML->dumpmem();
				if ($txta) echo "</textarea>";
				}

// Inserts $node as the first child of $parent				
function XMLaddFirst(&$parent , &$node) {
  if ($parent->has_child_nodes()) {
    return $parent->insert_before($node , $parent->first_child());
  } else {
    return $parent->add_child($node);
  }
}
				

?>