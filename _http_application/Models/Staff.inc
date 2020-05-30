<?php
namespace model;

use application\database\Database;
use application\Application;
use application\locations\Url;


class Staff{

    private $db;
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

    public function Upload($image,$time){
        $pubImages="";
        $imgName = $image['name'];
        $imgError = $image['error'];
        $imgTemp = $image['tmp_name'];
        $imgType = trim($image['type']);
        if($imgError == 0 and !empty($imgType)) {
            $temp = explode(".", $imgName);
            $extension = end($temp);
            $finfo = finfo_open(FILEINFO_MIME_TYPE);
            $mime = finfo_file($finfo, $imgTemp);
            if(in_array($mime,$this->general->imageAccepted)){
                $name = "staff_{$time}." . $extension;
                $filename = Url::get_upload_full_dir().$name;
                if(move_uploaded_file($imgTemp, $filename)) {
                    $pubImages = Url::get_upload_generate_dir("/").$name;
                }
            }
        }
        return $pubImages;
    }

    public function CheckIDExist($id):bool{
        $result = $this->db->execute("SELECT COUNT(id) AS total FROM staffs WHERE id=?",[$id])->fetchArrayOpt();
        return $result->total > 0;
    }

    public function ExistEmail($email){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS total FROM staffs WHERE email=?",[$email])->fetchArrayOpt();
        if(isset($exist->total)){
            return $exist->total > 0;
        }
        return false;
    }

    public function ExistPhone($email){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS total FROM staffs WHERE phone=?",[$email])->fetchArrayOpt();
        if(isset($exist->total)){
            return $exist->total > 0;
        }
        return false;
    }

    public function Add($avatar,$name,$gender,$maritalstatus,$email,$phone,$ssn,$country,$city,$residence){
        $database = $this->db;
        $date = time();
        $add = $database("INSERT INTO staffs(name, avatar, gender, maritalStatus, email, phone, ssn, country, city, residence, created) VALUES (?,?,?,?,?,?,?,?,?,?,?)",
            [$name,$avatar,$gender,$maritalstatus,$email,$phone,$ssn,$country,$city,$residence,$date]);
        return $add->isSaved();
    }

    public function Update($id,$avatar,$name,$gender,$maritalstatus,$email,$phone,$ssn,$country,$city,$residence){
        $database = $this->db;
        $time = time();
        $update = $database("UPDATE staffs SET avatar=:avatar, name=:name, gender=:gender, maritalStatus=:marital, email=:email, phone=:phone, ssn=:ssn, country=:country, city=:city, residence=:residence, updated=:updated WHERE id=:id", [
            ":avatar"=>$avatar, ":name"=>$name, ":gender"=>$gender, ":marital"=>$maritalstatus, ":email"=>$email, ":phone"=>$phone, ":ssn"=>$ssn, ":country"=>$country, ":city"=>$city, ":residence"=>$residence, ":updated"=>$time, ":id"=>$id
        ]);
        return $update->isUpdated();
    }

    public function Remove(int $id){
        $database = $this->db;
        $delete = $database("DELETE FROM staffs WHERE id=:id",[':id'=>$id]);
        return $delete->getStatement()->rowCount();
    }

    public function RemoveProfile($id){
        $database = $this->db;
        $item = $database("SELECT avatar FROM staffs WHERE id=?",[$id])->fetchArrayOpt();
        $file = Url::get_server_upload_root_url($item->avatar);
        return Url::fileRemove($file);
    }

    public function Listing(){
        $database = $this->db;
        $list = $database("SELECT id, avatar, name, gender, maritalStatus, email, phone, ssn, country, city, residence, updated, created FROM staffs ORDER BY id DESC")->fetchObject();
        return $list;
    }

    public function Get($id){
        $database = $this->db;
        $item = $database("SELECT id, avatar, name, gender, maritalStatus, email, phone, ssn, country, city, residence, updated, created FROM staffs WHERE id=?",[$id])->fetchArrayOpt();
        return $item;
    }




}
