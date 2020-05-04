<?php

namespace application\Init;
use appExtension;
use application\Application;
use application\database\Database;
use application\locations\Location;

class Load{

    function __construct(){}

    public function View(){
        return new DataView();
    }

    public function Module(){
        return new DataModule();
    }

    public function Widget(){
        return new DataWidget();
    }

    function __destruct(){
        $this->module = $this->widget = $this->view=null;
    }

}

class DataView implements IView {

    public function __construct(){

    }

    public function Exist($filename){
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_VIEW_DIRECTORY.APP_VIEW_PREFIX.$filename.appExtension::view;
        return file_exists($dir);
    }

    public function Load($filename,array $param=[]):DataView{
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_VIEW_DIRECTORY.APP_VIEW_PREFIX.$filename.appExtension::view;
        if(file_exists($dir)){
            $View_Data = $param;
            $View_Controller = new Application();
            $View_Database = new Database();
            include($dir);
        }else{
            Die("View File '$filename' Does not Exist");
        }
        return $this;
    }

}

class DataModule implements IModule{

    public function __construct(){

    }

    public function Exist($filename){
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_MODE_DIRECTORY.APP_MODULE_PREFIX.$filename.appExtension::module;
        return file_exists($dir);
    }

    public function Load($filename,array $param=[]):DataModule{
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_MODE_DIRECTORY.APP_MODULE_PREFIX.$filename.appExtension::module;
        $controller = new Application();
        if(file_exists($dir)){
            $Mold_Data = $param;
            $Mold_Controller = $controller;
            $Mold_Database = new Database();
            $mold_Request = $controller->PublicRequest();
            include($dir);
        }else{
            Die("Module File '$filename' Does not Exist");
        }
        return $this;
    }

}

class DataWidget implements IWidget {

    public function __construct(){

    }

    public function Exist($filename):bool{
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_WIDGET_DIRECTORY.APP_WIDGET_PREFIX.$filename.appExtension::view;
        return file_exists($dir);
    }

    public function Load($filename,array $param=[]):DataWidget{
        $filename = str_replace("-","_",strtolower($filename));
        $dir = APP_WIDGET_DIRECTORY.APP_WIDGET_PREFIX.$filename.appExtension::widget;
        if(file_exists($dir)){
            $Widget_Data = $param;
            $Widget_Controller = new Application();
            $Widget_Database = new Database();
            include($dir);
        }else{
            Die("Widget File '$filename' Does not Exist");
        }
        return $this;
    }

}

