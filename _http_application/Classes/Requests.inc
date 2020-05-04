<?php
namespace application\init;

class PublicRequest{


    private $reqname = null;

    private $current = null;

    public function __construct(){
        $this->reqname = APP_PUBLIC_REQUEST;
    }

    public function RequestAvailable(){
        return isset($_GET[$this->reqname]);
    }

    public function GetRequest(){
        if(isset($_GET[$this->reqname])){
            return $_GET[$this->reqname];
        }else{
            return null;
        }
    }

    public function ProcessRequest($request){
        $data = explode("/",$request);
        array_walk($data,function (&$index){
            $index = stripslashes(strip_tags($index));
        });
        $data = array_filter($data);
        return $data;
    }

    public function RequestSplitter(){
        $data = $this->GetRequest();
        $data = $this->ProcessRequest($data);
        return $data;
    }

    //manually ge the url paramemter your self
    public function HasDataInIndex(int $i){
        $data = $this->RequestSplitter();
        return !!array_key_exists($i,$data);
    }

    public function GetDataInIndex(int $i){
        $data = $this->RequestSplitter();
        if(array_key_exists($i,$data)){
            return $data[$i];
        }
        return null;
    }

    //set the position and it will calulate the next url parameter
    public function SetCurrent(string $name){
        $this->current = $name;
        return $this;
    }

    public function HasNext(){
        $data = $this->RequestSplitter();
        $next = $this->FindNext();
        if(!is_null($next)){
            return !!(count($data)>$next);
        }
        return false;
    }

    public function FindCurrent(){
        $val = null;
        $data = $this->RequestSplitter();
        if(in_array($this->current,$data)){
            foreach ($data as $i=>$d){
                if($this->current==$d){
                    $val = $i;
                }
            }
        }
        return $val;
    }

    public function FindCurrentValue(){
        $val = null;
        $data = $this->RequestSplitter();
        if(in_array($this->current,$data)){
            foreach ($data as $i=>$d){
                if($this->current==$d){
                    $val = $data[$i];
                }
            }
        }
        return $val;
    }

    public function FindNext(){
        $val = null;
        $data = $this->RequestSplitter();
        if(in_array($this->current,$data)){
            foreach ($data as $i=>$d){
                if($this->current==$d){
                    $val = ++$i;
                }
            }
        }
        return $val;
    }

    public function FindNextValue(){
        $val = null;
        $data = $this->RequestSplitter();
        if(in_array($this->current,$data)){
            foreach ($data as $i=>$d){
                if($this->current==$d){
                    $val = ++$i;
                    $val = $data[$val];
                }
            }
        }
        return $val;
    }

    public function GetNextParameters(bool $exclude_current=false):array {
        $data = $this->RequestSplitter();
        $position = ($exclude_current==false)? $this->FindCurrent() : $this->FindNext();
        return array_slice($data,$position);
    }

    public function GetNextParametersString(bool $exclude_current=false, string $glue="/"):string {
        return implode($glue,$this->GetNextParameters($exclude_current));
    }

}


abstract class LoadParameters{

    public function __invoke(){

    }

    public static function load_param_css($data){
        if(is_array($data)){
            if(key_exists("css",$data)){
                foreach ($data['css'] as $css){
                    echo "<link rel='stylesheet' href='$css'/>\r\n\t";
                }
            }
        }
    }

    public static function load_param_js($data){
        if(is_array($data)){
            if(key_exists("js",$data)){
                foreach ($data['js'] as $js){
                    echo "<script src='$js'></script>\r\n\t";
                }
            }
        }
    }

    public static function load_param_js_script($data){
        if(is_array($data)){
            if(key_exists("js-script",$data)){
                foreach ($data['js'] as $js){
                    echo "<script>\r\n$js\r\n</script>\r\n\t";
                }
            }
        }
    }

    public static function load_param_css_script($data){
        if(is_array($data)){
            if(key_exists("css-script",$data)){
                foreach ($data['css-script'] as $css){
                    echo "<style>\r\n$css\r\n\t</style>\r\n\t";
                }
            }
        }
    }

    public static function load_param_html($data){
        if(is_array($data)){
            if(key_exists("html",$data)){
                foreach ($data['html'] as $html){
                    echo "\r\n $html \r\n";
                }
            }
        }
    }

}