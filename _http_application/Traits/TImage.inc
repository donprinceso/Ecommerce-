<?php

namespace application\database;


use application\locations\Url;

trait TImage{

    function __construct(){

    }

    function processExtension(string $mime){
        switch ($mime){
            case "image/jpeg":return "jpg";
            case "image/jpg":return "jpg";
            case "image/png":return "png";
            case "image/x-png":return "png";
            default: return "";
        }
    }

    function isJPEG(string $mime){
        return ($mime=="image/jpeg" or $mime=="image/jpg");
    }

    /**
     * This remove all image header encoded by most cameras only on JPEG
     * @param $temp $_FILES['temp_name'] upload image
     */
    private function StripImageHeaderInfoJpeg($temp){
        exec(Url::Base_url()."ThirdParty/jhead -purejpg $temp");
    }

}