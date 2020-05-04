<?php
namespace application\security;

use ZipArchive;

class Zip
{

    private $zip;

    function __construct($url=null){
        $this->zip = new ZipArchive();
        if(!is_null($url)){
            $this->Create($url);
        }
    }

    public function Create($url){


    }


}