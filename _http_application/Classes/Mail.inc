<?php

namespace application\init;


use Exception;

class Mail{

    private $to = null;
    private $charset = "ISO-8859-1";
    private $subject = null;
    private $header = null;
    private $message = null;
    private $result = null;
    private $additional = null;


    function __construct($to=null){
        $this->to = null;
    }

    function AddTo(string $to):Mail{
        $this->to = $to;
        return $this;
    }

    function AddFrom(string $from):Mail{
        $this->header['From'] = $from;
        return $this;
    }

    function AddReplyTo(string $reply):Mail{
        $this->header['Reply-To'] = $reply;
        return $this;
    }

    function AddCC(string $cc):Mail{
        $this->header['CC'] = $cc;
        return $this;
    }

    function AddMIME(string $mime):Mail{
        $this->header['MIME-Version'] = $mime;
        return $this;
    }

    function AddContentType(string $type):Mail{
        $this->header['Content-Type'] = $type.$this->charset;
        return $this;
    }

    function AddCharset(string $charset):Mail{
        $this->charset = $charset;
        return $this;
    }

    function AddXMailer(string $mailer=null):Mail{
        $this->header['X-Mailer'] = is_null($mailer)?"PHP/".PHP_VERSION:$mailer;
        return $this;
    }

    function AddHeaders(string $from=null,string $reply_to=null,string $cc=null,string $mime=null,string $content_type=null,string $xmailer=null):Mail{
        if(!is_null($from)) $this->header['From'] = $from;
        if(!is_null($reply_to)) $this->header['Reply-To'] = $reply_to;
        if(!is_null($cc)) $this->header['CC'] = $cc;
        if(!is_null($mime)) $this->header['MIME-Version'] = $mime;
        if(!is_null($content_type)) $this->header['Content-Type'] = $content_type.$this->charset;
        if(!is_null($xmailer)) $this->header['X-Mailer'] = $xmailer;
        return $this;
    }

    function setTypeHtml(bool $value=false):Mail{
        if($value){
            $this->header['MIME-Version'] = "1.0";
            $this->header['Content-Type'] = "text/html; $this->charset";
        }else{
            unset($this->header['MIME-Version']);
            unset($this->header['Content-Type']);
        }
        return $this;
    }

    function isTypeHtml():bool{
        return isset($this->header['MIME-Version'],$this->header['Content-Type']);
    }

    function AddAdditionalParam($additional){
        $this->additional .= $additional;
    }

    function AddSubjects(string $subject):Mail{
        $this->subject = $subject;
        return $this;
    }

    function AddMessage(string $message):Mail{
        $this->message = $message;
        return $this;
    }

    function Send():Mail{
        $this->AddXMailer();
        try{
            $headerData = $this->header;
            $header = "";
            foreach ($headerData as $key=>$value){
                if(empty($key) or empty($value))continue;
                $header .= "$key: $value \r\n";
            }
            $this->result = mail($this->to,$this->subject,$this->message,$header,$this->additional);
        }catch (Exception $ex){
            $this->result = $ex->getMessage();
        }
        return $this;
    }

    function isSent():bool{
        if(is_bool($this->result)){
            return $this->result;
        }
        return false;
    }

    function isError(){
        if($this->message);
    }

}

