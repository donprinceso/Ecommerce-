<?php
namespace model;

use application\database\Database;
use application\Application;


class Staff{

    private $cookies;
    private $encrypt;
    private $control;
    private $general;

    public function __construct($db=null){
        if(!is_null($db)){
            $this->db = $db;
        }else{
            $this->db = new Database();
        }
        $this->general = new General($db);
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

    public function Listing(){
        $database = $this->db;
        $list = $database(" SELECT id, firstname, lastname, email, address, age, sex, department, created, updated FROM staffs ORDER BY id DESC")->fetchObject();
        return $list;
    }
    public function Add(array $data){
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $firstname = $data['firstname'];
            $lastname = $data['lastname'];
            $email = $data['email'];
            $address = $data['address'];
            $age = $data['age'];
            $aex = $data['sex'];
            $user = $database("INSERT INTO brands(name, image, created) VALUES (:name,:image,:created)",[":name"=>$name,":image"=>$image,':created'=>time()]);
            return !!($user->getDatabase()->lastInsertId());
        }
        return $result;
    }
}
