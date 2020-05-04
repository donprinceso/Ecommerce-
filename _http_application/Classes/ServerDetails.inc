<?php

namespace application\security;


class ServerDetails{

    private $server;

    function __construct(){
        $this->server = $_SERVER;
    }



    public function GetUser(){
        return $this->server['USER'];
    }

    public function GetRemoteIP(){
        return $this->server['REMOTE_ADDR'];
    }

    public function GetEnvironmentRemoteIP(){
        return getenv('REMOTE_ADDR');
    }

    public function GetServerIP(){
        return $this->server['SERVER_ADDR'];
    }

    public function GetRemotePort(){
        return $this->server['REMOTE_PORT'];
    }

    public function GetUserAgent(){
        return $this->server['HTTP_USER_AGENT'];
    }

}