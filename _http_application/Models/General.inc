<?php


namespace model;


use application\database\Database;

class General
{
    private $db;

    public function __construct($db=null){
        if(!is_null($db)){
            $this->db = $db;
        }else{
            $this->db = new Database();
        }
    }



    public $imageAccepted = ['image/jpg','image/jpeg','image/png'];
    public $imageSize = 1500;
    public $imageQuality = 85;
    public $imageDataSize = 1024*10;

    //brand
    public $imageBrandSize = 600;
    public $imageBrandQuality = 90;

}