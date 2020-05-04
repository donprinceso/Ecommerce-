<?php
namespace application\security;


use Exception;
use stdClass;

class GPS
{

    private $server;

    function __construct(){
        $this->server = new ServerDetails();
    }

    function GetLocation(){
        error_reporting(0);
        $result = new stdClass();

        $user_ip = $this->server->GetEnvironmentRemoteIP();
        try{
            $geo = unserialize(file_get_contents("http://www.geoplugin.net/php.gp?ip=$user_ip"));
            $country = trim($geo["geoplugin_countryName"]);
            $city = trim($geo["geoplugin_city"]);
        }catch (Exception $ex){
            $result->error = $ex;
            $country = null;
            $city = null;
        }
        $result->ip = $user_ip;
        $result->country = $country;
        $result->city = $city;
        return $result;
    }


}