<?php
namespace application\init;

use application\database\Database;

class jsonClass{

    private $db;
    private $db_table;
    private $db_field;

    function __construct($table,$field){
        $this->db = new Database();
        $this->db_table = $table;
        $this->db_field = $field;
    }

    public function jsonEncode($data){

    }

    public function jsonDecode($data){

    }





    public function AddItem($post,$userid){
        $database = $this->db;
        $user = $database("SELECT likes FROM post WHERE id=?",[$post])->fetchArrayOpt();
        $likes = $user->likes;
        $lik = (!empty($likes))? json_decode($likes,true) : [];
        $lik['likes'][] = ['date'=>time(), 'user'=>$userid];
        $likes = json_encode($lik);
        $user = $database("UPDATE post SET likes=? WHERE id=?",[$likes,$post]);
        return !!$user->getStatement()->rowCount();
    }

    public function ExistLikes(int $post,int $userid):bool {
        $database = $this->db;
        $user = $database("SELECT likes FROM post WHERE id=?",[$post])->fetchArrayOpt();
        $likesD = $user->likes;
        $likes = json_decode($likesD, true);
        if(!is_null($likes) and isset($likes['likes'])){
            foreach ($likes['likes'] as $like){
                if($like['user']==$userid) return true;
            }
        }
        return false;
    }

    public function RemoveLikes(int $post,int $userid):bool {
        $database = $this->db;
        $posts = $database("SELECT likes FROM post WHERE id=?",[$post])->fetchArrayOpt();
        $likesD = $posts->likes;
        $likes = json_decode($likesD, true);
        if(!is_null($likes) and isset($likes['likes'])){
            $likes = $likes['likes'];
            foreach ($likes as $i=>$like){
                if($like['user']==$userid) {
                    unset($likes[$i]);
                }
            }
            $likes = array_values($likes);
            $i1['likes'] = $likes;
            $likesD = json_encode($i1);
            $posts = $database("UPDATE post SET likes=? WHERE id=?",[$likesD,$post]);
            return !!$posts->isUpdated();
        }
        return false;
    }

    public function AppendLikes(&$data):Void{
        if(is_array($data)){
            array_walk($data,function(&$index) {
                if (array_key_exists("poster", $index)) {
                    $id = $index['id'];
                    if($this->Exist($id)){
                        $detail = $this->user->Data();
                        $index['likes'] = $this->ExistLikes($id,$detail->id);
                        $index['totalLikes'] = $this->GetTotalLikes($id);
                    }
                }
            });
        }
    }

    public function AppendSingleLikes(&$data):Void{
        if(isset($data->poster)){
            $poster = $data->poster;
            if($this->Exist($poster)){
                $detail = $this->user->Data();
                $data->likes = $this->ExistLikes($poster,$detail->id);
                $data->totalLikes = $this->GetTotalLikes($poster);
            }
        }
    }

    public function GetTotalLikes($post){
        $database = $this->db;
        $user = $database("SELECT likes FROM post WHERE id=?",[$post])->fetchArrayOpt();
        $likesD = $user->likes;
        $likes = json_decode($likesD, true);
        if(!is_null($likes) and isset($likes['likes'])){
            return count($likes['likes']);
        }
        return 0;
    }

}