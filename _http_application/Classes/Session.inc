<?php

namespace application\browser;

use application\Init\IClasses;

class Session implements IClasses{

    public function setSession($name,$value){
        $_SESSION[$name] = $value;
    }

    public function removeSession($name){
        if($this->SessionExist($name)) {
            $_SESSION[$name] = null;
            unset($_SESSION[$name]);
        }
    }

    public function getSession($name){
        return $this->SessionExist($name)?$_SESSION[$name]:null;
    }

    public function SessionExist($name):bool{
        return isset($_SESSION[$name]);
    }

}