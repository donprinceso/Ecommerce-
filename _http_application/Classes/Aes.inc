<?php
declare(strict_types=1);
namespace application\security;

use application\Init\IClasses;
use Exception;
use MongoDB\BSON\Type;

class Aes implements IClasses {

    protected $key;
    protected $data;
    protected $method;

    protected $options = 0;

    function __construct($data = null, $key = null, $blockSize = null, $mode = 'CBC') {
        $this->setData($data);
        $this->setKey($key);
        $this->setMethode($blockSize, $mode);
    }

    public function setData($data) {
        $this->data = $data;
    }
    /**
     *
     * @param type $key
     */
    public function setKey($key) {
        $this->key = $key;
    }

    public function setMethode($blckSize, $mode = 'CBC') {
        if($blckSize==192 && in_array('', array('CBC-HMAC-SHA1','CBC-HMAC-SHA256','XTS'))){
            $this->method=null;
            throw new Exception('Invlid block size and mode combination!');
        }
        $this->method = 'AES-' . $blckSize . '-' . $mode;
    }

    public function validateParams() {
        if ($this->data != null &&
            $this->method != null ) {
            return true;
        } else {
            return FALSE;
        }
    }

    protected function getIV() {
        return '1234567890123456';
    }

    public function encrypt() {
        if ($this->validateParams()) {
            return trim(openssl_encrypt($this->data, $this->method, $this->key, $this->options,$this->getIV()));
        } else {
            throw new Exception('Invlid params!');
        }
    }

    public function decrypt() {
        if ($this->validateParams()) {
            $ret=openssl_decrypt($this->data, $this->method, $this->key, $this->options,$this->getIV());
            if(is_string($ret)){
                return trim($ret);
            }else{
                return $ret;
            }
        } else {
            throw new Exception('Invlid params!');
        }
    }
}
