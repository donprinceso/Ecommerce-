<?php
namespace application\init;

class ThirdParty implements IClasses {

    private static $thirdparty = [];
    public static $url;


    static public function Add(string $name, string $file){
        self::$thirdparty[$name] = self::$url.$file;
    }

    static public function Exist(string $name):bool{
        return isset(self::$thirdparty[$name]);
    }

    static public function Load(string $name){
        return include self::$thirdparty[$name];
    }

}