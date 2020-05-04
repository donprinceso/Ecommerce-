<?php

namespace application;
use application\Init\PagesController;

class Pages extends PagesController{

    public function test(){
        if($this->view->Exist("test")) {
            $this->view->Load("test");
        }
    }

    public function index()
    {
        if($this->view->Exist("public-index")){
            $this->view->Load("public-index");
        }
    }

    public function dashboard(){
        if ($this->view->Exist("admin-dashboard")){
            $this->view->Load("admin-dashboard");
        }
    }

   public function admin_login(){
       if ($this->view->Exist("admin-login")) {
           $this->view->Load("admin-login");
       }
    }

    public function admin_signup(){
       if ($this->view->Exist("admin-signup")){
           $this->view->Load("admin-signup");
       }
    }

    public function addCollection(){
        if ($this->view->Exist("admin-collectionsAdd")){
            $this->view->Load("admin-collectionsAdd");
        }
    }

    public function editCollection(){
        if ($this->view->Exist("admin-collectionsEdit")){
            $this->view->Load("admin-collectionsEdit");
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

    public  function product(){
        if ($this->view->Exist("public-product")){
            $this->view->Load("public-product");
        }
    }
}