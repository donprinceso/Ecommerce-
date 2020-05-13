<?php

namespace application\init;

use application\database\TImage;
use application\locations\Url;
use Exception;
use stdClass;

class Image implements IClasses {

    private $img;

    private $url;
    private $extension;

    use TImage;

    function __construct(string $imgUrl=null){
       if(!is_null($imgUrl)){
           $this->setImage($imgUrl);
       }
    }

    function setImage($imgUrl){
        $this->url = $imgUrl;
        $img = explode(".",$imgUrl);
        $extension = end($img);
        $this->extension = $extension;
        switch ($extension) {
            case "jpg":
                $this->img = @imagecreatefromjpeg($imgUrl);
                if(!$this->img){
                    $this->img = @imagecreatefromstring(file_get_contents($imgUrl));
                }
                break;
            case "jpeg":
                $this->img = imagecreatefromjpeg($imgUrl);
                break;
            case "png":
                $this->img = imagecreatefrompng($imgUrl);
                break;
            case "gif":
                $this->img = imagecreatefromgif($imgUrl);
                break;
            case "bmp":
                $this->img = imagecreatefromwbmp($imgUrl);
                break;
            case "xpm":
                $this->img = imagecreatefromxpm($imgUrl);
                break;
            case "gd":
                $this->img = imagecreatefromgd($imgUrl);
                break;
            case "gd2":
                $this->img = imagecreatefromgd2($imgUrl);
                break;
            default:
                return null;
                break;
        }
        return $this;
    }

    function Draw($width,$height){
        $this->img = imagecreate($width,$height);
    }

    function getImage(){
        return $this->img;
    }

    function Info():InfoImg{
        return new InfoImg($this->img);
    }

    function Render():RenderImg{
        return new RenderImg($this->img);
    }

    function IsImage($image):bool{
        return $this->Validate($image,['jpeg','jpg','png','gif','xpm','gd'],null)->status;
    }

    function Validate($image,array $extf=['jpeg','jpg','png','gif'],$maxsize=2000000):stdClass{ //2mb
        $result = new stdClass();
        $result->status = false;
        $result->error = false;
        $result->size = true;
        $result->format = true;
        $imgName = ((isset($image['name']))?$image['name']:null);
        $imgType = ((isset($image['type']))?$image['type']:null);
        $imgSize = ((isset($image['size']))?$image['size']:null);
        $imgError = ((isset($image['error']))?$image['error']:null);
        $imgTemp = ((isset($image['tmp_name']))?$image['tmp_name']:null);

        $mimef = [];
        array_walk($extf,function($index) use (&$mimef){
            $mimef[] = "image/$index";
        });

        if($imgError == 0) {
            $temp = explode(".", $imgName);
            $extension = end($temp);
            $finfo = finfo_open(FILEINFO_MIME_TYPE);
            $mime = finfo_file($finfo, $imgTemp);
            if(in_array($mime,$mimef) and in_array($extension,$extf) and in_array($imgType,$mimef)){
                if(($imgSize<=$maxsize) or $maxsize==null){
                    $result->status = true;
                }else{
                    $result->size = false;
                }
            }else{
                $result->format = false;
            }
        }else{
            $result->error = true;
            switch ($imgError){
                default:
                    $result->info = "Image is not valid";
            }
        }
        return $result;
    }

    public function UploadImageUrl($temp,string $name,bool $auto_generate_url=true):string {
        $finfo = finfo_open(FILEINFO_MIME_TYPE);
        $mime = finfo_file($finfo, $temp);
        $ext = $this->processExtension($mime);
        if($auto_generate_url){
            $partUrl = Url::get_upload_generate_dir("/").$name.".$ext";
            $fullUrl = Url::get_server_upload_root_url(Url::get_server_upload_autogenerate_url($name.".$ext"));
        }else{
            $partUrl = Url::Upload_dir().$name.".$ext";
            $fullUrl = Url::get_server_upload_root_url($name.".$ext");
        }
        if($this->isJPEG($mime)){
            $this->StripImageHeaderInfoJpeg($temp);
        }
        if(move_uploaded_file($temp,$fullUrl)){
            return $partUrl;
        }
        return null;
    }

