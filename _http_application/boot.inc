<?php
//Checking PHP Version
if (!(version_compare(PHP_VERSION, '7.1.0') >= 0)){
    die("Framework was built on PHP7 and it requires PHP 7.1.0 or above to run");
}
if(!defined("King_Framework")) die("You cannot access boot file directly");

//starting a session
session_start();

$configure = "configure.inc";
$dirname = dirname(__FILE__);
require_once $dirname.DIRECTORY_SEPARATOR.$configure;

$interfaces = APP_FIRST_LOAD;
$traits = APP_SECOND_LOAD;
$classes = APP_THIRD_LOAD;
$thirdparty = APP_FORTH_LOAD;

function get_files($dir,$configure) :bool{
    $cdir = scandir($dir);
    foreach ($cdir as $key => $value)
    {
        if (!in_array($value,array(".",".."))) {
            $current_dir = $dir.DS.$value;

            if (is_dir($current_dir)) {
                //checking for special dir and excluding them
                if(in_array(($current_dir.DS),[APP_MODE_DIRECTORY,APP_VIEW_DIRECTORY,APP_WIDGET_DIRECTORY,APP_FIRST_LOAD,APP_SECOND_LOAD,APP_THIRD_LOAD,APP_FORTH_LOAD]))continue;

                get_files($current_dir,$configure);

            } else {
                if($current_dir==__FILE__ or $value==$configure)continue;
                require $current_dir;
            }
        }
    }
    return true;
}

if(get_files($interfaces,$configure) and get_files($traits,$configure) and get_files($classes,$configure)){
    if(get_files($dirname,$configure)) {
        require_once($thirdparty.'loader.inc');
    }
}

