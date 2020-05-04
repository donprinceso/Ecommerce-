<?php

namespace application\security;


class EncodingClass{

    //json
    public function JSON():Json{
        return new Json();
    }

    //utf8
    public function UTF_8():Utf8{
        return new Utf8();
    }

}


class Json{
    private $jsondata;

    public function Get(){

    }

    public function toJSON($data){
        return json_encode($data);
    }

}

class Utf8{
    private $jsondata;

    public function toJSON($data){
        return json_encode($data);
    }

}
