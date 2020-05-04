<?php
namespace application\security;

class Hashing{

    const SHA256 = "sha256";

    public function PasswordHash($password){
        if(function_exists('password_verify')){
            $s = password_hash($password,PASSWORD_DEFAULT,['cost'=> APP_ENCRYPTION_LEVEL]);
        }else{
            $s = hash(APP_ENCRYPTION_LEVEL_ALT ,$password);
        }
        return $s;
    }

    public function PasswordVerify($password,$hash):bool{
        return password_verify($password,$hash);
    }

    public function Hash(string $text, string $type = Hashing::SHA256){
        return hash($type,$text);
    }

}