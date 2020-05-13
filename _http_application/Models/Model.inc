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
            $user = $database("INSERT INTO models(name, brand, created) VALUES (:name,:brand,:created)",[":name"=>$name,":brand"=>$brand,':created'=>time()]);
            return !!($user->getDatabase()->lastInsertId());
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

    public function GetTotal($id){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS total FROM models WHERE brand=?",[$id])->fetchArrayOpt();
        return $exist->total;
    }

    public function Listing(){
        $database = $this->db;
        $list = $database(" SELECT id, name, (SELECT brd.name FROM brands AS brd WHERE brd.id=models.brand) AS brand, created, updated FROM models ORDER BY id DESC")->fetchObject();
        return $list;
    }

    public function Get($id){
        $database = $this->db;
        $result = $database("SELECT id, name, brand, created, updated FROM models WHERE id=:id", [':id'=>$id])->fetchArrayOpt();
        return $result;
    }

    public function UpdateModel($id ,array $data){
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $name = $data['name'];
            $brand = $data['brand'];
            $result = $database("UPDATE models SET name=:name, brand=:brand, updated=:updated WHERE id=:id",[":id"=>$id,":name"=>$name,":brand"=>$brand,':updated'=>time()]);
            return $result->isUpdated();
        }
        return $result;
    }

    public function Remove(int $id){
        $database = $this->db;
        $delete = $database("DELETE FROM models WHERE id=:id",[':id'=>$id]);
        return $delete->getStatement()->rowCount();
    }
}