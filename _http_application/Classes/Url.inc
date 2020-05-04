<?php

namespace application\locations;

use application\Init\IClasses;
use DateTime;

abstract class Url implements IClasses {

    protected static function calculate_url(){
        if(isset($_SERVER['HTTP_HOST']) and isset($_SERVER['REQUEST_URI'])){
            $http = (isset($_SERVER['HTTPS']) and $_SERVER['HTTPS']=='on')? "https://":"http://";
            if($_SERVER['HTTP_HOST']=='localhost'){
                $request = (explode("/",$_SERVER['REQUEST_URI']));
                if(isset($request[1])){
                    $request = $request[1];
                }else{
                    $request = "";
                }
                $domain = $_SERVER['HTTP_HOST']."/$request";
            }else{
                $domain = $_SERVER['HTTP_HOST'];
            }
            $url = $http.$domain;
        }else{
            $url = "http://url-not-working.com";
        }
        return $url;
    }

    public static function Url(){
        return (empty(APP_PUBLIC_URL)?self::calculate_url():APP_PUBLIC_URL);
    }

    public static function Url_to($name){
        $name = str_replace("\\","/",$name);
        return self::url().((empty($name)?"":"/$name"));
    }

    public static function Redirect_to($name){
        $url = self::url_to($name);
        header("location: $url");
    }

    public static function ChangeExtension($link,$ext){
        $d = explode(".",$link);
        array_pop($d);
        $d[] = "$ext";
        return implode(".",$d);
    }

    public static function GetExtension($link):string {
        $d = explode(".",$link);
        $ext = end($d);
        return $ext;
    }

    public static function CheckExtension(string $link,string $ext){
        $d = explode(".",$link);
        $cext = end($d);
        return ($cext==$ext);
    }



    //base url
    public static function Base_url(){
        return APP_ROOT.DS;
    }

    public static function Upload_dir(){
        return "uploads".DS;
    }

    public static function get_server_upload_url(){
        return self::Base_url().self::Upload_dir();
    }

    static function get_upload_generate_dir(string $dir=DS){
        $date = new DateTime();
        $year = $date->format("Y");
        $month = $date->format("M");
        $day = $date->format("d");
        $link = implode($dir,[$year,$month,$day]).$dir;
        $dlink = self::get_server_upload_url().$link;
        if(!is_dir($dlink)) mkdir($dlink,0777,true);
        return $link;
    }

    static function get_upload_full_dir(){
        return self::get_server_upload_url().self::get_upload_generate_dir();
    }


    public static function get_server_upload_autogenerate_url(string $url){
        return self::get_upload_generate_dir()."/$url";
    }

    public static function get_server_upload_root_url(string $url){
        $url = str_replace("/","\\",$url);
        return self::get_server_upload_url()."$url";
    }


    public static function fileExist(string $filename):bool{
        return file_exists($filename);
    }

    public static function fileRemove(string $filename):bool{
        return unlink($filename);
    }





}