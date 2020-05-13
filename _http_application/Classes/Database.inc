<?php

namespace application\database;

use application\security\Validation;
use PDO;
use PDOException;
use PDOStatement;
use stdClass;

ini_set('memory_limit', '256M');

class Database {

    private $conn = null;

    public function __construct(){
        $host= APP_DB_HOST;
        $username= APP_DB_USERNAME;
        $password= APP_DB_PASSWORD;
        $database= APP_DB_NAME;
        $driver = APP_DB_DRIVER;
        $charset = APP_DB_CHARSET;

        try {
            $this->conn = new PDO("$driver:host=$host;dbname=$database", $username, $password);
            $this->conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            $this->conn->setAttribute(PDO::MYSQL_ATTR_INIT_COMMAND, "SET NAMES '$charset'");
            return $this->conn;
        }catch(PDOException $e) {
            die("Database Error: ".$e->getMessage());
        }

    }

    public function __invoke($query,array...$params){
        try{
            $stmt = $this->conn->prepare($query);
            if(count($params)>0) {
                foreach ($params as $param) {
                    $stmt->execute($param);
                }
            }else{
                $stmt->execute();
            }
        }catch(PDOException $e) {
            return $e->getMessage();
        }
        return (is_bool($stmt))?$stmt: new ResultFetch($this->conn,$stmt);
    }

    public function quick_exec($query){
        try{
            $this->conn->exec($query);
            return true;
        }catch(PDOException $e) {
            return $e->getMessage();
        }
    }

    public function execute($query,array $param=null){
        try{
            $stmt = $this->conn->prepare($query);
            $stmt->execute($param);
        }catch(PDOException $e) {
            return $e->getMessage();
        }
        return (is_bool($stmt))?$stmt: new ResultFetch($this->conn,$stmt);
    }

    public function __destruct(){
        $this->conn = null;
    }

    public function beginTransaction(){
        $this->conn->beginTransaction();
    }

    public function commit(){
        $this->conn->commit();
    }

    public function rollback(){
        if ($this->conn->inTransaction()) {
            $this->conn->rollBack();
        }
    }

    public function escape($text){
        //return $this->conn->quote($text);
        return mysqli_real_escape_string(new \mysqli(APP_DB_HOST,APP_DB_USERNAME,APP_DB_PASSWORD,APP_DB_NAME),$text);
    }

    public function close(){
        $this->conn = null;
    }

}



class ResultFetch{
    private $db = null;
    private $statement = null;

    public function __construct(PDO $db,PDOStatement $statement){
        $this->db = $db;
        if(!is_bool($statement)){
            $this->statement = $statement;
        }
    }

    public function fetchJSON():string {
        $result = [];
        $objs = $this->statement->fetchAll(PDO::FETCH_OBJ);
        if($this->statement->rowCount()>0){
            foreach ($objs as $obj){
                $result[] = $obj;
            }
        }
        return json_encode($result);
    }

    public function fetchObject() {
        return json_decode($this->fetchJSON());
    }

    public function fetchArray():array {
        $result = [];
        $objs = $this->statement->fetchAll(PDO::FETCH_ASSOC);
        if($this->statement->rowCount()>0){
            foreach ($objs as $obj){
                $result[] = $obj;
            }
        }
        return $result;
    }

    public function isSaved(){
        return !!$this->db->lastInsertId();
    }

    public function SavedId(){
        return $this->db->lastInsertId();
    }

    public function isUpdated(){
        return !!$this->statement->rowCount();
    }

    public function totalUpdated(){
        return $this->statement->rowCount();
    }

    public function fetchArrayOpt(int $id=0):stdClass{
        $obj = new stdClass();
        $objs = $this->statement->fetchAll(PDO::FETCH_ASSOC);
        if($this->statement->rowCount()>0){
            if(array_key_exists($id,$objs)){
                $obj = $objs[$id];
            }
        }
        return json_decode(json_encode($obj));
    }

    public function getStatement(){
        return $this->statement;
    }

    public function getDatabase(){
        return $this->db;
    }

}


