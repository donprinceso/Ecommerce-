<?php


namespace model;


use application\Application;
use application\database\Database;
use application\locations\Url;

class Brand
{
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

    public function Add(array $data){
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $name = $data['name'];
            $image = $data['image'];
            $user = $database("INSERT INTO brands(name, image, created) VALUES (:name,:image,:created)",[":name"=>$name,":image"=>$image,':created'=>time()]);
            return !!($user->getDatabase()->lastInsertId());
        }
        return $result;
    }

    public function NameExist($name){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS exist FROM brands WHERE name=?",[$name])->fetchArrayOpt();
        return $exist->exist > 0;
    }

    public function Listing(){
        $database = $this->db;
        $list = $database(" SELECT id, name, image, (SELECT COUNT(mold.id) FROM models as mold WHERE mold.brand=brands.id) AS totalModels, created, updated FROM brands ORDER BY id DESC")->fetchObject();
        return $list;
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
                $name = "brand_{$time}." . $extension;
                $filename = Url::get_upload_full_dir().$name;
                if(move_uploaded_file($imgTemp, $filename)) {
                    $pubImages = Url::get_upload_generate_dir("/").$name;
                }
            }
        }
        return $pubImages;
    }

    public function UpdateBrand($id ,array $data){
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $name = $data['name'];
            $image = $data['image'];
            $result = $database("UPDATE brands SET name=:name, image=:image, updated=:updated WHERE id=:id",[":id"=>$id,":name"=>$name,":image"=>$image,':updated'=>time()]);
            return $result->isUpdated();
        }
        return $result;
    }

    public function CheckIDExist($id):bool{
        $result = $this->db->execute("SELECT COUNT(id) AS total FROM brands WHERE id=?",[$id])->fetchArrayOpt();
        return $result->total > 0;
    }

    public function Get($id){
        $database = $this->db;
        $result = $database("SELECT id, name, image, created, updated FROM brands WHERE id=:id", [':id'=>$id])->fetchArrayOpt();
        return $result;
    }

    public function RemoveLogo($id){
        $database = $this->db;
        $item = $database("SELECT image FROM brands WHERE id=?",[$id])->fetchArrayOpt();
        $image = Url::get_server_upload_root_url($item->image);
        return (Url::fileRemove($image));
    }

    public function Remove(int $id){
        $database = $this->db;
        $delete = $database("DELETE FROM brands WHERE id=:id",[':id'=>$id]);
        return $delete->getStatement()->rowCount();
    }


    public function GetName($id){
        $database = $this->db;
        $listname = $database("SELECT name FROM brands WHERE id=:id",["id"=>$id])->fetchArrayOpt();
        return $listname->name;
    }

    public function GetIcon($id){
        $database = $this->db;
        $listname = $database("SELECT image FROM brands WHERE id=:id",["id"=>$id])->fetchArrayOpt();
        return Url::Url_upload($listname->image);
    }

}