<?php
namespace model;

use application\database\Database;
use application\security\Validation;
use application\Application;
use application\locations\Url;

class Customer{

    private $id;
    private $db;
    private $cookies;
    private $user;
    private $encrypt;
    private $control;

    public $cookie_id =  "Customer_id";
    public $cookie_hash =  "Customer_sh";
    private $cookie_preifix = "user";

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

    public function __destruct()
    {
        if (!is_null($this->db)){
            $this->db->close();
        }
    }

    /**
     * @return bool
     */
    public function CustomerRequest(){
        return (isset($_POST) and isset($_POST['Customer_Request']));
    }

    public function Listing(){
        $database = $this->db;
        $list = $database(" SELECT id, firstname, lastname, email, password, hash, lastSeen, loginTime, created, updated FROM customers ORDER BY id DESC")->fetchObject();
        return $list;
    }
    /**
     * @return bool
     */
    public function IsLogedin():bool{
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
                    $user = $database("SELECT hash FROM customers WHERE id=:id AND hash=:hash", [':id' => $deid, ':hash' => $dehash])->fetchJSON();
                    $d = json_decode($user);
                    if (isset($d[0]->hash)) {
                        if ($d[0]->hash == $dehash) {
                            $result = true;
                        }
                    }
                }
            }catch (Exception $ex){

            }
        }

        return $result;
    }

    /**
     * @param array $data
     * @return bool
     */
    public function register(array $data){
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $data[] = time();
            $sql = "INSERT INTO customers(firstname,lastname, email, password) VALUES (?,?,?,?)";
            $user = $database($sql,$data);
            if(is_string($user)){
                echo $user;
            }else{
                return !!($user->getDatabase()->lastInsertId());
            }
        }
        return $result;
    }
    /**
     * @param array $data
     * @return bool
     */
    public function login(array $data):bool
    {
        $database = $this->db;
        $cookie = $this->cookies;
        if (is_array($data)){
            $email = $data['email'];
            $password = $data['password'];
            $sql = "SELECT id, password FROM customers WHERE email = :email,[':email'=>$email]]";
            $this->user =  $database($sql);
            $data = $this->user->fetchJSON();
            if ($data){
                $data = json_decode($data,false);
                if (isset($data[0]->password)){
                    $id = $data[0]->id;
                    $login = $this->cookie_preifix.time();
                    $hash = $data[0]->password;
                    if($this->control->Hashing()->PasswordVerify($password,$hash)){
                        $enid = $this->LoginEncode($id);
                        $enlogin = $this->LoginEncode($login);
                        $cookie->SetCookie($this->cookie_id,$enid)->SetCookie($this->cookie_hash,$enlogin);
                        $update = $database("UPDATE customers SET hash=:hash, lastSeen=:time, loginTime=:time WHERE id=:id",
                            [':time'=>time(),':hash'=>$login,':id'=>$id]);
                        if($update->getStatement()->rowCount()) {
                            $result = true;
                        }
                    }
                }
            }


        }
        return $result;
    }

    /**
     * @return bool
     */
    public function Signout(){
        $result = false;
        $database = $this->db;
        $cookies = $this->cookies;
        if($this->user!=null){
            $cookies->RemoveCookie($this->cookie_id)->RemoveCookie($this->cookie_hash);
            $update = $database("UPDATE customers SET lastSeen=:lastseen WHERE id=:id",[':lastseen'=>time(),':id'=>$this->user->id]);
            if($update->getStatement()->rowCount()){
                $result = true;
            }
        }else{
            $cookies->RemoveCookie($this->cookie_id)->RemoveCookie($this->cookie_hash);
            $result = true;
        }
        return $result;
    }

    function LoginEncode($data):string {
        return $this->encrypt->AesEncrypt(base64_encode($data));
    }

    function LoginDecode($data):string {
        return base64_decode($this->encrypt->AesDecrypt($data));
    }
}
