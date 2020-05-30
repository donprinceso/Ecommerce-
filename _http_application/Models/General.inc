<?php


namespace model;


use application\database\Database;
use application\locations\Url;

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

    const appName = "Tele Tech Ltd.";

    public $imageAccepted = ['image/jpg','image/jpeg','image/png'];
    public $imageSize = 1500;
    public $imageQuality = 85;
    public $imageDataSize = 1024*10;

    //brand
    public $imageBrandSize = 600;
    public $imageBrandQuality = 90;

    public $currencySymbol = "$";
    public $dateFormat = "l j F Y (h:iA)";

    public function processPrice($price){
        $price = floatval($price);
        return $this->currencySymbol.number_format($price,2);
    }

    public function processImage($link){
        return Url::Url_upload($link);
    }

    public function processDate($date){
        if(is_null($date) or empty($date)){
           return "N/A";
        }
        $date = floatval($date);
        return date($this->dateFormat,$date);
    }



}