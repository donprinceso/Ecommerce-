<?php
namespace application\security;
//Encryption codes are here
use application\Init\IClasses;

class EncryptionClass implements IClasses {

    public function AesEncrypt($inputText){
        try {
            $inputKey = md5(APP_ENCRYPTION_AES);
            $blockSize = 256;
            $aes = new Aes($inputText, $inputKey, $blockSize);
            return $aes->encrypt();
        }catch (\Exception $ex){
            die($ex);
        }
    }

    public function AesDecrypt($data){
        try {
            $inputKey = md5(APP_ENCRYPTION_AES);
            $blockSize = 256;
            $aes = new Aes("",$inputKey,$blockSize);
            $aes->setData($data);
            return $aes->decrypt();
        }catch (\Exception $ex){
            die($ex);
        }
    }

    /** * Encrypt the passed file and saves the result in a new file with ".enc" as suffix.
     * * @param string $source Path to file that should be encrypted
     * @param string $key    The key used for the encryption
     * @param string $dest   File name where the encryped file should be written to.
     * @return string|false  Returns the file name that has been created or FALSE if an error occurred */
    function OpenSSLEncryptFile($source, $key, $dest)
    {
        $key = substr(sha1($key, true), 0, 16);
        $iv = openssl_random_pseudo_bytes(16);
        $error = false;
        if ($fpOut = fopen($dest, 'w')) {
            fwrite($fpOut, $iv);
            if ($fpIn = fopen($source, 'rb')) {
                while (!feof($fpIn)) {
                    $plaintext = fread($fpIn, 16 * APP_FILE_ENCRYPTION_BLOCKS);
                    $ciphertext = openssl_encrypt($plaintext, 'AES-128-CBC', $key, OPENSSL_RAW_DATA, $iv);
                    $iv = substr($ciphertext, 0, 16);
                    fwrite($fpOut, $ciphertext);
                }
                fclose($fpIn);
            } else {
                $error = true;
            }
            fclose($fpOut);
        } else {
            $error = true;
        }
        return $error ? false : $dest;
    }


    /** * Dencrypt the passed file and saves the result in a new file, removing the * last 4 characters from file name.
     * * @param string $source Path to file that should be decrypted
     * @param string $key    The key used for the decryption (must be the same as for encryption)
     * @param string $dest   File name where the decryped file should be written to.
     * @return string|false  Returns the file name that has been created or FALSE if an error occurred */
    function OpenSSLDecryptFile($source, $key, $dest) {
        $key = substr(sha1($key, true), 0, 16);
        $error = false;
        if ($fpOut = fopen($dest, 'w')) {
            if ($fpIn = fopen($source, 'rb')) {
                // Get the initialzation vector from the beginning of the file
                $iv = fread($fpIn, 16);
                while (!feof($fpIn)) {
                    $ciphertext = fread($fpIn, 16 * (APP_FILE_ENCRYPTION_BLOCKS + 1));
                    // we have to read one block more for decrypting than for encrypting
                    $plaintext = openssl_decrypt($ciphertext, 'AES-128-CBC', $key, OPENSSL_RAW_DATA, $iv);
                    // Use the first 16 bytes of the ciphertext as the next initialization vector
                    $iv = substr($ciphertext, 0, 16);
                    fwrite($fpOut, $plaintext);
                }
                fclose($fpIn);
            } else {
                $error = true;
            }
            fclose($fpOut);
        } else {
            $error = true;
        }
        return $error ? false : $dest;
    }


    public function Open_SSL_Encryption($text,$password){
        if (in_array("aes-128-gcm", openssl_get_cipher_methods())) {
            $ivlen = openssl_cipher_iv_length("aes-128-gcm");
            $iv = openssl_random_pseudo_bytes($ivlen);
            return openssl_encrypt($text, "aes-128-gcm", $password, $options=0, $iv);
        }
        return null;
    }

    public function Open_SSL_Decryption($ciphertext,$password){
        if (in_array("aes-128-gcm", openssl_get_cipher_methods())) {
            $ivlen = openssl_cipher_iv_length("aes-128-gcm");
            $iv = openssl_random_pseudo_bytes($ivlen);
            return openssl_decrypt($ciphertext, "aes-128-gcm", $password, $options=0, $iv);
        }
        return null;
    }

}
