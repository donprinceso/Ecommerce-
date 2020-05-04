<?php

namespace application;



abstract class Header{
    #html
    const HTML = "Content-Type: text/html; charset=UTF-8";

    #images
    const JPEG = "Content-Type: image/jpeg";
    const PNG = "Content-Type: image/png";
    const GIF = "Content-Type: image/gif";
    const JSON = "Content-Type: application/json; charset=UTF-8";

    const STREAM = "Content-Type: text/event-stream";
    const NOCACHE = "Cache-Control: no-cache";
    const TRANSPORT_SECURITY = "strict-transport-security: max-age=600";


    static function setHeader($header="",$replace=true,$response=null){
        header($header, $replace,$response);
    }

}

