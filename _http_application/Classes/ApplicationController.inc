<?php

namespace application\Init;

if(!defined("King_Framework")) die("You cannot access controller file directly");

use application\browser\Cookies;
use application\browser\Session;
use application\Pages;
use application\security\EncryptionClass;
use application\security\Hashing;
use application\security\ServerDetails;


abstract class ApplicationController implements IClasses, IController {

    public function __construct(){

    }

    public function __invoke() {}

    public function Page():PagesController{
        return new Pages();
    }

    public function Image($img=null):Image{
        return new Image($img);
    }

    public function Load():Load{
        return new Load();
    }

    public function PublicRequest():PublicRequest{
        return new PublicRequest();
    }

    public function Encryption():EncryptionClass{
        return new EncryptionClass();
    }

    public function Hashing():Hashing{
        return new Hashing();
    }

    public function Cookies():Cookies{
        return new Cookies();
    }

    public function Storage():Session{
        return new Session();
    }

    public function DateAndTime():DateAndTime{
        return new DateAndTime();
    }

    public function Server():ServerDetails{
        return new ServerDetails();
    }

    public function __destruct(){

    }

}




