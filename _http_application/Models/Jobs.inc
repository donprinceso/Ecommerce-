<?php


namespace model;

use application\Application;
use application\database\Database;
use application\locations\Url;
class Jobs
{

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
    }

    public function __destruct()
    {
        if (!is_null($this->db)){
            $this->db->close();
        }
    }

    public function TitleExist($title){
        $database = $this->db;
        $exist = $database("SELECT COUNT(id) AS exist FROM news WHERE title=?",[$title])->fetchArrayOpt();
        return $exist->exist > 0;
    }

    public function CheckIDExist(int $id){
        $database = $this->db;
        $result = $database("SELECT COUNT(id) AS total FROM news WHERE id=?",[$id])->fetchArrayOpt();
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
                $name = "brand_{$time}." . $extension;
                $filename = Url::get_upload_full_dir().$name;
                if(move_uploaded_file($imgTemp, $filename)) {
                    $pubImages = Url::get_upload_generate_dir("/").$name;
                }
            }
        }
        return $pubImages;
    }

    public function Add(array $data)
    {
        $result = false;
        $database = $this->db;
        if(is_array($data)){
            $title = $data['title'];
            $body = $data['body'];
            $news = $database("INSERT INTO news(title, body, created) VALUES (:title, :body, :created)",[":title" => $title, ":body" => $body, ':created'=>time()])->isSaved();
            return $news;
        }
        return $result;
    }

    public function listing(){
        $database =$this->db;
        $list = $database("SELECT id,title,view,created,updated FROM news ORDER BY id DESC")->fetchObject();
        return$list;
    }

    public function Remove(int $id){
        $database = $this->db;
        $delete = $database("DELETE FROM brands WHERE id=:id",[':id'=>$id]);
        return $delete->getStatement()->rowCount();
    }



}