<?php
namespace application\browser;


use application\Init\IClasses;


class Cookies implements IClasses {

    /**
     * @param $name
     * @param string $value
     * @param int $expire In Minutes
     * @return Cookies
     */
    public function SetCookie(string $name, $value='',int $expire=0):Cookies{
        $expire = ($expire>0)?(time()+(60*$expire)):$expire;
        if(version_compare(PHP_VERSION, '7.3')>0){
            setcookie($name,$value,$expire,APP_COOKIE_PATH,APP_COOKIE_DOMAIN,APP_COOKIE_SECURE,APP_COOKIE_HTTP,APP_COOKIE_SAMESITE);
        }else{
            setcookie($name,$value,$expire,APP_COOKIE_PATH,APP_COOKIE_DOMAIN,APP_COOKIE_SECURE,APP_COOKIE_HTTP);
        }
        return $this;
    }

    public function CookieExist($name):bool{
        return isset($_COOKIE[$name]);
    }

    public function GetCookie(string $name){
        if(isset($_COOKIE) and isset($_COOKIE[$name])){
            return $_COOKIE[$name];
        }
        return null;
    }

    public function GetCookies():array{
        $arr = [];
        foreach ($_COOKIE as $id=>$value){
            $arr[] = ['name'=>$id,'value'=>$value];
        }
        return $arr;
    }

    public function RemoveCookie($name){
        if(version_compare(PHP_VERSION, '7.3')>0) {
            setcookie($name, '', time() - 3600, APP_COOKIE_PATH, APP_COOKIE_DOMAIN, APP_COOKIE_SECURE, APP_COOKIE_HTTP, APP_COOKIE_SAMESITE);
            setcookie($name,false);
            unset($_COOKIE[$name]);
        }else{
            setcookie($name, '', time() - 3600, APP_COOKIE_PATH, APP_COOKIE_DOMAIN, APP_COOKIE_SECURE, APP_COOKIE_HTTP);
            setcookie($name,false);
            unset($_COOKIE[$name]);
        }
        return $this;
    }

}


