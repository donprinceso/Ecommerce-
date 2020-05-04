<?php
namespace application\Init;
/*You can add external page as method to this class*/
use application\Application;

class PagesController implements IPage{

    protected $control = null;
    protected $view = null;
    protected $widget = null;

    function __construct(){
        $this->control = new Application();
        $this->view = $this->control->Load()->View();
        $this->widget = $this->control->Load()->Widget();
    }

    function Exist($name){
        return method_exists($this,$name);
    }

    public function index(){
        if($this->view->Exist("index")) {
            $this->view->Load("index");
        }
    }

}
