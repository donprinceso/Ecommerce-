<?php

namespace application\database;


class QueryBuilder{

    use TQueryBuilder;

    const GREATER = 1;
    const GREATER_EQUAL = 2;
    const EQUAL = 3;
    const LESSER = 4;
    const LESSER_EQUAL = 5;
    const NOT_EQUAL = 5;

    private $query="";

    public function Get(string $table,string...$fields):QueryBuilder{
        $fields = implode(",",$fields);
        $this->query = "SELECT $fields FROM $table";
        return $this;
    }

    public function Add(string $table,array...$fields):QueryBuilder{
        $fields = array_values($fields);
        $keys = $values = [];
        array_walk($fields,function ($index) use (&$keys,&$values) {
            $key = array_keys($index)[0];
            $keys[] = $key;
            $values[] = $index[$key];
        });
        $field = implode(",",$keys);
        $value = "'".implode("','",$values)."'";
        $this->query = "INSERT INTO $table ($field) VALUES ($value)";
        return $this;
    }

    public function Edit(string $table,array ...$fields):QueryBuilder{
        $fields = array_values($fields);
        $keys = $values = [];
        array_walk($fields,function ($index) use (&$keys,&$values) {
            $key = array_keys($index)[0];
            $keys[] = $key;
            $values[] = $index[$key];
        });
        $field = implode(",",$keys);
        $value = "'".implode("','",$values)."'";
        $this->query = "INSERT INTO $table ($field) VALUES ($value)";
        return $this;
    }

    public function Filter(string $field,$value,int $condition=self::EQUAL):QueryBuilder{

        switch ($condition){
            case self::EQUAL:
                $c = "=";
                break;
            case self::GREATER:
                $c = ">";
                break;
            case self::LESSER:
                $c = "<";
                break;
            case self::LESSER_EQUAL:
                $c = "<=";
                break;
            case self::GREATER_EQUAL:
                $c = ">=";
                break;
            case self::NOT_EQUAL:
                $c = "<>";
                break;
            default:
                $c = "=";
                break;
        }

        $this->query .= "WHERE $field $c $value";
        return $this;
    }

    public function Reset(){
        $this->query = "";
    }

    public function getQuery(){
        return $this->query;
    }

}