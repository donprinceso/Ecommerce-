<?php

namespace application\security;


use application\Init\IClasses;

class Validation implements IClasses {

    public static function sanitize($data){
        $data = trim($data);
        $data = stripslashes($data);
        $data = strip_tags($data);
        $data = htmlspecialchars($data);
        return $data;
    }

    public static function deep_cleans(&$text):Void{
        $db = new Database();
        $text = $db->escape($text);
        $text = self::sanitize($text);
    }

    public static function fullname($str){
        if(preg_match("/^[A-Za-z]{3,20} [A-Za-z]{3,20}+$/",$str)){
            return false;
        }
        return true;
    }

    public static function name($str){
        if(preg_match("/^[A-Za-z]{3,20}+$/",$str)){
            return false;
        }
        return true;
    }

    public static function city($str){
        if(preg_match("/^[A-Za-z\-, ]{2,60}+$/",$str)){
            return false;
        }
        return true;
    }

    public static function email($str){
        $str = filter_var($str, FILTER_SANITIZE_EMAIL);
        if(!filter_var($str,FILTER_VALIDATE_EMAIL)===false){
            return false;
        }
        return true;
    }

    public static function password($str){
        if(strlen($str)>=6 and strlen($str)<=50){
            if(preg_match("#^[ -~£±§]*$#",$str) === 1){
                return false;
            }
        }
        return true;
    }



    public static function LoginId($str){
        if(preg_match("/^[0-9]{1,10}+$/",$str)){
            return true;
        }
        return false;
    }

    public static function LoginHash($hash){
        if(preg_match("/^[a-z0-9\-\_]{15,22}+$/",$hash)){
            return true;
        }
        return false;
    }


    public static function text($text,int $min=3, $max=null,bool $optional=false){

        if($optional and empty($text)){
            return true;
        }

        $length = (is_null($max))? (strlen($text)>=$min) : (strlen($text)>=$min and strlen($text)<=$max);
        if($length){
            if(preg_match("#^[ -~£±§\"']*$#",$text)){
                return true;
            }
        }
        return false;
    }

    public static function number($num,int $min = 0, $max=null,bool $optional=false){

        if($optional and empty($text)){
            return true;
        }

        $num = intval($num);
        if(is_int($num)){
            return (is_null($max))? ($num>=$min) : ($num>=$min && $num<=$max);
        }
        return false;
    }

    public static function check_valid_url($url){

        $c=curl_init();
        curl_setopt($c,CURLOPT_URL,$url);
        curl_setopt($c,CURLOPT_HEADER,1);//get the header
        curl_setopt($c,CURLOPT_NOBODY,1);//and *only* get the header
        curl_setopt($c,CURLOPT_RETURNTRANSFER,1);//get the response as a string from curl_exec(), rather than echoing it
        curl_setopt($c,CURLOPT_FRESH_CONNECT,1);//don't use a cached version of the url
        if(!curl_exec($c)){
            return false;
        }else{
            return true;
        }
        //$httpcode=curl_getinfo($c,CURLINFO_HTTP_CODE);
        //return ($httpcode<400);
    }


    private static function mb_str_replace($needle, $replacement, $haystack) {
        return implode($replacement, mb_split($needle, $haystack));
    }

    public static function filterNormalText($text,$format="/[^A-Za-z0-9 ]/"){
        return preg_replace($format,"",trim($text));
    }

    public static function filterFromHtmlText($text,$format="/[^A-Za-z0-9 ]/"){
        $text = filter_var($text, FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH);
        return preg_replace($format,"",trim($text));
    }

    public static function pickWords($text,$to=10,$from=0){
        $wordArr = explode(" ",$text);
        $wordArr = array_slice($wordArr,$from,$to);
        $wordArr = array_filter($wordArr,function ($i){
            return !empty($i);
        });
        $text = implode(" ",$wordArr);
        return $text;
    }

}