<?php

namespace application;
use application\Init\PagesController;
use application\security\Hashing;
use model\Admin;

class Pages extends PagesController{


    /****THIS IS THE ADMIN SECTION ====================****/
    public function admin(){
       $Admin = new Admin();
       if($Admin->isLogedin()){
           if ($this->view->Exist("admin")){
               $this->view->Load("admin");
           }
       }else{
           if ($this->view->Exist("admin-login")) {
               $this->view->Load("admin-login");
           }
       }
    }






    /****THE CUSTOMER SECTION =========================****/
    public function index()
    {
        if($this->view->Exist("public-index")){
            $this->view->Load("public-index");
        }
    }

    public function login(){
        if ($this->view->Exist("public-login")){
            $this->view->Load("public-login");
        }
    }

    public function collectionList(){
        if($this->view->Exist("public-collectionList")){
            $this->view->Load("public-collectionList");
        }
    }

    public  function register(){
        if ($this->view->Exist("public-signup")){
            $this->view->Load("public-signup");
        }
    }

    public  function contact(){
        if ($this->view->Exist("public-contact")){
            $this->view->Load("public-contact");
        }
    }

    public  function cart(){
        if ($this->view->Exist("public-cart")){
            $this->view->Load("public-cart");
        }
    }

    public  function collection(){
        if ($this->view->Exist("public-collection")){
            $this->view->Load("public-collection");
        }
    }

    public  function news(){
        if ($this->view->Exist("public-news")){
            $this->view->Load("public-news");
        }
    }

    public  function about(){
        if ($this->view->Exist("public-about")){
            $this->view->Load("public-about");
        }
    }

    public  function faq(){
        if ($this->view->Exist("public-faq")){
            $this->view->Load("public-faq");
        }
    }

    public function hash($pass = "123456"){
        $con = new Application();
       echo $con->Hashing()->PasswordHash($pass);

    }

    public  function product(){
        if ($this->view->Exist("public-product")){
            $this->view->Load("public-product");
        }
    }
}