    public function UploadImage($temp,string $name,bool $auto_generate_url=true):Image {
        $imgUrl = $this->UploadImageUrl($temp,$name,$auto_generate_url);
        if($imgUrl!=null){
            return $this->setImage($imgUrl);
        }
        return null;
    }

    public function PortraitOrientation():Image{
        $deg = 0;
        if($this->extension!=="png") {
            $exif = exif_read_data($this->url);
            if ($exif && isset($exif['Orientation'])) {
                $orientation = $exif['Orientation'];
                if ($orientation != 1) {
                    switch ($orientation) {
                        case 3:
                            $deg = 180;
                            break;
                        case 6:
                            $deg = 270;
                            break;
                        case 8:
                            $deg = 90;
                            break;
                    }
                }
            }
        }
        $this->img = imagerotate($this->img, $deg, 0);
        return $this;
    }

    public function Recreate(int $width,int $height=null):Image{
        error_reporting(E_ERROR);
        $height = (is_null($height))?$width:$height;
        list($width_orig, $height_orig) = getimagesize($this->url);
        $ratio_orig = $width_orig/$height_orig;
        if ($width/$height > $ratio_orig){
            $width = $height*$ratio_orig;
        }else{
            $height = $width/$ratio_orig;
        }
        $image_p = imagecreatetruecolor($width, $height);
        imagecopyresampled($image_p, $this->img, 0, 0, 0, 0, $width, $height, $width_orig, $height_orig);
        $this->img = $image_p;
        return $this;
    }

    public function Square():Image{
        error_reporting(E_ERROR);
        $size = min(imagesx($this->img), imagesy($this->img));
        $this->img = imagecrop($this->img, ['x' => 0, 'y' => 0, 'width' => $size, 'height' => $size]);
        return $this;
    }

    public function Watermark():Watermark{
        return new Watermark($this->img);
    }

    function FlipVertical(){
        $this->img = imageflip($this->img,IMG_FLIP_VERTICAL);
    }

    function FlipHorizontal(){
        $this->img = imageflip($this->img,IMG_FLIP_HORIZONTAL);
    }

    function Flip(){
        $this->img = imageflip($this->img,IMG_FLIP_BOTH);
    }

    public function mimeToExtension(string $mime){
        image_type_to_extension($mime,true);
    }

    function Remove():bool{
        return (!is_null($this->img))? imagedestroy($this->img): false;
    }


    function __destruct(){
        $this->Remove();
    }

}

class Watermark{

    private $img;
    private $padding = 10;
    private $position = ['x'=>10,'y'=>10];
    private $bgcolor = ImageSettings::COLOR_WHITE;
    private $size = ['width'=>100,'height'=>30];
    private $fontSize = 14;
    private $textcolor = ImageSettings::COLOR_BLACK;
    private $text = "Text";
    private $cage;

    function __construct($img){
        $this->img = $img;
    }

    public function getImageJPEG(){
        return imagejpeg($this->img,null,70);
    }

    public function setPosition(int $position=ImageSettings::POSITION_TOP_LEFT):Watermark{
        $size = $this->getImageSize();
        $padding = $this->padding;
        $sizeX = $size['x'];
        $sizeY = $size['y'];
        switch ($position){
            case ImageSettings::POSITION_TOP_LEFT:
                $this->position = ['x'=>$padding,'y'=>$padding];
                break;
            case ImageSettings::POSITION_TOP_CENTER:
                $x = $this->getImageSize()['x'];
                $px = ($x/2) - $this->size['width'] - $padding;
                $this->position = ['x'=>10,'y'=>10];
                break;
            case ImageSettings::POSITION_TOP_RIGHT:
                $x = $this->getImageSize()['x'];
                $px = $x - $this->size['width'] - $padding;
                $this->position = ['x'=>$px,'y'=>$padding];
                break;
            case ImageSettings::POSITION_CENTER_LEFT:
                $this->position = ['x'=>$padding,'y'=>$padding];
                break;
        }
        return $this;
    }

    public function setPadding(int $padding):self {
        $this->padding = $padding;
        return $this;
    }

    public function setCustomPosition(int $x,int $y):Watermark{
        $this->position = ['x'=>$x,'y'=>$y];
        return $this;
    }

