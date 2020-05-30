<?php
namespace model;

use application\database\Database;
use application\security\Validation;
use application\Application;
use Exception;
use stdClass;

class Admin{

     private $id;
     private $cookies;
     private $encrypt;
     private $control;
     private $general;
     public $cookie_id =  "admin_id";
     public $cookie_hash =  "admin_sh";
     private $cookie_preifix = "admin_";


     public function __construct($db=null){
         if(!is_null($db)){
             $this->db = $db;
         }else{
             $this->db = new Database();
         }
         $this->control = new Application();
         $this->cookies = $this->control->Cookies();
         $this->encrypt = $this->control->Encryption();

     }

     public function Data(){
         $result = new stdClass();
         $database = $this->db;
         $cookies = $this->cookies;

         if($this->IsLogedin()){
             $id = $cookies->GetCookie($this->cookie_id);
             $hash = $cookies->GetCookie($this->cookie_hash);
             try {
                 $deid = $this->LoginDecode($id);
                 $dehash = $this->LoginDecode($hash);
                 if (Validation::LoginId($deid) and Validation::LoginHash($dehash)) {
                     $deid = Validation::sanitize($this->db->escape($deid));
                     $dehash = Validation::sanitize($this->db->escape($dehash));
                     $user = $database("SELECT id, email FROM admin WHERE id=:id AND hash=:hash", [':id' => $deid, ':hash' => $dehash])->fetchArrayOpt();
                     $result = $user;
                 }
             }catch (Exception $ex){

             }
         }
         return $result;
     }

     public function isLogedin(){
         $result = false;
         $database = $this->db;
         $cookies = $this->cookies;

         if($cookies->CookieExist($this->cookie_id) and $cookies->CookieExist($this->cookie_hash)){
             $id = $cookies->GetCookie($this->cookie_id);
             $hash = $cookies->GetCookie($this->cookie_hash);
             try {
                 $deid = $this->LoginDecode($id);
                 $dehash = $this->LoginDecode($hash);
                 if(Validation::LoginId($deid) and Validation::LoginHash($dehash)) {
                     $deid = Validation::sanitize($this->db->escape($deid));
                     $dehash = Validation::sanitize($this->db->escape($dehash));
                     $user = $database("SELECT hash FROM admin WHERE id=:id AND hash=:hash", [':id' => $deid, ':hash' => $dehash])->fetchArrayOpt();
                     if (isset($user->hash)) {
                         if ($user->hash == $dehash) {
                             $result = true;
                         }
                     }
                 }
             }catch (Exception $ex){}
         }

         return $result;
     }

     public function login(array $data){
         $database = $this->db;
         $result = false;
         $cookies = $this->cookies;
         if(is_array($data)){
             $email = $data['email'];
             $password = $data['password'];
             $user = $database("SELECT id,email,password FROM admin WHERE email=:email",[':email' => $email]);
               $data = $user->fetchArrayOpt();
             if ($data){
                 if (isset($data->password)){
                     $id = $data->id;
                     $login = $this->cookie_preifix.time();
                     $hash = $data->password;
                     if($this->control->Hashing()->PasswordVerify($password,$hash)) {
                         $enid = $this->LoginEncode($id);
                         $enlogin = $this->LoginEncode($login);
                         $cookies->SetCookie($this->cookie_id, $enid)->SetCookie($this->cookie_hash, $enlogin);
                         $update = $database("UPDATE admin SET hash=:hash, lastSeen=:tt, loginTime=:tt WHERE id=:id",
                             [':tt' => time(), ':hash' => $login, ':id' => $id]);
                         if ($update->getStatement()->rowCount()) {
                             $result = true;
                         }
                     }
                 }
             }
         }
         return $result;
     }



    public function verifyPassword($id,$password){
        $database = $this->db;
        $user = $database("SELECT id,password FROM admin WHERE id=:id",[':id' => $id])->fetchArrayOpt();
        if (isset($user->password)) {
            $hash = $user->password;
            return $this->control->Hashing()->PasswordVerify($password, $hash);
        }
    }

    public function UpdatePassword(int $id,string $password):bool{
        $db = $this->db;
        return $db("UPDATE admin SET password=:password WHERE id=:id",[':id'=>$id,':password'=>$password])->isUpdated();
    }

     function LoginEncode($data):string {
         return $this->encrypt->AesEncrypt(base64_encode($data));
     }

     function LoginDecode($data):string {
         return base64_decode($this->encrypt->AesDecrypt($data));
     }

    public function Signout(){
        $cookies = $this->cookies;
        $cookies->RemoveCookie($this->cookie_id)->RemoveCookie($this->cookie_hash);
        return true;
    }
 }