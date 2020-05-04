<?php
//This is an Authenticator
use application\Application;

define("King_Framework",dirname(__FILE__));

//This calls all the required files
require("_http_application/boot.inc");

$app = new Application();

$app->handle_request();