    public function setTextColor($color){
        return $this->textcolor = $color;
    }

    public function setTextColorRGB(int $red=0,int $green=0,int $blue=0){
        $this->textcolor = imagecolorallocate($this->img,$red,$green,$blue);
        return $this;
    }

    public function setCageColor($color){
        return $this->bgcolor = $color;
    }

    public function setCageColorRGB(int $red=0,int $green=0,int $blue=0){
        $this->bgcolor = imagecolorallocate($this->img,$red,$green,$blue);
        return $this;
    }

    public function setCageSize(int $width,int $height){
        $this->size = ['width'=>$width,'height'=>$height];
        return $this;
    }

    public function setFontSize($size=14):Watermark{
        $this->fontSize = $size;
        return $this;
    }

    public function setText($text){
        $this->text = $text;
        return $this;
    }

    public function getImageSize(){
        return ['x'=>imagesx($this->img),'y'=>imagesy($this->img)];
    }

    public function addRectangle(){
        $x = $this->position['x'];
        $y = $this->position['y'];
        $width = $x+$this->size['width'];
        $height = $y+$this->size['height'];
        $this->createCage();
        imagerectangle($this->cage,0,0,$width,$height,$this->bgcolor);
        //$this->blend();
        return $this;
    }

    public function addRectangleFill(int $opacity):self {
        $width = $this->size['width'];
        $height = $this->size['height'];
        $this->createCage();
        imagefilledrectangle($this->cage,0,0,$width,$height,$this->bgcolor);
        $this->blend($opacity);
        return $this;
    }

    private function createCage(){
        $width = $this->size['width'];
        $height = $this->size['height'];
        $this->cage = imagecreatetruecolor($width,$height);
    }

    private function blend(int $opacity){
        $width = $this->size['width'];
        $height = $this->size['height'];
        $x = $this->position['x'];
        $y = $this->position['y'];
        imagecopymerge($this->img, $this->cage,$x,$y,0,0,$width,$height,$opacity);
    }

    public function RenderCage():RenderImg{
        return new RenderImg($this->cage);
    }

    public function Render():RenderImg{
        return new RenderImg($this->img);
    }


}

class InfoImg{

    private $img;

    function __construct($img){
        $this->img = $img;
    }

    function getWidth():int{
        return imagesx($this->img);
    }

    function getHeight():int{
        return imagesy($this->img);
    }

    function getType(){

    }

}

class RenderImg{
    private $img;
    private $quality = 95;

    function __construct($img){
        $this->img = $img;
    }

    public function setQuality(int $val=80):RenderImg{
        $this->quality = $val;
        return $this;
    }

    public function setSize(int $width,int $height=null,$mode=IMG_BILINEAR_FIXED):RenderImg{
        if(!is_null($height) and is_numeric($height)) {
            $this->img = imagescale($this->img, $width, $height,$mode);
        }else{
            $this->img = imagescale($this->img, $width,-1,$mode);
        }
        return $this;
    }

    public function getQuality(){
        return $this->quality;
    }

    public function toJPEG(){
        return imagejpeg($this->img,null,$this->quality);
    }

    public function saveJPEG(string $url){
        return imagejpeg($this->img,$url,$this->quality);
    }

    public function savePNG(string $url){
        $quality = $this->quality/100;
        return imagepng($this->img,$url,$quality);
    }

    public function toPNG(){
        $quality = intdiv($this->quality,10);
        return imagepng($this->img,null,$quality);
    }

}

abstract class ImageSettings{
    public const POSITION_TOP_LEFT = 0;
    public const POSITION_TOP_CENTER = 1;
    public const POSITION_TOP_RIGHT = 2;
    public const POSITION_CENTER_LEFT = 3;
    public const POSITION_CENTER_CENTER = 4;
    public const POSITION_CENTER_RIGHT = 5;
    public const POSITION_BOTTOM_LEFT = 6;
    public const POSITION_BOTTOM_CENTER = 7;
    public const POSITION_BOTTOM_RIGHT = 8;

    //color ===========================================

    public const COLOR_WHITE = 16777215;
    public const COLOR_BLACK = 0;
    public const COLOR_PINK = 14165108;
    public const COLOR_RED = 12517376;
    public const COLOR_BLUE = 427431;

}