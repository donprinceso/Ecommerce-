<?php


namespace model;


use application\Application;
use application\database\Database;
use application\locations\Url;

class Model
{
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
            $brand = $data['brand'];
            $description = $data['description'];
            $price = $data['price'];
            $banner = $data['banner'];
            $sideFront = $data['sideFront'];
            $sideBack = $data['sideBack'];
            $sideTop = $data['sideTop'];
            $sideDown = $data['sideDown'];
            $sideLeft = $data['sideLeft'];
            $sideRight = $data['sideRight'];
            $features = $data['features'];
            $user = $database("INSERT INTO models(name, brand, description, price, banner, sideFront, sideBack, sideTop, sideDown, sideLeft, sideRight, features, created) VALUES (:name,:brand,:description,:price,:banner,:sideFront,:sideBack,:sideTop,:sideDown,:sideLeft,:sideRight,:features,:created)",
                [":name"=>$name,":brand"=>$brand,":description"=>$description,":price"=>$price,":banner"=>$banner,":sideFront"=>$sideFront,":sideBack"=>$sideBack,
                    ":sideTop"=>$sideTop,":sideDown"=>$sideDown,":sideLeft"=>$sideLeft,":sideRight"=>$sideRight,':features'=>$features,':created'=>time()])->isSaved();
            return $user;
        }
        return $result;
    }

    public function NameExist($brand,$name){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS exist FROM models WHERE brand=? AND name=?",[$brand,$name])->fetchArrayOpt();
        return $exist->exist > 0;
    }

    public function CheckIDExist($id):bool{
        $result = $this->db->execute("SELECT COUNT(id) AS total FROM models WHERE id=?",[$id])->fetchArrayOpt();
        return $result->total > 0;
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
                $name = "model_{$time}." . $extension;
                $filename = Url::get_upload_full_dir().$name;
                if(move_uploaded_file($imgTemp, $filename)) {
                    $pubImages = Url::get_upload_generate_dir("/").$name;
                }
            }
        }
        return $pubImages;
    }


    public function GetTotal($id){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS total FROM models WHERE brand=?",[$id])->fetchArrayOpt();
        return $exist->total;
    }

    public function Listing(){
        $database = $this->db;
        $list = $database(" SELECT id, name, (SELECT brd.name FROM brands AS brd WHERE brd.id=models.brand) AS brand, banner, price, created, updated FROM models ORDER BY id DESC")->fetchObject();
        return $list;
    }

    public function Get($id){
        $database = $this->db;
        return $database("SELECT id, name, brand, price, description, banner, sideTop, sideDown, sideFront, sideBack, sideLeft, sideRight, features, created, updated FROM models WHERE id=:id", [':id'=>$id])->fetchArrayOpt();
    }

    public function Update($id,$name,$brand,$price,$description,$banner,$top,$down,$front,$back,$left,$right,$features){
        $database = $this->db;
        $time = time();
        $result = $database("UPDATE models SET name=:name, brand=:brand, price=:price, description=:description, banner=:banner, sideTop=:top, sideDown=:down, sideFront=:front, sideBack=:back, sideLeft=:left, sideRight=:right, features=:features, updated=:updated WHERE id=:id",
            [":id"=>$id, ":name"=>$name, ":brand"=>$brand, ":price"=>$price, ":description"=>$description, ":banner"=>$banner, ":top"=>$top, ":down"=>$down, ":front"=>$front, ":back"=>$back, ":left"=>$left, ":right"=>$right, ":features"=>$features, ":updated"=>$time]);
        return $result->isUpdated();
    }

    public function RemoveLogo($id){
        $database = $this->db;
        $item = $database("SELECT banner, sideFront, sideBack, sideTop, sideDown, sideLeft, sideRight FROM models WHERE id=?",[$id])->fetchArrayOpt();
        $banner = Url::get_server_upload_root_url($item->banner);
        $sideFront = Url::get_server_upload_root_url($item->sideFront);
        $sideBack = Url::get_server_upload_root_url($item->sideBack);
        $sideTop = Url::get_server_upload_root_url($item->sideTop);
        $sideDown = Url::get_server_upload_root_url($item->sideDown);
        $sideLeft = Url::get_server_upload_root_url($item->sideLeft);
        $sideRight = Url::get_server_upload_root_url($item->sideRight);
        return (Url::fileRemove($banner) &&
            Url::fileRemove($sideFront) &&
            Url::fileRemove($sideBack) &&
            Url::fileRemove($sideTop) &&
            Url::fileRemove($sideDown) &&
            Url::fileRemove($sideLeft) &&
            Url::fileRemove($sideRight));
    }

    public function Remove(int $id){
        $database = $this->db;
        $delete = $database("DELETE FROM models WHERE id=:id",[':id'=>$id]);
        return $delete->getStatement()->rowCount();
    }

}