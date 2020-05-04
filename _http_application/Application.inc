<?php
namespace application;

use application\database\DatabasePDO;
use application\Init\ApplicationController;

class Application extends ApplicationController {

    public function handle_request(){
        $module = $this->Load()->Module();
        $pages = $this->Page();
        $request = $this->PublicRequest();
        if($request->RequestAvailable()){
            $request = $request->ProcessRequest($request->GetRequest());
            $file_name = $request[0];
            if($module->Exist($file_name)){
                $module->Load($file_name);
            }else{
                if($pages->Exist($file_name)){
                    $pages->$file_name();
                }else{
                    $module->Load(APP_DEFAULT_404PAGE);
                }
            }
        }else{
            $home = APP_DEFAULT_HOMEPAGE;
            if($pages->Exist($home)){
                $pages->$home();
            }else{
                if($module->Exist($home)) {
                    $module->Load($home);
                }else{
                    die("There is not default page specified.");
                }
            }

        }
    }

}